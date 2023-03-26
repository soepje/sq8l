#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ProgramManager.h"
#include "ParameterHelper.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
        : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
        synth(44100),
        parameters(*this, nullptr, juce::Identifier ("PROGRAM"), ParameterHelper::createParameterLayout()),
        programManager(parameters) {
    synth.mSettings = &settings;
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
    return true;
}

bool AudioPluginAudioProcessor::producesMidi() const
{
    return false;
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
    return false;
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms() {
    return programManager.getNumPrograms();
}

int AudioPluginAudioProcessor::getCurrentProgram() {
    return programManager.getCurrentProgram();
}

void AudioPluginAudioProcessor::setCurrentProgram (int index) {
    programManager.setCurrentProgram(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index) {
    return programManager.getProgramName(index);
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName) {
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setSampleRate(sampleRate);
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages) {
    int numSamples = buffer.getNumSamples();

    updateSettings();

    juce::ScopedNoDenormals noDenormals;

    midiKeyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);

    auto* leftChannelData = buffer.getWritePointer (0);
    auto* rightChannelData = buffer.getWritePointer (1);

    for (int i = 0; i < numSamples; i++) {
        for (auto it = midiMessages.findNextSamplePosition(i); it != midiMessages.cend(); it++) {
            const juce::MidiMessageMetadata metadata = *it;
            if (metadata.samplePosition > i) {
                break;
            }

            const juce::MidiMessage message = metadata.getMessage();

            if (message.isNoteOn()) {
                synth.triggerNote(message.getChannel(), message.getNoteNumber(), message.getVelocity());
            } else if (message.isNoteOff()) {
                synth.triggerNote(message.getChannel(), message.getNoteNumber(), 0);
            } else if (message.isProgramChange()) {
                setCurrentProgram(message.getProgramChangeNumber());
                updateSettings();
            }
        }

        float left = 0.0, right = 0.0;

        synth.getSamples(true, &left, 1, &right);

        leftChannelData[i] = left;
        rightChannelData[i] = right;
    }

    midiMessages.clear();
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

// TODO this is inefficient, maybe use a lock free buffer in the audio processing loop to process parameter change messages individually
void AudioPluginAudioProcessor::updateSettings() {
    for (int i = 0; i < 3; i++) {
        auto osc = "OSC" + std::to_string(i + 1);
        auto dca = "DCA" + std::to_string(i + 1);

        SynthOscSettings* s = &synth.mSettings->oscSettings[i];
        s->octave = static_cast<char>(parameters.getRawParameterValue(osc + ".OCT")->load());
        s->semi = static_cast<char>(parameters.getRawParameterValue(osc + ".SEMI")->load());
        s->fine = static_cast<char>(parameters.getRawParameterValue(osc + ".FINE")->load());
        s->wave = static_cast<unsigned short>(parameters.getRawParameterValue(osc + ".WAVE")->load());
        s->modulationSource1 = static_cast<short>(parameters.getRawParameterValue(osc + ".MOD1.SEL")->load()) - 1;
        s->modulationAmount1 = static_cast<char>(parameters.getRawParameterValue(osc + ".MOD1.AMT")->load());
        s->modulationSource2 = static_cast<short>(parameters.getRawParameterValue(osc + ".MOD2.SEL")->load()) - 1;
        s->modulationAmount2 = static_cast<char>(parameters.getRawParameterValue(osc + ".MOD2.AMT")->load());

        s->level = static_cast<char>(parameters.getRawParameterValue(dca + ".LEVEL")->load());
        s->enabled = static_cast<char>(parameters.getRawParameterValue(dca + ".OUTPUT")->load());
        s->modulationSource3 = static_cast<short>(parameters.getRawParameterValue(dca + ".MOD1.SEL")->load()) - 1;
        s->modulationAmount3 = static_cast<char>(parameters.getRawParameterValue(dca + ".MOD1.AMT")->load());
        s->modulationSource4 = static_cast<short>(parameters.getRawParameterValue(dca + ".MOD2.SEL")->load()) - 1;
        s->modulationAmount4 = static_cast<char>(parameters.getRawParameterValue(dca + ".MOD2.AMT")->load());
    }

    {
        FilterSettings* s = &synth.mSettings->filterSettings;
        s->cutoff = static_cast<char>(parameters.getRawParameterValue("FILT.FREQ")->load());
        s->resonance = static_cast<char>(parameters.getRawParameterValue("FILT.RES")->load());
        s->noteModulationAmount = static_cast<char>(parameters.getRawParameterValue("FILT.KEYBD")->load());
        s->modulationSource1 = static_cast<short>(parameters.getRawParameterValue("FILT.MOD1.SEL")->load()) - 1;
        s->modulationAmount1 = static_cast<char>(parameters.getRawParameterValue("FILT.MOD1.AMT")->load());
        s->modulationSource2 = static_cast<short>(parameters.getRawParameterValue("FILT.MOD2.SEL")->load()) - 1;
        s->modulationAmount2 = static_cast<char>(parameters.getRawParameterValue("FILT.MOD2.AMT")->load());
    }

    {
        AmpSettings* s = &synth.mSettings->ampSettings;
        s->envelopeAmplitudeModulationAmount = static_cast<char>(parameters.getRawParameterValue("DCA4.AMT")->load());
        s->saturation = static_cast<char>(parameters.getRawParameterValue("DCA4.SAT")->load()) - 1;
        s->stereoPanning = static_cast<char>(parameters.getRawParameterValue("DCA4.PAN")->load());
        s->amplitudeModulationSource = static_cast<short>(parameters.getRawParameterValue("DCA4.AMOD.SEL")->load()) - 1;
        s->amplitudeModulationAmount = static_cast<char>(parameters.getRawParameterValue("DCA4.AMOD.AMT")->load());
        s->panningModulationSource = static_cast<short>(parameters.getRawParameterValue("DCA4.PMOD.SEL")->load()) - 1;
        s->panningModulationAmount = static_cast<char>(parameters.getRawParameterValue("DCA4.PMOD.AMT")->load());
    }

    {
        synth.mSettings->sync = static_cast<char>(parameters.getRawParameterValue("MODE.SYNC")->load());
        synth.mSettings->am = static_cast<char>(parameters.getRawParameterValue("MODE.AM")->load());
        synth.mSettings->mono = static_cast<char>(parameters.getRawParameterValue("MODE.MONO")->load());
        synth.mSettings->glide = static_cast<char>(parameters.getRawParameterValue("MODE.GLIDE")->load());
        synth.mSettings->restartVoice = static_cast<char>(parameters.getRawParameterValue("MODE.REST.VC")->load());
        synth.mSettings->restartEnvelopes = static_cast<char>(parameters.getRawParameterValue("MODE.REST.ENV")->load());
        synth.mSettings->restartOscillators = static_cast<char>(parameters.getRawParameterValue("MODE.REST.OSC")->load());
        synth.mSettings->cycle = static_cast<char>(parameters.getRawParameterValue("MODE.CYC")->load());
    }

    for (int i = 0; i < 4; i++) {
        auto lfo = "LFO" + std::to_string(i + 1);

        SynthLfoSettings* s = &synth.mSettings->lfoSettings[i];
        s->frequency = static_cast<unsigned char>(parameters.getRawParameterValue(lfo + ".FREQ")->load());
        s->reset = static_cast<char>(parameters.getRawParameterValue(lfo + ".RESET")->load());
        s->humanize = static_cast<char>(parameters.getRawParameterValue(lfo + ".HUMAN")->load());
        s->wave = static_cast<unsigned char>(parameters.getRawParameterValue(lfo + ".WAVE")->load());
        s->startAmplitude = static_cast<char>(parameters.getRawParameterValue(lfo + ".L1")->load());
        s->finalAmplitude = static_cast<char>(parameters.getRawParameterValue(lfo + ".L2")->load());
        s->fading = static_cast<unsigned char>(parameters.getRawParameterValue(lfo + ".DELAY")->load());

        s->amplitudeModulationSource = static_cast<short>(parameters.getRawParameterValue(lfo + ".AMOD.SEL")->load()) - 1;
        s->amplitudeModulationAmount = static_cast<char>(parameters.getRawParameterValue(lfo + ".AMOD.AMT")->load());
        s->frequencyModulationSource = static_cast<short>(parameters.getRawParameterValue(lfo + ".FMOD.SEL")->load()) - 1;
        s->frequencyModulationAmount = static_cast<char>(parameters.getRawParameterValue(lfo + ".FMOD.AMT")->load());

        s->phase = static_cast<unsigned char>(parameters.getRawParameterValue(lfo + ".PHASE")->load());
        s->smoothness = static_cast<unsigned char>(parameters.getRawParameterValue(lfo + ".SMTH")->load());

        // TODO figure out how to encode the modes (MODE.PLAY, MODE.AM, MODE.FM, MODE.DELAY)
//        s->playMode = static_cast<unsigned char>(parameters.getRawParameterValue(lfo + ".MODE.PLAY")->load());
        s->playMode = 0;
    }

    for (int i = 0; i < 4; i++) {
        auto env = "ENV" + std::to_string(i + 1);

        Env_settings* s = &synth.mSettings->envSettings[i];

        s->levels[0] = static_cast<char>(parameters.getRawParameterValue(env + ".L0")->load());
        s->levels[1] = static_cast<char>(parameters.getRawParameterValue(env + ".L1")->load());
        s->levels[2] = static_cast<char>(parameters.getRawParameterValue(env + ".L2")->load());
        s->levels[3] = static_cast<char>(parameters.getRawParameterValue(env + ".L3")->load());
        s->levels[4] = 0;
        s->levelModulation = static_cast<char>(parameters.getRawParameterValue(env + ".LV")->load());
        s->attackTimeModulation = static_cast<char>(parameters.getRawParameterValue(env + ".T1V")->load());

        s->times[0] = static_cast<char>(parameters.getRawParameterValue(env + ".T1")->load());
        s->times[1] = static_cast<char>(parameters.getRawParameterValue(env + ".T2")->load());
        s->times[2] = static_cast<char>(parameters.getRawParameterValue(env + ".T3")->load());
        s->times[3] = static_cast<char>(parameters.getRawParameterValue(env + ".T4")->load());

        s->timeScaling = static_cast<char>(parameters.getRawParameterValue(env + ".TK")->load());


        // TODO figure out how to encode options (CYC, SHAPE, MODE.T1V)
        s->options = 0;

        s->outputSmoothness = static_cast<char>(parameters.getRawParameterValue(env + ".SMTH")->load());
    }

    for (int i = 0; i < 3; i++) {
        auto mat = "MAT" + std::to_string(i + 1);

        ModulationMatrixSettings* s = &synth.mSettings->modulationMatrixSettings[i];
        s->modulationSource1 = static_cast<short>(parameters.getRawParameterValue(mat + ".MOD1.SEL")->load()) - 1;
        s->modulationAmount1 = static_cast<char>(parameters.getRawParameterValue(mat + ".MOD1.AMT")->load());
        s->modulationSource2 = static_cast<short>(parameters.getRawParameterValue(mat + ".MOD2.SEL")->load()) - 1;
        s->modulationAmount2 = static_cast<char>(parameters.getRawParameterValue(mat + ".MOD2.AMT")->load());
        s->modulationSource3 = static_cast<short>(parameters.getRawParameterValue(mat + ".MOD3.SEL")->load()) - 1;
        s->modulationAmount3 = static_cast<char>(parameters.getRawParameterValue(mat + ".MOD3.AMT")->load());
        s->amplitudeModulationSource = static_cast<short>(parameters.getRawParameterValue(mat + ".AMOD.SEL")->load()) - 1;
        s->amplitudeModulationAmount = static_cast<char>(parameters.getRawParameterValue(mat + ".AMOD.AMT")->load());
    }

    {
        synth.mSettings->pitchBendRange = static_cast<char>(parameters.getRawParameterValue("EMU.BRANGE")->load());
        synth.mSettings->pitchBendMode = static_cast<char>(parameters.getRawParameterValue("EMU.BMODE")->load());
        synth.mSettings->voiceStealingMode = static_cast<char>(parameters.getRawParameterValue("EMU.VSTEAL")->load()) - 1;
        synth.mSettings->muffle = static_cast<char>(parameters.getRawParameterValue("EMU.MUF")->load());

        // TODO figure out how to encode options (AMBUG, DCA13, DCA4, DCB)

        synth.mSettings->emuOptions = 1;
    }
}


#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ProgramLoader.h"
#include "Data.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
     synth(44100)
{
    settings = ProgramLoader::loadDefaultProgram();
    synth.mSettings = &settings;

    auto data = std::vector<uint8_t>(std::begin(programBankData), std::end(programBankData));

    presets = ProgramLoader::loadSysexProgramBank(data);
    presets[0] = presets[4];

    currentPreset = 0;
    settings = presets[0];
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

int AudioPluginAudioProcessor::getNumPrograms()
{
    return presets.size();
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return currentPreset;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    currentPreset = index;
    settings = presets[index];
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return juce::String(settings.presetName);
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
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
                std::cout << "note on " << message.getNoteNumber() << "\n";

                synth.triggerNote(message.getChannel(), message.getNoteNumber(), message.getVelocity());
            } else if (message.isNoteOff()) {
                std::cout << "note off " << message.getNoteNumber() << "\n";

                synth.triggerNote(message.getChannel(), message.getNoteNumber(), 0);
            }
        }

        float left = 0.0, right = 0.0;

        // TODO fix panning in synth
//        synth.mSettings->ampSettings.stereoPanning = 32;
//synth.mSettings->restartVoice = 1;

        synth.getSamples(true, &left, 1, &right);

        leftChannelData[i] = left;
        rightChannelData[i] = right;
    }
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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

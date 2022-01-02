#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), midiKeyboard(p.midiKeyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible (&midiKeyboard);

    midiKeyboard.setBounds(0, getHeight() - 90, getWidth(), 90);

    addAndMakeVisible(&programMenu);

    programMenu.setBounds(0, 0, 100, 20);

    for (int i = 0; i < processorRef.presets.size(); i++) {
        auto preset = processorRef.presets[i];
        programMenu.addItem(juce::String(preset.presetName), i + 1);
    }

    programMenu.setSelectedId (1);
    programMenu.onChange = [this]() {
        int id = programMenu.getSelectedId();
        processorRef.setCurrentProgram(id - 1);
    };
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    midiKeyboard.setBounds(0, getHeight() - 90, getWidth(), 90);
}

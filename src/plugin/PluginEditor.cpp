#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LcdScreen.h"
#include "LookAndFeel.h"
#include "ParameterHelper.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor    (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), midiKeyboard(p.midiKeyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
      lcdScreen(ParameterHelper::createPageLayout())
{
    juce::ignoreUnused (processorRef);


    for (auto & pageGroup : *lcdScreen.getPageLayout()) {
        for (auto & page : *pageGroup) {
            for (auto & param : *page) {
                param->setParent(&lcdScreen);
                auto attachment = LcdScreen::makeAttachment(processorRef.parameters, param->getId(), *param);
                if (attachment != nullptr) {
                    lcdParameterAttachments.add(std::move(attachment));
                }
            }
        }
    }

    processorRef.getProgramManager().addListener(this);


    setLookAndFeel(&lookAndFeel);

    setResizable(false, false);
    setSize (626, 430 + 100);

    addAndMakeVisible (&midiKeyboard);

    midiKeyboard.setBounds(0, getHeight() - 100, getWidth(), 100);

    addAndMakeVisible(&presetNameButton);
    presetNameButton.setBounds(140, 15, 150, 30);
    setPresetName();

    addAndMakeVisible(prevPresetButton);
    prevPresetButton.setBounds(300, 15, 20, 30);
    prevPresetButton.setButtonText("<");
    prevPresetButton.onClick = [this] {
        this->processorRef.getProgramManager().loadPreviousPreset();
    };

    addAndMakeVisible(nextPresetButton);
    nextPresetButton.setBounds(325, 15, 20, 30);
    nextPresetButton.setButtonText(">");
    nextPresetButton.onClick = [this] {
        this->processorRef.getProgramManager().loadNextPreset();
    };

    addAndMakeVisible(presetButton);
    presetButton.setButtonText("Menu");
    presetButton.setBounds(350, 15, 60, 30);
    presetButton.onClick = [this] {
        presetMenu.showMenu (juce::PopupMenu::Options().withMinimumWidth (100)
                                   .withMaximumNumColumns (3)
                                   .withTargetComponent (presetButton));
    };
    
    populatePresetMenu();

    auto logoImage = juce::ImageCache::getFromMemory(sq8l::data::sq8l, (int) sq8l::data::sq8lSize);
    logo.setImage(logoImage);
    logo.setBounds(0, 18, 120, 30);
    addAndMakeVisible(&logo);

    addAndMakeVisible(lcdScreen);
    lcdScreen.setBounds(38, 252, 550, 50);

    addAndMakeVisible(lcdButtonUp);
    lcdButtonUp.setName("lcdButtonUp");
    lcdButtonUp.setBounds(570, 252, 20, 18);
    lcdButtonUp.setDirection(LcdButton::Direction::up);
    lcdButtonUp.setOn(true);
    lcdButtonUp.onClick = [this] {
        this->lcdScreen.getPageGroup()->previousPage();
        this->lcdScreen.repaint();
        updateLcdButtons();
    };
    
    addAndMakeVisible(lcdButtonDown);
    lcdButtonUp.setName("lcdButtonDown");
    lcdButtonDown.setBounds(570, 270, 20, 18);
    lcdButtonDown.setDirection(LcdButton::Direction::down);
    lcdButtonDown.setOn(true);
    lcdButtonDown.onClick = [this] {
        this->lcdScreen.getPageGroup()->nextPage();
        this->lcdScreen.repaint();
        updateLcdButtons();
    };

    addAndMakeVisible(mixButton);
    LookAndFeel::setImages(mixButton, LookAndFeel::Color::green);
    mixButton.setBounds(28, 138, 44, 22);
    mixButton.onClick = [this] {
        this->showPageGroup(0);
    };

    addAndMakeVisible(osc1Button);
    LookAndFeel::setImages(osc1Button, LookAndFeel::Color::red);
    osc1Button.setBounds(158, 110, 44, 22);
    osc1Button.onClick = [this] {
        this->showPageGroup(1);
    };

    addAndMakeVisible(osc2Button);
    LookAndFeel::setImages(osc2Button, LookAndFeel::Color::red);
    osc2Button.setBounds(158, 138, 44, 22);
    osc2Button.onClick = [this] {
        this->showPageGroup(2); // TODO page
    };

    addAndMakeVisible(osc3Button);
    LookAndFeel::setImages(osc3Button, LookAndFeel::Color::red);
    osc3Button.setBounds(158, 166, 44, 22);
    osc3Button.onClick = [this] {
        this->showPageGroup(3); // TODO page
    };

    addAndMakeVisible(dca1Button);
    LookAndFeel::setImages(dca1Button, LookAndFeel::Color::grey);
    dca1Button.setBounds(277, 110, 44, 22);
    dca1Button.onClick = [this] {
        this->showPageGroup(1);
    };

    addAndMakeVisible(dca2Button);
    LookAndFeel::setImages(dca2Button, LookAndFeel::Color::grey);
    dca2Button.setBounds(277, 138, 44, 22);
    dca2Button.onClick = [this] {
        this->showPageGroup(2);
    };

    addAndMakeVisible(dca3Button);
    LookAndFeel::setImages(dca3Button, LookAndFeel::Color::grey);
    dca3Button.setBounds(277, 166, 44, 22);
    dca3Button.onClick = [this] {
        this->showPageGroup(3);
    };

    addAndMakeVisible(filterButton);
    LookAndFeel::setImages(filterButton, LookAndFeel::Color::grey);
    filterButton.setBounds(376, 138, 44, 22);
    filterButton.onClick = [this] {
        this->showPageGroup(4);
    };

    addAndMakeVisible(dca4Button);
    LookAndFeel::setImages(dca4Button, LookAndFeel::Color::grey);
    dca4Button.setBounds(445, 138, 44, 22);
    dca4Button.onClick = [this] {
        this->showPageGroup(5);
    };

    addAndMakeVisible(modesEmuButton);
    LookAndFeel::setImages(modesEmuButton, LookAndFeel::Color::green);
    modesEmuButton.setBounds(552, 160, 44, 22);
    modesEmuButton.onClick = [this] {
        this->showPageGroup(6);
    };


    addAndMakeVisible(syncButton);
    syncButton.setBounds(94, 126, 46, 18);
    buttonParameterAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.parameters, "MODE.SYNC", syncButton));

    addAndMakeVisible(amButton);
    amButton.setBounds(207, 126, 46, 18);
    buttonParameterAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.parameters, "MODE.AM", amButton));

    addAndMakeVisible(monoButton);
    monoButton.setBounds(545, 110, 46, 18);
    buttonParameterAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.parameters, "MODE.MONO", monoButton));


    for (int i = 0; i < 4; i++) {
        auto lfoButton = std::make_unique<juce::ImageButton>();
        addAndMakeVisible(*lfoButton);
        LookAndFeel::setImages(*lfoButton, LookAndFeel::Color::grey);
        lfoButton->setBounds(49 + i * 46, 370, 44, 22);
        lfoButton->onClick = [this, i] {
            this->showPageGroup(7 + i);
        };
        lfoButtons.add(std::move(lfoButton));
    }

    for (int i = 0; i < 4; i++) {
        auto envButton = std::make_unique<juce::ImageButton>();
        addAndMakeVisible(*envButton);
        LookAndFeel::setImages(*envButton, LookAndFeel::Color::grey);
        envButton->setBounds(243 + i * 46, 370, 44, 22);
        envButton->onClick = [this, i] {
            this->showPageGroup(11 + i);
        };
        envButtons.add(std::move(envButton));
    }

    for (int i = 0; i < 3; i++) {
        auto matButton = std::make_unique<juce::ImageButton>();
        addAndMakeVisible(*matButton);
        LookAndFeel::setImages(*matButton, LookAndFeel::Color::grey);
        matButton->setBounds(440 + i * 46, 370, 44, 22);
        matButton->onClick = [this, i] {
            this->showPageGroup(15 + i);
        };
        matButtons.add(std::move(matButton));
    }

    
    updateLcdButtons();
}

void AudioPluginAudioProcessorEditor::populatePresetMenu() {

    auto presets = processorRef.programManager.getPresets();


    pupulateSubMenu(presetMenu, "", presets);


    // presetMenu.addSu

    // presetMenu.addItem("init", [this] {  });

}

void AudioPluginAudioProcessorEditor::pupulateSubMenu(juce::PopupMenu& menu, juce::String directory, std::vector<juce::String> presets) {

    std::vector<juce::String> subdirectories;
    std::vector<juce::String> directoryPresets;

    for (auto p : presets) {
        if (p.startsWith(directory)) {
            auto rest = p.substring(directory.length());

            if (rest.contains("/")) {
                auto subdir = (directory.length() > 0 ? directory + "/" : "") + rest.substring(0, rest.indexOf("/"));



                if (std::find(subdirectories.begin(), subdirectories.end(), subdir) == subdirectories.end()) {
                    subdirectories.push_back(subdir);
                }
            } else {
                directoryPresets.push_back(p);
            }
        }
    }

    if (!subdirectories.empty()) {
        for (auto s : subdirectories) {
            juce::PopupMenu m;
            pupulateSubMenu(m, s + "/", presets);
            menu.addSubMenu(s, m, true);
        }
    } else {
        for (auto p : directoryPresets) {
            menu.addItem(p.substring(p.lastIndexOf("/") + 1), [this, p] {
                processorRef.getProgramManager().loadPreset(p);
            });
        }
    }
    
}


AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {
    setLookAndFeel(nullptr);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g) {
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    auto backgroundImage = juce::ImageCache::getFromMemory(sq8l::data::backgroundImage, (int) sq8l::data::backgroundImageSize);

    g.drawImage(backgroundImage, 0, 89, backgroundImage.getWidth(), backgroundImage.getHeight() - 89, 0, 89, backgroundImage.getWidth(), backgroundImage.getHeight() - 89, false);
    g.drawImage(backgroundImage, 0, 79, backgroundImage.getWidth(), 12, 0, 22, backgroundImage.getWidth(), 12, false);
    g.drawImage(backgroundImage, 0, 58, backgroundImage.getWidth(), 21, 0, 409, backgroundImage.getWidth(), 21, false);
}

void AudioPluginAudioProcessorEditor::showPageGroup(int i) {
    if (lcdScreen.getPageGroupIndex() == i) {
        lcdScreen.getPageGroup()->nextPage();
    } else {
        lcdScreen.setPageGroupIndex(i);
    }
    lcdScreen.repaint();
    updateLcdButtons();
}

void AudioPluginAudioProcessorEditor::updateLcdButtons() {
    lcdButtonUp.setOn(!lcdScreen.getPageGroup()->isFirstPage());
    lcdButtonDown.setOn(!lcdScreen.getPageGroup()->isLastPage());
}

void AudioPluginAudioProcessorEditor::onPresetLoad(ProgramManager* pm) {
    setPresetName();
}

void AudioPluginAudioProcessorEditor::setPresetName() {
    auto preset = processorRef.getProgramManager().getPresetName();
    auto presetName = preset.substring(preset.lastIndexOf("/")+1);

    presetNameButton.setButtonText(presetName);
}
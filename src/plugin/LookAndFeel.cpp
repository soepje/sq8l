
#include "../data/Data.h"
#include "LookAndFeel.h"

void LookAndFeel::drawToggleButton(juce::Graphics & g, juce::ToggleButton & toggleButton, bool shouldDrawButtonAsHighlighted,
                                   bool shouldDrawButtonAsDown) {

    auto ledImages = juce::ImageCache::getFromMemory(sq8l::data::toggleButtonLed, (int) sq8l::data::toggleButtonLedSize);
    juce::Image ledImage;
    if (toggleButton.getToggleState()) {
        ledImage = ledImages.getClippedImage(juce::Rectangle<int>(14, 0, 14, 14));
    } else {
        ledImage = ledImages.getClippedImage(juce::Rectangle<int>(0, 0, 14, 14));
    }

    auto toggleButtonImages = juce::ImageCache::getFromMemory(sq8l::data::toggleButton, (int) sq8l::data::toggleButtonSize);
    juce::Image toggleButtonImage;
    if (!shouldDrawButtonAsDown) {
        toggleButtonImage = toggleButtonImages.getClippedImage(juce::Rectangle<int>(0, 0, 30, 18));
    } else {
        toggleButtonImage = toggleButtonImages.getClippedImage(juce::Rectangle<int>(30, 0, 30, 18));
    }

    g.drawImage(ledImage, 0, 1, 14, 14, 0, 0, 14, 14, false);
    g.drawImage(toggleButtonImage, 16, 0, 30, 18, 0, 0, 30, 18, false);

}

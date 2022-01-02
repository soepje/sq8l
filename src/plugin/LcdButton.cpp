#include "../data/Data.h"
#include "LcdButton.h"

void LcdButton::paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
    auto lcdButtons = juce::ImageCache::getFromMemory(sq8l::data::lcdButtons, (int) sq8l::data::lcdButtonsSize);

    juce::Image image;

    if (isOn() && getDirection() == up) {
        image = lcdButtons.getClippedImage(juce::Rectangle<int>(20, 0, 20, 18));
    } else if (isOn() && getDirection() == down) {
        image = lcdButtons.getClippedImage(juce::Rectangle<int>(20, 18, 20, 18));
    } else if (!isOn() && getDirection() == up) {
        image = lcdButtons.getClippedImage(juce::Rectangle<int>(0, 0, 20, 18));
    } else if (!isOn() && getDirection() == down) {
        image = lcdButtons.getClippedImage(juce::Rectangle<int>(0, 18, 20, 18));
    }

    g.drawImage(image, 0, 0, 20, 18, 0, 0, 20, 18, false);

    if (shouldDrawButtonAsHighlighted) {
        g.setColour(juce::Colour::fromRGB(14, 124, 95));
        g.drawRect(0, 0, 20, 18);
    }
}

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>

class LcdButton : public juce::Button {
public:
    enum Direction {
        up,
        down
    };

private:
    Direction direction;
    bool on;

public:
    explicit LcdButton (const juce::String& name = juce::String())  : juce::Button(name) {
        direction = up;
        on = false;
    }

    void paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void setDirection(Direction d) {
        direction = d;
        repaint();
    }

    Direction getDirection() const {
        return direction;
    }

    void setOn(bool o) {
        on = o;
        repaint();
    }

    bool isOn() const {
        return on;
    };
};

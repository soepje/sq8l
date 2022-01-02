#ifndef SQ8L_LOOKANDFEEL_H
#define SQ8L_LOOKANDFEEL_H

#include "../data/Data.h"
#include <juce_gui_basics/juce_gui_basics.h>

class LookAndFeel : public juce::LookAndFeel_V4 {


    void drawToggleButton(juce::Graphics &, juce::ToggleButton &, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;


public:

    enum Color { green, red, grey };

    static void setImages(juce::ImageButton& button, Color color) {
        auto plainButtons = juce::ImageCache::getFromMemory(sq8l::data::buttons, (int) sq8l::data::buttonsSize);

        int y;
        switch (color) {
            case grey: y = 0; break;
            case red: y = 22; break;
            case green: y = 44; break;
        }

        auto upImage = plainButtons.getClippedImage(juce::Rectangle<int>(0, y, 44, 22));
        auto downImage = plainButtons.getClippedImage(juce::Rectangle<int>(44, y, 44, 22));

        button.setImages(false, false, false,
                         upImage, 1.0, juce::Colour::fromRGBA(0, 0, 0, 0),
                         upImage, 1.0, juce::Colour::fromRGBA(0, 0, 0, 0),
                         downImage, 1.0, juce::Colour::fromRGBA(0, 0, 0, 0));
    }

};


#endif //SQ8L_LOOKANDFEEL_H

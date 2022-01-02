#ifndef SQ8L_PARAMETERHELPER_H
#define SQ8L_PARAMETERHELPER_H

#include <juce_audio_processors/juce_audio_processors.h>
#include "LcdScreen.h"

template<typename... Args>
std::unique_ptr<juce::AudioParameterInt> makeParamInt(Args... args) {
    return std::make_unique<juce::AudioParameterInt>(std::forward<Args>(args)...);
}

template<typename... Args>
std::unique_ptr<juce::AudioParameterChoice> makeParamChoice(Args... args) {
    return std::make_unique<juce::AudioParameterChoice>(std::forward<Args>(args)...);
}

class ParameterHelper {
public:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    static std::unique_ptr<LcdScreen::PageLayout> createPageLayout();
};


#endif //SQ8L_PARAMETERHELPER_H

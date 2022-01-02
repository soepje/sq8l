#ifndef LCD_COMPONENT_H
#define LCD_COMPONENT_H

#include <string>
#include <utility>
#include <vector>
#include <functional>
#include "Data.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_audio_processors/juce_audio_processors.h>

// TODO add parent references to param, page, pageGroup to notify lcdscreen for redraws
// TODO add page layout container so configuration can be separated into ParameterHelper

class LcdScreen  : public juce::Component {

public:


    class Parameter {
    public:
        class Listener {
        public:
            virtual void parameterValueChanged (Parameter*)=0;
            virtual void parameterDragStarted(Parameter*)=0;
            virtual void parameterDragEnded(Parameter*)=0;

            virtual ~Listener() = default;
        };

    private:
        int row;
        int col;
        std::string id;
        std::string label;
        int valueWidth;
        int min, max;
        int value;
        int previousValue;
        bool selected;
        LcdScreen* parent = nullptr;

        juce::StringArray stringValues;

        juce::ListenerList<Parameter::Listener> listeners;

    public:
        Parameter(int _row, int _col, std::string _label, std::string _id, int _valueWidth, int _min, int _max);
        Parameter(int _row, int _col, std::string _label, std::string _id);


        void setParent(LcdScreen* p) {
            parent = p;
        }

        void addListener(Listener* l){ listeners.add(l); }
        void removeListener(Listener* l){ listeners.remove(l); }

        void setStringValues(juce::StringArray vs) {
            stringValues = std::move(vs);
        }

        void setMin(int m) {
            min = m;
        }

        void setMax(int m) {
            max = m;
        }

        void setValue(int v) {
            value = v;
            listeners.call([&] (Parameter::Listener& l) {  l.parameterValueChanged(this); });

            if (parent != nullptr) {
                parent->repaint();
            }
        }

        int getValue() const {
            return value;
        }

        void setPreviousValue(int v) {
            previousValue = v;
        }

        int getPreviousValue() const {
            return previousValue;
        }

        std::string getText();

        int getWidth() {
            return (int) label.size() + valueWidth;
        }

        int getCol() const {
            return col;
        }

        int getRow() const {
            return row;
        }

        bool canSelect() const {
            return valueWidth > 0;
        }

        bool isSelected() const {
            return selected;
        }

        void setSelected(bool _selected) {
            selected = _selected;
        }

        std::string getId() const {
            return id;
        }

        int getMin() const {
            return min;
        }

        int getMax() const {
            return max;
        }

        juce::NormalisableRange<float> getNormalisableRange() const {
            return juce::NormalisableRange<float>((float) min, (float) max, 1);
        }
    };


    class Page {
    private:
        juce::OwnedArray<Parameter> params;

    public:
        Page() = default;

        juce::OwnedArray<Parameter>& getParams() {
            return params;
        }

        Parameter** begin() noexcept {
            return params.begin();
        }

        Parameter** end() noexcept {
            return params.end();
        }

        void addParameter(std::unique_ptr<Parameter> p) {
            params.add(std::move(p));
        }

        class Builder {
            int row;
            int col;
            std::unique_ptr<Page> page;

        public:
            Builder();
            Builder& skip(int space);
            Builder& newline();
            Builder& param(std::string _label);
            Builder& param(std::string _label, int position, const std::string& _id, int _valueWidth);
            std::unique_ptr<Page> build();
        };
    };

    class PageGroup {
        juce::OwnedArray<Page> pages;
        int pageIndex;

    public:
        PageGroup() {
            pageIndex = 0;
        }

        Page** begin() noexcept {
            return pages.begin();
        }

        Page** end() noexcept {
            return pages.end();
        }

        void addPage(std::unique_ptr<Page> page) {
            pages.add(std::move(page));
        }

        Page* getPage() {
            return pages[pageIndex];
        }

        void previousPage() {
            pageIndex = (pageIndex + (int) pages.size() - 1) % (int) pages.size();
        }

        void nextPage() {
            pageIndex = (pageIndex + 1) % (int) pages.size();
        }

        bool isFirstPage() const {
            return pageIndex == 0;
        }

        bool isLastPage() const {
            return pageIndex == (int) pages.size() - 1;
        }
    };

    class LcdParameterAttachment : private LcdScreen::Parameter::Listener {
    public:
        LcdParameterAttachment (juce::RangedAudioParameter& parameter, LcdScreen::Parameter& lcdParam);
        ~LcdParameterAttachment();

        void sendInitialUpdate();

    private:
        void setValue (float newValue);

        void parameterValueChanged(LcdScreen::Parameter*) override;
        void parameterDragStarted(LcdScreen::Parameter*) override;
        void parameterDragEnded(LcdScreen::Parameter*) override;

        LcdScreen::Parameter& lcdParam;
        juce::ParameterAttachment attachment;
        bool ignoreCallbacks = false;
    };

    static std::unique_ptr<LcdScreen::LcdParameterAttachment> makeAttachment(juce::AudioProcessorValueTreeState& parameters, const juce::String& parameterID, LcdScreen::Parameter& param) {
        if (auto* parameter = parameters.getParameter (parameterID))
            return std::make_unique<LcdScreen::LcdParameterAttachment> (*parameter, param);

        return nullptr;
    }

    class PageLayout {
    private:
        friend LcdScreen;

        LcdScreen* parent;
        juce::OwnedArray<PageGroup> pageGroups;

        void setParent(LcdScreen* p) {
            parent = p;
        }

    public:
        void addPageGroup(std::unique_ptr<PageGroup> pageGroup) {
            pageGroups.add(std::move(pageGroup));
        }

        PageGroup** begin() noexcept {
            return pageGroups.begin();
        }

        PageGroup** end() noexcept {
            return pageGroups.end();
        }
    };


private:
    std::unique_ptr<PageLayout> pageLayout;

    int pageGroupIndex;

public:
    LcdScreen(std::unique_ptr<PageLayout> _pageLayout) {
        pageLayout = std::move(_pageLayout);
        pageLayout->setParent(this);

        pageGroupIndex = 0;
    }

    PageGroup* getPageGroup() {
        return pageLayout->pageGroups[pageGroupIndex];
    }

    PageLayout* getPageLayout() {
        return pageLayout.get();
    }

    int getPageGroupIndex() const {
        return pageGroupIndex;
    }

    void setPageGroupIndex(int i) {
        pageGroupIndex = i;
    }

    void paint(juce::Graphics &g) override;
    void mouseDown (const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
};

#endif

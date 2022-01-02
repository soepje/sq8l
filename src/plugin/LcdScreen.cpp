#include "LcdScreen.h"
#include "../data/Data.h"

std::string LcdScreen::Parameter::getText() {
    if (valueWidth == 0) {
        return label;
    }

    if (!stringValues.isEmpty()) {
        auto s = stringValues[value].toStdString();

        if (s.size() < (size_t) valueWidth) {
            int fill = std::max(valueWidth - (int) s.size(), 0);
            s = std::string((size_t) fill, ' ') + s;
        } else if (s.size() > (size_t) valueWidth) {
            s = s.substr(0, (size_t) valueWidth);
        }

        return label + s;
    } else {
        auto sign = min < 0;
        auto n = value;
        bool negative = n < 0;
        if (negative) {
            n = -n;
        }

        auto s = std::to_string(n);

        int fill = std::max(valueWidth - (int) s.size() - (sign ? 1 : 0), 0);
        s = std::string((size_t) fill, '0') + s;

        if (sign) {
            s = (negative ? "-" : "+") + s;
        }

        return label + s.substr(0, (size_t) valueWidth);
    }
}

static int mapChar(char c) {
    int pos  = 0;
    if (c >= 34 && c <= 96) {
        pos = c - 33;
    }
    return pos;
}

LcdScreen::Parameter::Parameter(int _row, int _col, std::string _label, std::string _id, int _valueWidth, int _min, int _max) {
    row = _row;
    col = _col;
    id = std::move(_id);
    label = std::move(_label);
    min = _min;
    max = _max;
    valueWidth = _valueWidth;

    value = 0;
    previousValue = 0;
    selected = false;
}

LcdScreen::Parameter::Parameter(int _row, int _col, std::string _label, std::string _id) : Parameter(_row, _col, std::move(_label), std::move(_id), 0, 0, 0) {

}

void LcdScreen::paint(juce::Graphics &g) {
    auto lcdScreenMap = juce::ImageCache::getFromMemory(sq8l::data::lcdImageMap, (int) sq8l::data::lcdImageMapSize);

    auto page = pageLayout->pageGroups[pageGroupIndex]->getPage();

    for (int i = 0; i < 44; i++) {
        for (int j = 0; j < 2; j++) {
            g.drawImage(lcdScreenMap, i * 12, j * 19, 11, 18, 0, 0, 11, 18, false);
        }
    }

    for (auto& param : page->getParams()) {
        auto text = param->getText();

        for(int i = 0; i < (int) text.size(); i++) {
            auto destX = (i + param->getCol()) * 12;
            auto destY = param->getRow() * 19;
            auto sourceY = mapChar(text[(size_t) i]) * 18;
            auto sourceX = param->isSelected() ? 11 : 0;

            g.drawImage(lcdScreenMap, destX, destY, 11, 18, sourceX, sourceY, 11, 18, false);
        }
    }
}

void LcdScreen::mouseDown(const juce::MouseEvent &event) {
    Component::mouseDown(event);

    auto page = pageLayout->pageGroups[pageGroupIndex]->getPage();
    auto& params = page->getParams();

    for (auto& param : params) {
        if (param->canSelect()) {
            auto x = param->getCol() * 12;
            auto y = param->getRow() * 19;

            auto width = param->getWidth() * 12;
            auto height = 19;

            auto mouseX = event.getMouseDownPosition().getX();
            auto mouseY = event.getMouseDownPosition().getY();

            if (mouseX >= x && mouseX < (x + width) && mouseY >= y && mouseY < (y + height)) {
                param->setSelected(true);
                param->setPreviousValue(param->getValue());

                for (auto &p : params) {
                    if (param->getId() != p->getId()) {
                        p->setSelected(false);
                    }
                }

                break;
            }
        }
    }

    repaint();
}

void LcdScreen::mouseDrag(const juce::MouseEvent &event) {
    Component::mouseDrag(event);

    auto page = pageLayout->pageGroups[pageGroupIndex]->getPage();
    auto& params = page->getParams();

    for (auto& param : params) {
        if (!param->canSelect()) {
            continue;
        }

        auto x = param->getCol() * 12;
        auto y = param->getRow() * 19;

        auto width = param->getWidth() * 12;
        auto height = 19;

        auto mouseX = event.getMouseDownX();
        auto mouseY = event.getMouseDownY();

        if (mouseX >= x && mouseX < (x + width) && mouseY >= y && mouseY < (y + height)) {
            auto steps = abs(param->getMin()) + abs(param->getMax()) + 1;
            auto stepsPerPixel = steps / 200.0;

            auto change = (int) (event.getDistanceFromDragStartY() * stepsPerPixel);

            auto newValue = std::min(std::max(param->getPreviousValue() - change, param->getMin()), param->getMax());

            if (param->getValue() != newValue) {
                param->setValue(newValue);
                repaint();
            }

            break;
        }
    }
}

LcdScreen::Page::Builder::Builder() {
    row = 0;
    col = 0;
    page = std::make_unique<Page>();
}

LcdScreen::Page::Builder& LcdScreen::Page::Builder::skip(int space) {
    col += space;
    return *this;
}

LcdScreen::Page::Builder& LcdScreen::Page::Builder::newline() {
    row++;
    col = 0;
    return *this;
}

LcdScreen::Page::Builder& LcdScreen::Page::Builder::param(std::string _label) {
    auto p = std::make_unique<Parameter>(row, col, std::move(_label), "", 0, 0, 0);
    col += p->getWidth();
    page->addParameter(std::move(p));
    return *this;
}

LcdScreen::Page::Builder& LcdScreen::Page::Builder::param(std::string _label, int position, const std::string& _id, int _valueWidth) {
    auto p = std::make_unique<Parameter>(row, col, std::move(_label), _id, _valueWidth, 0, 0);
    col += p->getWidth();
    page->addParameter(std::move(p));
    return *this;
}

std::unique_ptr<LcdScreen::Page> LcdScreen::Page::Builder::build() {
    return std::move(page);
}


LcdScreen::LcdParameterAttachment::LcdParameterAttachment(juce::RangedAudioParameter &param,
                                               LcdScreen::Parameter& lcdParam) : lcdParam(lcdParam),
                                                                                 attachment (param, [this] (float f) { setValue (f); }, nullptr) {


    lcdParam.setStringValues(param.getAllValueStrings());
    lcdParam.setMin((int) param.getNormalisableRange().getRange().getStart());
    lcdParam.setMax((int) param.getNormalisableRange().getRange().getEnd());

    lcdParam.addListener(this);
    sendInitialUpdate();
}

LcdScreen::LcdParameterAttachment::~LcdParameterAttachment() {
    lcdParam.removeListener(this);
}

void LcdScreen::LcdParameterAttachment::sendInitialUpdate() {
    attachment.sendInitialUpdate();
}

void LcdScreen::LcdParameterAttachment::setValue(float newValue) {
    const juce::ScopedValueSetter<bool> svs (ignoreCallbacks, true);

    lcdParam.setValue ((int) newValue);
}

void LcdScreen::LcdParameterAttachment::parameterValueChanged(LcdScreen::Parameter* p) {
    if (ignoreCallbacks) {
        return;
    }

    attachment.setValueAsPartOfGesture((float) p->getValue());
}

void LcdScreen::LcdParameterAttachment::parameterDragStarted(LcdScreen::Parameter *) {
    attachment.beginGesture();
}

void LcdScreen::LcdParameterAttachment::parameterDragEnded(LcdScreen::Parameter *) {
    attachment.endGesture();
}


#pragma once
#include <gd.h>
#include "CvoltonAlertLayerStub.h"

class ExtendedLevelInfo : public CvoltonAlertLayerStub {
    gd::GJGameLevel* level;
public:
    static ExtendedLevelInfo* create(gd::GJGameLevel* level);
    void onClose(cocos2d::CCObject* sender);
    static std::string getGameVersionName(int version);
    static std::string stringDate(std::string date);
    static const char* getDifficultyIcon(int stars);
    static std::string passwordString(int password);
    static std::string zeroIfNA(int value);
    static std::string workingTime(int value);
    static std::string printableProgress(std::string personalBests, int percentage);
    bool init();
    static cocos2d::CCLabelBMFont* createTextLabel(const std::string text, const cocos2d::CCPoint& position, const float scale, cocos2d::CCNode* menu, const char* font = "bigFont.fnt");
};
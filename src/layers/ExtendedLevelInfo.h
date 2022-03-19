#pragma once
#include <cocos2d.h>
#include <gd.h>

class ExtendedLevelInfo : public gd::FLAlertLayer {
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
    bool init();
    static cocos2d::CCLabelBMFont* createTextLabel(const std::string text, const cocos2d::CCPoint& position, const float scale, cocos2d::CCNode* menu, const char* font = "bigFont.fnt");
};
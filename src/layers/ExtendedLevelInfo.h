#pragma once
#include <gd.h>
#include "CvoltonAlertLayerStub.h"

class ExtendedLevelInfo : public CvoltonAlertLayerStub {
    std::string primary;
    std::string secondary;
    gd::GJGameLevel* level;
    gd::TextArea* info;
    gd::CCMenuItemSpriteExtra* prevBtn;
    gd::CCMenuItemSpriteExtra* nextBtn;
    int page = 0;
public:
    static ExtendedLevelInfo* create(gd::GJGameLevel* level);
    void onClose(cocos2d::CCObject* sender);
    void onCopyName(cocos2d::CCObject* sender);
    void onCopyAuthor(cocos2d::CCObject* sender);
    void onCopyDesc(cocos2d::CCObject* sender);
    void onNext(cocos2d::CCObject* sender);
    void onPrev(cocos2d::CCObject* sender);
    void loadPage(int page);
    static std::string getGameVersionName(int version);
    static std::string stringDate(std::string date);
    static const char* boolString(bool value);
    static const char* getDifficultyIcon(int stars);
    static std::string passwordString(int password);
    static std::string zeroIfNA(int value);
    static std::string workingTime(int value);
    static std::string printableProgress(std::string personalBests, int percentage);
    static void showProgressDialog(gd::GJGameLevel* level);
    bool init();
    static cocos2d::CCLabelBMFont* createTextLabel(const std::string text, const cocos2d::CCPoint& position, const float scale, cocos2d::CCNode* menu, const char* font = "bigFont.fnt");
};
#pragma once
#include <gd.h>
#include "CvoltonAlertLayerStub.h"

class LevelBrowserEndLayer : public CvoltonAlertLayerStub, public gd::OnlineListDelegate {
    gd::LevelBrowserLayer* levelBrowserLayer = nullptr;
    gd::TextArea* textLabel = nullptr;
    gd::CCMenuItemSpriteExtra* goBtn = nullptr;
    gd::LoadingCircle* circle = nullptr;

    int min = 0;
    int max = 0;
    int requests = 0;
public:
    static LevelBrowserEndLayer* create(gd::LevelBrowserLayer* levelBrowserLayer);
    void onClose(cocos2d::CCObject* sender);
    void onOK(cocos2d::CCObject* sender);
    bool init();
    static cocos2d::CCLabelBMFont* createTextLabel(const std::string text, const cocos2d::CCPoint& position, const float scale, cocos2d::CCNode* menu, const char* font = "bigFont.fnt");

    void loadListFinished(cocos2d::CCArray*, const char*);
    void loadListFailed(const char*);
    void setupPageInfo(std::string, const char*);

    void updateDisplay();
};
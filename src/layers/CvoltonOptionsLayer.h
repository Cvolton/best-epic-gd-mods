#pragma once
#include <gd.h>
#include "CvoltonAlertLayerStub.h"

class CvoltonOptionsLayer : public CvoltonAlertLayerStub {
    int toggleCount = 0;
public:
    static CvoltonOptionsLayer* create();
    void onClose(cocos2d::CCObject* sender);
    void onToggle(cocos2d::CCObject* sender);
    bool init();
    static cocos2d::CCLabelBMFont* createTextLabel(const std::string text, const cocos2d::CCPoint& position, const float scale, cocos2d::CCNode* menu, const char* font = "bigFont.fnt");
    void CvoltonOptionsLayer::createToggle(const char* option, const char* name);
};
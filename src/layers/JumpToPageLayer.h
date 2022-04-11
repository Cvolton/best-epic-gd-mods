#pragma once
#include <gd.h>
#include "CvoltonAlertLayerStub.h"
#include "DailyViewLayer.h"

class JumpToPageLayer : public CvoltonAlertLayerStub {
    gd::InfoLayer* infoLayer = nullptr;
    DailyViewLayer* dailyViewLayer = nullptr;
    gd::CCTextInputNode* textNode;
public:
    static JumpToPageLayer* create(gd::InfoLayer* infoLayer);
    static JumpToPageLayer* create(DailyViewLayer* dailyViewLayer);
    void onClose(cocos2d::CCObject* sender);
    void onOK(cocos2d::CCObject* sender);
    bool init();
    int pageNumber();
    static cocos2d::CCLabelBMFont* createTextLabel(const std::string text, const cocos2d::CCPoint& position, const float scale, cocos2d::CCNode* menu, const char* font = "bigFont.fnt");
};
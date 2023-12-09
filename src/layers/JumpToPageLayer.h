#pragma once
#include <gd.h>
#include "CvoltonAlertLayerStub.h"
#include "../delegates/PageNumberDelegate.h"

class JumpToPageLayer : public CvoltonAlertLayerStub {
    gd::InfoLayer* infoLayer = nullptr;
    PageNumberDelegate* pageNumberDelegate = nullptr;
    gd::CCTextInputNode* textNode;
public:
    static JumpToPageLayer* create(gd::InfoLayer* infoLayer);
    static JumpToPageLayer* create(PageNumberDelegate* pageNumberDelegate);
    void onClose(cocos2d::CCObject* sender);
    void onOK(cocos2d::CCObject* sender);
    void onPrev(cocos2d::CCObject* sender);
    void onNext(cocos2d::CCObject* sender);
    void onReset(cocos2d::CCObject* sender);
    bool init();
    int pageNumber();
    static cocos2d::CCLabelBMFont* createTextLabel(const std::string text, const cocos2d::CCPoint& position, const float scale, cocos2d::CCNode* menu, const char* font = "bigFont.fnt");
};
#pragma once
#include <gd.h>
#include "CvoltonAlertLayerStub.h"

class CvoltonUpdateLayer : public CvoltonAlertLayerStub {

    gd::LoadingCircle* circle;
    gd::TextArea* changelog;
    gd::TextArea* description;
    cocos2d::CCLabelBMFont* changelogFailText;
    cocos2d::CCLabelBMFont* title;
    
public:
    static CvoltonUpdateLayer* create();
    void onClose(cocos2d::CCObject* sender);
    void onUpdate(cocos2d::CCObject* sender);
    void onVisit(cocos2d::CCObject* sender);
    void onChangelog(cocos2d::CCObject* sender);
    void onLoadFailed();
    void onLoadFinished();
    void showVersion();
    void downloadChangelog();
    bool init();
    static cocos2d::CCLabelBMFont* createTextLabel(const std::string text, const cocos2d::CCPoint& position, const float scale, cocos2d::CCNode* menu, const char* font = "bigFont.fnt");
};
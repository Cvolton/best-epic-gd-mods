#pragma once
#include <gd.h>
#include "CvoltonOptionsLayer.h"

class CvoltonSearchOptions : public CvoltonOptionsLayer {
public:
    static CvoltonSearchOptions* create();
    void onClose(cocos2d::CCObject* sender);
    void onPrev(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    bool init();
    void destroyToggles();
    void drawToggles();
    void CvoltonSearchOptions::createToggle(const char* option, const char* name, float x, float y);
    void CvoltonSearchOptions::onCompletedPrev(cocos2d::CCObject* sender);
    void CvoltonSearchOptions::onCompletedNext(cocos2d::CCObject* sender);
    static std::string CvoltonSearchOptions::getCompletedString();
};
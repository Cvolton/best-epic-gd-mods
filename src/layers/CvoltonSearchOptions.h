#pragma once
#include <gd.h>
#include "CvoltonOptionsLayer.h"
#include "../delegates/IDRangeDelegate.h"

class CvoltonSearchOptions : public CvoltonOptionsLayer, public IDRangeDelegate {
public:
    static CvoltonSearchOptions* create();
    void onClose(cocos2d::CCObject* sender);
    void onPrev(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    bool init();
    void destroyToggles();
    void drawToggles();
    void createToggle(const char* option, const char* name, float x, float y);
    void onCompletedPrev(cocos2d::CCObject* sender);
    void onCompletedNext(cocos2d::CCObject* sender);
    void onPercentageRange(cocos2d::CCObject* sender);
    static std::string getCompletedString();

    void onIDRangeFinished(int min, int max, int additional);
};
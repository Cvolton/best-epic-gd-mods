#pragma once
#include <gd.h>
#include "CvoltonOptionsLayer.h"

class ProfileSearchOptions : public CvoltonOptionsLayer {
public:
    static ProfileSearchOptions* create();
    void onClose(cocos2d::CCObject* sender);
    void onPrev(cocos2d::CCObject* sender);
    bool init();
    void destroyToggles();
    void drawToggles();
    void ProfileSearchOptions::createToggle(const char* option, const char* name, float x, float y);
    void ProfileSearchOptions::createButtonToggle(const char* option, cocos2d::CCNode* sprite, float x, float y, float scale = 1.f);
};
#pragma once
#include <gd.h>
#include "CvoltonOptionsLayer.h"

class CvoltonSearchOptions : public CvoltonOptionsLayer {
    int toggleCount = 0;
public:
    static CvoltonSearchOptions* create();
    void onClose(cocos2d::CCObject* sender);
    void onPrev(cocos2d::CCObject* sender);
    bool init();
    void destroyToggles();
    void drawToggles();
    void CvoltonSearchOptions::createToggle(const char* option, const char* name, float x, float y);
};
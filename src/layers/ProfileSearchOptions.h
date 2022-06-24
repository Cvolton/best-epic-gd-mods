#pragma once
#include <gd.h>
#include "CvoltonOptionsLayer.h"
#include "../delegates/DialogCloseDelegate.h"

class ProfileSearchOptions : public CvoltonOptionsLayer, public DialogCloseDelegate {
    gd::LevelBrowserLayer* levelBrowserLayer = nullptr;
public:
    static ProfileSearchOptions* create(gd::LevelBrowserLayer* levelBrowserLayer = nullptr);
    void onClose(cocos2d::CCObject* sender);
    void onPrev(cocos2d::CCObject* sender);
    void onSong(cocos2d::CCObject* sender);
    bool init();
    void destroyToggles();
    void drawToggles();
    void ProfileSearchOptions::createToggle(const char* option, const char* name, float x, float y);
    void ProfileSearchOptions::createButtonToggle(const char* option, cocos2d::CCNode* sprite, float x, float y, float scale = 1.f);
    void onDialogClosed();
};
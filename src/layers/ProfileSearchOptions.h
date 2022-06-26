#pragma once
#include <gd.h>
#include "CvoltonOptionsLayer.h"
#include "../delegates/DialogCloseDelegate.h"

class ProfileSearchOptions : public CvoltonOptionsLayer, public DialogCloseDelegate {
    gd::LevelBrowserLayer* levelBrowserLayer = nullptr;
    gd::CCMenuItemSpriteExtra* prevBtn = nullptr;
    gd::CCMenuItemSpriteExtra* nextBtn = nullptr;
    cocos2d::extension::CCScale9Sprite* lengthBg = nullptr;
    cocos2d::extension::CCScale9Sprite* diffBg = nullptr;
    cocos2d::extension::CCScale9Sprite* demonDiffBg = nullptr;
    int page = 0;
public:
    static ProfileSearchOptions* create(gd::LevelBrowserLayer* levelBrowserLayer = nullptr);
    void onClose(cocos2d::CCObject* sender);
    void onPrev(cocos2d::CCObject* sender);
    void onSong(cocos2d::CCObject* sender);
    void onNext(cocos2d::CCObject* sender);
    bool init();
    void destroyToggles();
    void drawToggles();
    void drawTogglesPrimary();
    void drawTogglesSecondary();
    void ProfileSearchOptions::createToggle(const char* option, const char* name, float x, float y);
    void ProfileSearchOptions::createButtonToggle(const char* option, cocos2d::CCNode* sprite, float x, float y, float scale = 1.f);
    void onDialogClosed();
};
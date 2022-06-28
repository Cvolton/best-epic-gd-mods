#pragma once
#include <gd.h>
#include "CvoltonOptionsLayer.h"
#include "../delegates/DialogCloseDelegate.h"
#include "../delegates/IDRangeDelegate.h"

class ProfileSearchOptions : public CvoltonOptionsLayer, public DialogCloseDelegate, public IDRangeDelegate {
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
    void onIdRange(cocos2d::CCObject* sender);
    void onNext(cocos2d::CCObject* sender);
    void onSecondaryInfo(cocos2d::CCObject* sender);
    bool init();
    void destroyToggles();
    void drawToggles();
    void drawTogglesPrimary();
    void drawTogglesSecondary();
    void createToggle(const char* option, const char* name, float x, float y);
    void onDialogClosed();
    void onIDRangeFinished(int min, int max);
};
#pragma once
#include <gd.h>
#include "CvoltonOptionsLayer.h"
#include "../delegates/DialogCloseDelegate.h"
#include "../delegates/IDRangeDelegate.h"
#include "../objects/BISearchObject.h"

class ProfileSearchOptions : public CvoltonOptionsLayer, public DialogCloseDelegate, public IDRangeDelegate {
    std::map<std::string, bool> options;
    std::map<std::string, int> optionInts;
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
    void createToggle(const char* option, const char* name, float x, float y, cocos2d::SEL_MenuHandler additional);
    void onDialogClosed();
    void onIDRangeFinished(int min, int max);
    bool getOption(const std::string& option);
    int getOptionInt(const std::string& option);
    bool toggleOption(const std::string& option);
    void onToggle(cocos2d::CCObject* sender);
    void setOption(const std::string& option, bool value);
    void setOptionInt(const std::string& option, int value);
    BISearchObject getSearchObject();
    void setSearchObject(const BISearchObject& searchObj);
};
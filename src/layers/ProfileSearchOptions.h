#pragma once
#include <gd.h>
#include "CvoltonOptionsLayer.h"
#include "../delegates/DialogCloseDelegate.h"
#include "../delegates/IDRangeDelegate.h"
#include "../delegates/BISearchObjectDelegate.h"
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
    BISearchObjectDelegate* searchObjDelegate = nullptr;
    std::string prefix;
    int page = 0;
public:
    static ProfileSearchOptions* create(gd::LevelBrowserLayer* levelBrowserLayer = nullptr, const std::string& prefix = "", BISearchObjectDelegate* searchObjDelegate = nullptr);
    void onClose(cocos2d::CCObject* sender);
    void onPrev(cocos2d::CCObject* sender);
    void onSong(cocos2d::CCObject* sender);
    void onIdRange(cocos2d::CCObject* sender);
    void onStarRange(cocos2d::CCObject* sender);
    void onNext(cocos2d::CCObject* sender);
    void onSecondaryInfo(cocos2d::CCObject* sender);
    bool init(gd::LevelBrowserLayer* levelBrowserLayer = nullptr, const std::string& prefix = "", BISearchObjectDelegate* searchObjDelegate = nullptr);
    void destroyToggles();
    void drawToggles();
    void drawTogglesPrimary();
    void drawTogglesSecondary();
    void drawTogglesTerciary();
    void createToggle(const char* option, const char* name, float x, float y);
    void createToggle(const char* option, const char* name, float x, float y, cocos2d::SEL_MenuHandler additional);
    void onDialogClosed();
    void onIDRangeFinished(int min, int max, int additional);
    bool getOption(const std::string& option);
    int getOptionInt(const std::string& option);
    bool toggleOption(const std::string& option);
    void setOption(const std::string& option, bool value);
    void setOptionInt(const std::string& option, int value);
    BISearchObject getSearchObject();
    void setSearchObject(const BISearchObject& searchObj);
};
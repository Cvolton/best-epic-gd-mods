#pragma once

#include <gd.h>
#include <deque>
#include "LevelSearchListView.h"
#include "../delegates/BISearchObjectDelegate.h"
#include "../objects/BISearchObject.h"

class LevelSearchViewLayer : public cocos2d::CCLayer, public gd::OnlineListDelegate, public BISearchObjectDelegate {
    BISearchObject m_searchObj;
    gd::GJSearchObject* m_gjSearchObj = nullptr;
    gd::GJSearchObject* m_gjSearchObjOptimized = nullptr;
    gd::GJSearchObject* m_gjSearchObjLoaded = nullptr;
    LevelSearchListView* m_listView = nullptr;
    gd::GJListLayer* m_listLayer = nullptr;
    gd::CCMenuItemSpriteExtra* m_prevBtn = nullptr;
    gd::CCMenuItemSpriteExtra* m_nextBtn = nullptr;
    cocos2d::CCLabelBMFont* m_statusText = nullptr;
    cocos2d::CCLabelBMFont* m_counter = nullptr;
    cocos2d::CCArray* m_loadedLevels = nullptr;
    std::deque<gd::GJGameLevel*> m_unloadedLevels;
    std::deque<gd::GJGameLevel*> m_allLevels;
    size_t m_page = 0;
    size_t m_shownLevels = 0;
    size_t m_firstIndex = 0;
    size_t m_lastIndex = 0;
    size_t m_totalAmount = 0;
protected:
    virtual bool init(std::deque<gd::GJGameLevel*> allLevels, BISearchObject searchObj = BISearchObject());
    virtual bool init(gd::GJSearchObject* gjSearchObj, BISearchObject searchObj = BISearchObject());
    virtual bool init(BISearchObject searchObj = BISearchObject());
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
    void unload();
    void reload();
    void startLoading();
    void setTextStatus(bool finished);
public:
    void loadPage(bool reload);
    static LevelSearchViewLayer* create(std::deque<gd::GJGameLevel*> allLevels, BISearchObject searchObj = BISearchObject());
    static LevelSearchViewLayer* create(gd::GJSearchObject* gjSearchObj, BISearchObject searchObj = BISearchObject());
    static cocos2d::CCScene* scene(std::deque<gd::GJGameLevel*> allLevels, BISearchObject searchObj = BISearchObject());
    static cocos2d::CCScene* scene(gd::GJSearchObject* gjSearchObj, BISearchObject searchObj = BISearchObject());
    
    void onPrev(cocos2d::CCObject*);
    void onNext(cocos2d::CCObject*);
    void onFilters(cocos2d::CCObject*);
    void keyDown(cocos2d::enumKeyCodes key) override;

    void loadListFinished(cocos2d::CCArray*, const char*);
    void loadListFailed(const char*);
    void setupPageInfo(std::string, const char*);

    void onSearchObjectFinished(const BISearchObject& searchObj);
    void showInfoDialog();

    void optimizeSearchObject();
    void resetUnloadedLevels();

    void queueLoad(float dt);
};
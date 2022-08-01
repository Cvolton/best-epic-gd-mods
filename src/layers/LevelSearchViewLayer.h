#pragma once

#include <gd.h>
#include <deque>
#include "LevelSearchListView.h"

class LevelSearchViewLayer : public cocos2d::CCLayer, public gd::OnlineListDelegate {
    LevelSearchListView* m_listView = nullptr;
    gd::GJListLayer* m_listLayer = nullptr;
    cocos2d::CCArray* m_loadedLevels = nullptr;
    std::deque<gd::GJGameLevel*> m_allLevels;
protected:
    virtual bool init();
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
    void startLoading();
public:
    void loadPage(int page);
    static LevelSearchViewLayer* create();
    static cocos2d::CCScene* scene();

    void loadListFinished(cocos2d::CCArray*, const char*);
    void loadListFailed(const char*);
    void setupPageInfo(std::string, const char*);
};
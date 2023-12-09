#pragma once

#include <gd.h>
#include "../delegates/BILeaderboardDelegate.h"

class LeaderboardViewLayer : public cocos2d::CCLayer, public BILeaderboardDelegate {
    gd::CustomListView* m_leaderboardView = nullptr;
    gd::GJListLayer* m_listLayer = nullptr;
    cocos2d::CCArray* m_scores = nullptr;
    gd::LoadingCircle* m_circle = nullptr;
    int m_accountID = 0;
protected:
    virtual bool init(int accountID);
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
    void onRefresh(cocos2d::CCObject*);
public:
    void loadPage();
    static LeaderboardViewLayer* create(int accountID);
    static cocos2d::CCScene* scene(int accountID);

    void onLeaderboardFinished(cocos2d::CCArray* scores);
};
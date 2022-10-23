#pragma once

#include <gd.h>
#include "LeaderboardListView.h"
#include "../delegates/BILeaderboardDelegate.h"

class LeaderboardViewLayer : public cocos2d::CCLayer, public BILeaderboardDelegate {
    LeaderboardListView* m_leaderboardView = nullptr;
    gd::GJListLayer* m_listLayer = nullptr;
    cocos2d::CCArray* m_scores = nullptr;
    int m_accountID = 0;
protected:
    virtual bool init(int accountID);
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
public:
    void loadPage();
    static LeaderboardViewLayer* create(int accountID);
    static cocos2d::CCScene* scene(int accountID);

    void onLeaderboardFinished(cocos2d::CCArray* scores);
};
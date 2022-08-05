#pragma once

#include <gd.h>
#include "LeaderboardListView.h"

class LeaderboardViewLayer : public cocos2d::CCLayer {
    LeaderboardListView* leaderboardView = nullptr;
    gd::GJListLayer* listLayer = nullptr;
    cocos2d::CCArray* scores = nullptr;
    int accountID = 0;
protected:
    virtual bool init(int accountID);
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
    void startLoading();
    void onLoadFinished(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
    void generateScores(const std::string& response);
public:
    void loadPage();
    static LeaderboardViewLayer* create(int accountID);
    static cocos2d::CCScene* scene(int accountID);
};
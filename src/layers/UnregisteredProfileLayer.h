#pragma once
#include <gd.h>
#include "CvoltonAlertLayerStub.h"


class UnregisteredProfileLayer : public CvoltonAlertLayerStub {
    gd::GJUserScore* score;
    cocos2d::CCNode* invoker;
public:
    static UnregisteredProfileLayer* create(gd::GJUserScore* score, cocos2d::CCNode* invoker = nullptr);
    void onClose(cocos2d::CCObject* sender);
    static void displayProfile(int userID, std::string userName, cocos2d::CCNode* invoker = nullptr);
    static void doSearch(int userID);
    void onMyLevels(CCObject* sender);
    void onCommentHistory(CCObject* sender);
    bool init();
};
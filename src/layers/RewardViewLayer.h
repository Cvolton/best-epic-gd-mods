#pragma once

#include <gd.h>
#include "RewardListView.h"

class RewardViewLayer : public cocos2d::CCLayer {
    RewardListView* rewardView = nullptr;
    gd::GJListLayer* listLayer = nullptr;
    cocos2d::CCArray* sortedRewards = nullptr;
    gd::CCMenuItemSpriteExtra* prevBtn = nullptr;
    gd::CCMenuItemSpriteExtra* nextBtn = nullptr;
    cocos2d::CCLabelBMFont* counter = nullptr;
    gd::ButtonSprite* pageBtnSprite = nullptr;
    unsigned int page = 0;
protected:
    virtual bool init();
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
    void onPrev(cocos2d::CCObject*);
    void onNext(cocos2d::CCObject*);
    void onJumpToPageLayer(cocos2d::CCObject*);
    void onRandom(cocos2d::CCObject*);
    int rewardsPerPage() const;
public:
    void loadPage(unsigned int page);
    static RewardViewLayer* create();
    static bool compareRewards(const void* l1, const void* l2);
    static cocos2d::CCScene* scene();
    int getPage() const;
    void keyDown(cocos2d::enumKeyCodes key) override;
};
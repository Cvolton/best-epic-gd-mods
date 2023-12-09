#pragma once

#include <gd.h>
#include "RewardListView.h"
#include "../delegates/PageNumberDelegate.h"

class RewardViewLayer : public cocos2d::CCLayer, public PageNumberDelegate {
    RewardListView* rewardView = nullptr;
    gd::GJListLayer* listLayer = nullptr;
    cocos2d::CCArray* sortedRewards = nullptr;
    gd::CCMenuItemSpriteExtra* prevBtn = nullptr;
    gd::CCMenuItemSpriteExtra* nextBtn = nullptr;
    cocos2d::CCLabelBMFont* counter = nullptr;
    gd::ButtonSprite* pageBtnSprite = nullptr;
    const char* title = nullptr;
    unsigned int page = 0;
protected:
    virtual bool init(cocos2d::CCDictionary* chests, const char* title);
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
    void onPrev(cocos2d::CCObject*);
    void onNext(cocos2d::CCObject*);
    void onJumpToPageLayer(cocos2d::CCObject*);
    void onRandom(cocos2d::CCObject*);
    int rewardsPerPage() const;
public:
    void loadPage(unsigned int page);
    static RewardViewLayer* create(cocos2d::CCDictionary* chests, const char* title);
    static bool compareRewards(const void* l1, const void* l2);
    static cocos2d::CCScene* scene(cocos2d::CCDictionary* chests, const char* title);
    int getPage() const;
    void keyDown(cocos2d::enumKeyCodes key) override;
};
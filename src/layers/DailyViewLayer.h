#pragma once

#include <gd.h>
#include "DailyListView.h"

class DailyViewLayer : public cocos2d::CCLayerColor {
    DailyListView* dailyView = nullptr;
    gd::GJListLayer* listLayer = nullptr;
    cocos2d::CCArray* sortedLevels = nullptr;
    gd::CCMenuItemSpriteExtra* prevBtn = nullptr;
    gd::CCMenuItemSpriteExtra* nextBtn = nullptr;
    cocos2d::CCLabelBMFont* counter = nullptr;
    unsigned int page = 0;
    bool isWeekly = false;
protected:
    virtual bool init(bool isWeekly);
    virtual void keyBackClicked();
    void onBack(cocos2d::CCObject*);
    void onPrev(cocos2d::CCObject*);
    void onNext(cocos2d::CCObject*);
    void loadPage(unsigned int page);
public:
    static DailyViewLayer* create(bool isWeekly);
    static bool compareDailies(const void* l1, const void* l2);
};
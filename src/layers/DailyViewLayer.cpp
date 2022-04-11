#include "DailyViewLayer.h"
#include "DailyListView.h"
#include "../managers/CvoltonManager.h"

using namespace gd;
using namespace cocos2d;

DailyViewLayer* DailyViewLayer::create(bool isWeekly) {
    auto ret = new DailyViewLayer();
    if (ret && ret->init(isWeekly)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool DailyViewLayer::compareDailies(const void* l1, const void* l2){
    const GJGameLevel* level1 = reinterpret_cast<const GJGameLevel*>(l1);
    const GJGameLevel* level2 = reinterpret_cast<const GJGameLevel*>(l2);
    return level1->dailyID < level2->dailyID;
}

bool DailyViewLayer::init(bool isWeekly) {

    initWithColor({0, 0, 0, 150});
    this->isWeekly = isWeekly;

    auto GLM = gd::GameLevelManager::sharedState();
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    auto backBtn = gd::CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(DailyViewLayer::onBack)
    );

    auto menuBack = CCMenu::create();
    menuBack->addChild(backBtn);
    menuBack->setPosition({25, winSize.height - 25});

    addChild(menuBack);

    setKeypadEnabled(true);

    auto dailyLevels = GLM->m_dailyLevels;
    sortedLevels = CCArray::create();
    sortedLevels->retain();
    CCDictElement* obj;
    CCDICT_FOREACH(dailyLevels, obj){
        auto currentLvl = static_cast<GJGameLevel*>(obj->getObject());
        if(
            currentLvl != nullptr &&
            ((isWeekly && currentLvl->dailyID >= 100000) || (!isWeekly && currentLvl->dailyID < 100000))
        ){
            sortedLevels->addObject(currentLvl);
        }
    }
    std::sort(sortedLevels->data->arr, sortedLevels->data->arr + sortedLevels->data->num, DailyViewLayer::compareDailies);

    loadPage(0);

    auto menu = CCMenu::create();
    addChild(menu);

    auto prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    prevBtn = gd::CCMenuItemSpriteExtra::create(
        prevSprite,
        this,
        menu_selector(DailyViewLayer::onPrev)
    );
    prevBtn->setPosition({- (winSize.width / 2) + 25, 0});
    menu->addChild(prevBtn);

    auto nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    nextSprite->setFlipX(true);
    nextBtn = gd::CCMenuItemSpriteExtra::create(
        nextSprite,
        this,
        menu_selector(DailyViewLayer::onNext)
    );
    nextBtn->setPosition({+ (winSize.width / 2) - 25, 0});
    menu->addChild(nextBtn);

    return true;
}

void DailyViewLayer::loadPage(unsigned int page){

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if(listLayer != nullptr) listLayer->removeFromParentAndCleanup(true);

    this->page = page;
    CCArray* displayedLevels = CCArray::create();
    //TODO: can we clone this by passing an iterator or something like that
    //TODO: read the game variable that tells it if we want 10 or 20 lvls
    const unsigned int levelCount = 10;
    unsigned int lastIndex = (page+1) * levelCount;

    for(unsigned int i = page * levelCount; i < lastIndex; i++){
        auto levelObject = sortedLevels->objectAtIndex(i);
        if(levelObject == nullptr) break;

        displayedLevels->addObject(levelObject);
    }

    //TODO: crash when array is empty
    dailyView = DailyListView::create(displayedLevels, 356.f, 220.f);
    listLayer = GJListLayer::create(dailyView, "Daily Levels", {255, 255, 255, 255}, 356.f, 220.f);
    listLayer->setPosition(winSize / 2 - listLayer->getScaledContentSize() / 2);
    addChild(listLayer);
}

void DailyViewLayer::keyBackClicked() {
    sortedLevels->release();
    removeFromParentAndCleanup(true);
}


void DailyViewLayer::onBack(CCObject* object) {
    keyBackClicked();
}

void DailyViewLayer::onPrev(CCObject* object) {
    loadPage(--page);
}

void DailyViewLayer::onNext(CCObject* object) {
    loadPage(++page);
}
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

    initWithColor({0, 0, 0, 0x4B});
    this->isWeekly = isWeekly;

    m_pLayer = cocos2d::CCLayer::create();
    this->addChild(m_pLayer);

    auto GLM = gd::GameLevelManager::sharedState();
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_pButtonMenu = CCMenu::create();
    m_pLayer->addChild(m_pButtonMenu);
    
    auto backBtn = gd::CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(DailyViewLayer::onBack)
    );

    backBtn->setPosition({(-winSize.width / 2) + 25, (winSize.height / 2) - 25});
    m_pButtonMenu->addChild(backBtn);

    /*auto menuBack = CCMenu::create();
    menuBack->addChild(backBtn);
    menuBack->setPosition({25, winSize.height - 25});

    addChild(menuBack);*/

    setTouchEnabled(true);
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

    auto prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    prevBtn = gd::CCMenuItemSpriteExtra::create(
        prevSprite,
        this,
        menu_selector(DailyViewLayer::onPrev)
    );
    prevBtn->setPosition({- (winSize.width / 2) + 25, 0});
    m_pButtonMenu->addChild(prevBtn);

    auto nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    nextSprite->setFlipX(true);
    nextBtn = gd::CCMenuItemSpriteExtra::create(
        nextSprite,
        this,
        menu_selector(DailyViewLayer::onNext)
    );
    nextBtn->setPosition({+ (winSize.width / 2) - 25, 0});
    m_pButtonMenu->addChild(nextBtn);

    counter = CCLabelBMFont::create("0 to 0 of 0", "goldFont.fnt");
    counter->setAnchorPoint({ 1.f, 1.f });
    counter->setPosition(winSize - CCPoint(7,3));
    counter->setScale(0.5f);
    m_pLayer->addChild(counter);

    loadPage(0);
    return true;
}

void DailyViewLayer::loadPage(unsigned int page){

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if(listLayer != nullptr) listLayer->removeFromParentAndCleanup(true);

    this->page = page;
    CCArray* displayedLevels = CCArray::create();
    //TODO: can we clone this by passing an iterator or something like that
    const unsigned int levelCount = (gd::GameManager::sharedState()->getGameVariable("0093")) ? 20 : 10;
    unsigned int firstIndex = page * levelCount;
    unsigned int lastIndex = (page+1) * levelCount;

    for(unsigned int i = firstIndex; i < lastIndex; i++){
        auto levelObject = sortedLevels->objectAtIndex(i);
        if(i >= sortedLevels->count() || levelObject == nullptr) break;

        displayedLevels->addObject(levelObject);
    }

    dailyView = DailyListView::create(displayedLevels, 356.f, 220.f);
    listLayer = GJListLayer::create(dailyView, isWeekly ? "Weekly Demons" : "Daily Levels", {191, 114, 62, 255}, 356.f, 220.f);
    listLayer->setPosition(winSize / 2 - listLayer->getScaledContentSize() / 2 - CCPoint(0,5));
    m_pLayer->addChild(listLayer);

    if(page == 0) prevBtn->setVisible(false);
    else prevBtn->setVisible(true);

    if(sortedLevels->count() >= lastIndex) nextBtn->setVisible(true);
    else nextBtn->setVisible(false);

    counter->setCString(CCString::createWithFormat("%i to %i of %i", firstIndex+1, (sortedLevels->count() >= lastIndex) ? lastIndex : sortedLevels->count(), sortedLevels->count())->getCString());
}

void DailyViewLayer::keyBackClicked() {
    setTouchEnabled(false);
    setKeypadEnabled(false);
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
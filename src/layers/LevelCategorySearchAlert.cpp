#include "LevelCategorySearchAlert.h"
#include "LevelSearchViewLayer.h"
#include "../utils.hpp"

using namespace cocos2d;
using namespace gd;

LevelCategorySearchAlert* LevelCategorySearchAlert::create(){
    auto ret = new LevelCategorySearchAlert();
    if (ret && ret->init()) {
        //robert 1 :D
        ret->autorelease();
    } else {
        //robert -1
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void LevelCategorySearchAlert::onClose(cocos2d::CCObject* sender)
{
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void LevelCategorySearchAlert::onPlayed(CCObject* sender) {
    auto browserLayer = LevelSearchViewLayer::scene(BetterInfo::completedDeque());
    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
    CCDirector::sharedDirector()->pushScene(transitionFade);
}

void LevelCategorySearchAlert::onPercentage(cocos2d::CCObject* sender) {
    //TODO
}

void LevelCategorySearchAlert::onCompleted(cocos2d::CCObject* sender) {
    BISearchObject searchObj;
    searchObj.completed = true;

    auto browserLayer = LevelSearchViewLayer::scene(BetterInfo::completedDeque(), searchObj);
    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
    CCDirector::sharedDirector()->pushScene(transitionFade);
}

void LevelCategorySearchAlert::onOrbs(cocos2d::CCObject* sender) {
    BISearchObject searchObj;
    searchObj.completedOrbs = true;

    auto browserLayer = LevelSearchViewLayer::scene(BetterInfo::completedDeque(), searchObj);
    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
    CCDirector::sharedDirector()->pushScene(transitionFade);
}
void LevelCategorySearchAlert::onLeaderboard(cocos2d::CCObject* sender) {
    BISearchObject searchObj;
    searchObj.completedLeaderboard = true;

    auto browserLayer = LevelSearchViewLayer::scene(BetterInfo::completedDeque(), searchObj);
    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
    CCDirector::sharedDirector()->pushScene(transitionFade);
}

void LevelCategorySearchAlert::onCoins(cocos2d::CCObject* sender) {
    //TODO
}

void LevelCategorySearchAlert::onNoCoins(cocos2d::CCObject* sender) {
    //TODO
}

bool LevelCategorySearchAlert::init(){
    bool init = cocos2d::CCLayerColor::initWithColor({0x00, 0x00, 0x00, 0x4B});
    if(!init) return false;

    cocos2d::CCDirector* director = cocos2d::CCDirector::sharedDirector();
    director->getTouchDispatcher()->incrementForcePrio(2);

    setTouchEnabled(true);
    setKeypadEnabled(true);

    cocos2d::CCSize winSize = director->getWinSize();
    m_pLayer = cocos2d::CCLayer::create();

    this->addChild(m_pLayer);

    cocos2d::extension::CCScale9Sprite* bg = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    bg->setContentSize({ 360.0f, 190.0f });
    m_pLayer->addChild(bg, -1);
    bg->setPosition({ winSize.width / 2, winSize.height / 2 });

    auto closeButton = gd::CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"),
        this,
        menu_selector(LevelCategorySearchAlert::onClose)
    );

    m_pButtonMenu = CCMenu::create();
    m_pLayer->addChild(m_pButtonMenu, 10);
    m_pButtonMenu->addChild(closeButton);
    closeButton->setPosition({-170.5f, 79});
    closeButton->setSizeMult(1.2f);

    auto searchTitle = CCLabelBMFont::create("Search", "bigFont.fnt");
    searchTitle->setPosition({0,71});

    m_pButtonMenu->addChild(searchTitle);

    auto separator = CCSprite::createWithSpriteFrameName("floorLine_001.png");
    separator->setPosition({285,207});
    separator->setScaleX(0.75f);
    separator->setOpacity(100);
    m_pLayer->addChild(separator);

    /*auto neighborButton = createButton(m_pButtonMenu, "Neighbors", menu_selector(LevelCategorySearchAlert::onNeighbors), 0, 4, (int)(120*0.6), 44*0.6f, 0.6f);
    auto similarButton = createButton(m_pButtonMenu, "Similar", menu_selector(LevelCategorySearchAlert::onSimilar), 0, -48, (int)(120*0.6), 44*0.6f, 0.6f);*/

    auto similarButton = createButton(m_pButtonMenu, "Played", menu_selector(LevelCategorySearchAlert::onPlayed), -57, 18, (int)(120*0.6), 44*0.6f, 0.6f);
    auto percentageButton = createButton(m_pButtonMenu, "Percentage", menu_selector(LevelCategorySearchAlert::onPercentage), 57, 18, (int)(120*0.6), 44*0.6f, 0.6f);
    auto completedButton = createButton(m_pButtonMenu, "Completed", menu_selector(LevelCategorySearchAlert::onCompleted), -114, -22, (int)(120*0.6), 44*0.6f, 0.6f);
    auto orbsButton = createButton(m_pButtonMenu, "C. With Orbs", menu_selector(LevelCategorySearchAlert::onOrbs), 0, -22, (int)(120*0.6), 44*0.6f, 0.6f);
    auto leaderboardButton = createButton(m_pButtonMenu, "C. Leaderboard", menu_selector(LevelCategorySearchAlert::onLeaderboard), 114, -22, (int)(120*0.6), 44*0.6f, 0.6f);
    auto coinsButton = createButton(m_pButtonMenu, "With Coins", menu_selector(LevelCategorySearchAlert::onCoins), -57, -64, (int)(120*0.6), 44*0.6f, 0.6f);
    auto noCoinsButton = createButton(m_pButtonMenu, "Without Coins", menu_selector(LevelCategorySearchAlert::onNoCoins), 57, -64, (int)(120*0.6), 44*0.6f, 0.6f);
    /*auto songButton = createButton(m_pButtonMenu, "Leaderboard", menu_selector(LevelCategorySearchAlert::onLeaderboard), 0, -48, (int)(120*0.6), 44*0.6f, 0.6f);*/

    return true;
}

gd::CCMenuItemSpriteExtra* LevelCategorySearchAlert::createButton(CCNode* menu, const char* text, cocos2d::SEL_MenuHandler handler, float x, float y, int width, float height, float scale){
    auto buttonSprite = gd::ButtonSprite::create(text, width, true, "bigFont.fnt", "GJ_button_01.png", height, scale);
    auto buttonButton = gd::CCMenuItemSpriteExtra::create(
        buttonSprite,
        this,
        handler
    );
    buttonButton->setSizeMult(1.2f);
    buttonButton->setPosition({x,y});
    menu->addChild(buttonButton);

    return buttonButton;
}
#include "LevelCategorySearchAlert.h"
#include "LevelSearchViewLayer.h"
#include <deque>

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
    std::deque<GJGameLevel*> levelsDeque;
    auto levels = GameLevelManager::sharedState()->m_onlineLevels;
    CCDictElement* obj;
    CCDICT_FOREACH(levels, obj){
        auto currentLvl = static_cast<GJGameLevel*>(obj->getObject());
        levelsDeque.push_back(currentLvl);
    }

    auto browserLayer = LevelSearchViewLayer::scene(levelsDeque);
    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
    CCDirector::sharedDirector()->pushScene(transitionFade);
}

void LevelCategorySearchAlert::onTestBtn(CCObject* sender) {
    std::deque<GJGameLevel*> levelsDeque;
    auto levels = GameLevelManager::sharedState()->m_onlineLevels;
    CCDictElement* obj;
    CCDICT_FOREACH(levels, obj){
        auto currentLvl = static_cast<GJGameLevel*>(obj->getObject());
        levelsDeque.push_back(currentLvl);
    }

    BISearchObject searchObj;
    searchObj.starRangeMin = 4;
    searchObj.starRangeMax = 7;
    searchObj.unfeatured = true;
    searchObj.copied = true;

    auto browserLayer = LevelSearchViewLayer::scene(levelsDeque, searchObj);
    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
    CCDirector::sharedDirector()->pushScene(transitionFade);
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
    bg->setContentSize({ 360.0f, 180.0f });
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
    searchTitle->setPosition({0,66});

    m_pButtonMenu->addChild(searchTitle);

    auto separator = CCSprite::createWithSpriteFrameName("floorLine_001.png");
    separator->setPosition({285,202});
    separator->setScaleX(0.75f);
    separator->setOpacity(100);
    m_pLayer->addChild(separator);

    /*auto neighborButton = createButton(m_pButtonMenu, "Neighbors", menu_selector(LevelCategorySearchAlert::onNeighbors), 0, 4, (int)(120*0.6), 44*0.6f, 0.6f);
    auto similarButton = createButton(m_pButtonMenu, "Similar", menu_selector(LevelCategorySearchAlert::onSimilar), 0, -48, (int)(120*0.6), 44*0.6f, 0.6f);*/

    auto similarButton = createButton(m_pButtonMenu, "Played", menu_selector(LevelCategorySearchAlert::onPlayed), -75, 8, (int)(120*0.6), 44*0.6f, 0.6f);
    auto neighborButton = createButton(m_pButtonMenu, "Test btn", menu_selector(LevelCategorySearchAlert::onTestBtn), 75, 8, (int)(120*0.6), 44*0.6f, 0.6f);
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
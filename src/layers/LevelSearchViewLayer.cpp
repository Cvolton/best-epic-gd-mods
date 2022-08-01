#include "LevelSearchViewLayer.h"
#include "DailyListView.h"
#include "JumpToPageLayer.h"
#include "../managers/CvoltonManager.h"

using namespace gd;
using namespace cocos2d;
using namespace cocos2d::extension;

LevelSearchViewLayer* LevelSearchViewLayer::create() {
    auto ret = new LevelSearchViewLayer();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool LevelSearchViewLayer::init() {

    auto CM = CvoltonManager::sharedState();
    CM->loadTextures();

    auto GLM = gd::GameLevelManager::sharedState();
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto backgroundSprite = CCSprite::create("game_bg_14_001.png");
    bool controllerConnected = CCApplication::sharedApplication()->getControllerConnected();

    backgroundSprite->setScale(winSize.width / backgroundSprite->getContentSize().width);
    backgroundSprite->setAnchorPoint({0, 0});
    backgroundSprite->setPosition({0,-75});
    backgroundSprite->setColor({164, 0, 255}); //purple
    backgroundSprite->setZOrder(-2);
    addChild(backgroundSprite);

    auto menu = CCMenu::create();
    addChild(menu);
    
    auto backBtn = gd::CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(LevelSearchViewLayer::onBack)
    );

    backBtn->setPosition({(-winSize.width / 2) + 25, (winSize.height / 2) - 25});
    menu->addChild(backBtn);

    /*auto menuBack = CCMenu::create();
    menuBack->addChild(backBtn);
    menuBack->setPosition({25, winSize.height - 25});

    addChild(menuBack);*/

    setTouchEnabled(true);
    setKeypadEnabled(true);

    m_loadedLevels = CCArray::create();
    m_loadedLevels->retain();

    //corners
    auto cornerBL = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    cornerBL->setPosition({0,0});
    cornerBL->setAnchorPoint({0,0});
    addChild(cornerBL, -1);

    auto cornerBR = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    cornerBR->setPosition({winSize.width,0});
    cornerBR->setAnchorPoint({0,0});
    cornerBR->setRotation(270);
    addChild(cornerBR, -1);

    auto levels = GameLevelManager::sharedState()->m_onlineLevels;
    CCDictElement* obj;
    CCDICT_FOREACH(levels, obj){
        auto currentLvl = static_cast<GJGameLevel*>(obj->getObject());
        m_allLevels.push_back(currentLvl);
    }

    loadPage(0);
    startLoading();

    return true;
}

void LevelSearchViewLayer::startLoading(){
    std::stringstream toDownload;
    bool first = true;
    for(size_t i = 0; i < ((m_allLevels.size() < 10) ? m_allLevels.size() : 10); i++) {
        if(!first) toDownload << ",";
        toDownload << m_allLevels[0]->levelID;
        m_allLevels.pop_front();
        first = false;
    }

    auto searchObj = GJSearchObject::create(kSearchType19, toDownload.str());
    auto GLM = GameLevelManager::sharedState();
    GLM->m_pOnlineListDelegate = this;
    GLM->getOnlineLevels(searchObj);
}

void LevelSearchViewLayer::loadPage(int page){
    if(!m_loadedLevels) return;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if(m_listLayer != nullptr) m_listLayer->removeFromParentAndCleanup(true);

    m_listView = LevelSearchListView::create(m_loadedLevels, 356.f, 220.f);
    m_listLayer = GJListLayer::create(m_listView, "Levels", {191, 114, 62, 255}, 356.f, 220.f);
    m_listLayer->setPosition(winSize / 2 - m_listLayer->getScaledContentSize() / 2 - CCPoint(0,5));
    addChild(m_listLayer);
}

void LevelSearchViewLayer::keyBackClicked() {
    setTouchEnabled(false);
    setKeypadEnabled(false);
    for(size_t i = 0; i < m_loadedLevels->count(); i++) {
        auto level = static_cast<GJGameLevel*>(m_loadedLevels->objectAtIndex(i));
        level->release();
    }

    m_loadedLevels->release();
    m_loadedLevels = nullptr;
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}


void LevelSearchViewLayer::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* LevelSearchViewLayer::scene() {
    auto layer = LevelSearchViewLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

void LevelSearchViewLayer::loadListFinished(cocos2d::CCArray* levels, const char*) {
    for(size_t i = 0; i < levels->count(); i++) {
        auto level = static_cast<GJGameLevel*>(levels->objectAtIndex(i));
        if(level == nullptr) continue;

        level->retain();
        m_loadedLevels->addObject(level);
    }

    loadPage(0);
}

void LevelSearchViewLayer::loadListFailed(const char*) {

}

void LevelSearchViewLayer::setupPageInfo(std::string, const char*) {

}
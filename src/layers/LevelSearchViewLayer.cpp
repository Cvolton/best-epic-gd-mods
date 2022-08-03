#include "LevelSearchViewLayer.h"
#include "DailyListView.h"
#include "JumpToPageLayer.h"
#include "../managers/CvoltonManager.h"
#include "../utils.hpp"

using namespace gd;
using namespace cocos2d;
using namespace cocos2d::extension;

LevelSearchViewLayer* LevelSearchViewLayer::create(std::deque<gd::GJGameLevel*> allLevels, BISearchObject searchObj) {
    auto ret = new LevelSearchViewLayer();
    if (ret && ret->init(allLevels, searchObj)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool LevelSearchViewLayer::init(std::deque<gd::GJGameLevel*> allLevels, BISearchObject searchObj) {

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

    m_allLevels = m_unloadedLevels = allLevels;

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

    m_statusText = CCLabelBMFont::create("Waiting", "goldFont.fnt");
    m_statusText->setPosition({winSize.width / 2, winSize.height / 2 - 147});
    m_statusText->setScale(.7f);
    addChild(m_statusText);

    auto prevSprite = CCSprite::createWithSpriteFrameName(controllerConnected ? "controllerBtn_DPad_Left_001.png" : "GJ_arrow_03_001.png");
    m_prevBtn = gd::CCMenuItemSpriteExtra::create(
        prevSprite,
        this,
        menu_selector(LevelSearchViewLayer::onPrev)
    );
    m_prevBtn->setPosition({- (winSize.width / 2) + 25, 0});
    menu->addChild(m_prevBtn);

    auto nextSprite = CCSprite::createWithSpriteFrameName(controllerConnected ? "controllerBtn_DPad_Right_001.png" : "GJ_arrow_03_001.png");
    if(!controllerConnected) nextSprite->setFlipX(true);
    m_nextBtn = gd::CCMenuItemSpriteExtra::create(
        nextSprite,
        this,
        menu_selector(LevelSearchViewLayer::onNext)
    );
    m_nextBtn->setPosition({+ (winSize.width / 2) - 25, 0});
    menu->addChild(m_nextBtn);

    m_counter = CCLabelBMFont::create("0 to 0", "goldFont.fnt");
    m_counter->setAnchorPoint({ 1.f, 1.f });
    m_counter->setPosition(winSize - CCPoint(7,3));
    m_counter->setScale(0.5f);
    addChild(m_counter);

    m_searchObj = searchObj;

    loadPage(true);
    startLoading();

    return true;
}

void LevelSearchViewLayer::startLoading(){
    if(m_unloadedLevels.empty() || (m_page + 2) * 10 < m_loadedLevels->count()) {
        setTextStatus(true);
        return;
    }

    setTextStatus(false);

    bool starFilter = m_searchObj.star || m_searchObj.starRangeMin > 0;
    size_t levelsPerRequest = (starFilter) ? 300 : 10;

    std::stringstream toDownload;
    bool first = true;
    for(size_t i = 0; i < ((m_unloadedLevels.size() < levelsPerRequest) ? m_unloadedLevels.size() : levelsPerRequest); i++) {
        if(!first) toDownload << ",";
        toDownload << m_unloadedLevels[0]->levelID;
        m_unloadedLevels.pop_front();
        first = false;
    }

    auto searchObj = GJSearchObject::create(starFilter ? kSearchTypeMapPackList : kSearchType19, toDownload.str());
    auto GLM = GameLevelManager::sharedState();
    GLM->m_pOnlineListDelegate = this;
    GLM->getOnlineLevels(searchObj);
}

void LevelSearchViewLayer::loadPage(bool reload){
    if(!m_loadedLevels) return;

    auto currentPage = CCArray::create();
    auto end = (m_page + 1) * 10;
    end = m_loadedLevels->count() < end ? m_loadedLevels->count() : end;
    for(size_t i = m_page * 10; i < end; i++) {
        currentPage->addObject(m_loadedLevels->objectAtIndex(i));
    }

    size_t lastIndex = (m_page * 10) + currentPage->count();
    size_t totalAmount = m_unloadedLevels.size() + m_loadedLevels->count();
    m_counter->setCString(CCString::createWithFormat("%i to %i of %i / %i", (m_page * 10) + 1, lastIndex, m_loadedLevels->count(), totalAmount)->getCString());

    if(m_page == 0) m_prevBtn->setVisible(false);
    else m_prevBtn->setVisible(true);

    if(m_loadedLevels->count() > lastIndex) m_nextBtn->setVisible(true);
    else m_nextBtn->setVisible(false);

    if(!reload && m_shownLevels == currentPage->count()) return;
    m_shownLevels = currentPage->count();

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if(m_listLayer != nullptr) m_listLayer->removeFromParentAndCleanup(true);

    m_listView = LevelSearchListView::create(currentPage, 356.f, 220.f);
    m_listLayer = GJListLayer::create(m_listView, "Levels", {191, 114, 62, 255}, 356.f, 220.f);
    m_listLayer->setPosition(winSize / 2 - m_listLayer->getScaledContentSize() / 2 - CCPoint(0,1));
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

CCScene* LevelSearchViewLayer::scene(std::deque<gd::GJGameLevel*> allLevels, BISearchObject searchObj) {
    auto layer = LevelSearchViewLayer::create(allLevels, searchObj);
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

void LevelSearchViewLayer::loadListFinished(cocos2d::CCArray* levels, const char*) {
    for(size_t i = 0; i < levels->count(); i++) {
        auto level = static_cast<GJGameLevel*>(levels->objectAtIndex(i));
        if(level == nullptr) continue;

        level->retain();
        if(BetterInfo::levelMatchesObject(level, m_searchObj)) m_loadedLevels->addObject(level);
    }

    loadPage(false);

    startLoading();
}

void LevelSearchViewLayer::loadListFailed(const char*) {
    //setTextStatus(true);
    startLoading();
}

void LevelSearchViewLayer::setupPageInfo(std::string, const char*) {

}

void LevelSearchViewLayer::setTextStatus(bool finished) {
    if(m_statusText) m_statusText->setString(finished ? "Finished" : "Loading");
}

void LevelSearchViewLayer::onPrev(cocos2d::CCObject*) {
    if(m_page == 0) return;
    m_page--;
    loadPage(true);
}

void LevelSearchViewLayer::onNext(cocos2d::CCObject*) {
    m_page++;
    loadPage(true);
    startLoading();
}

void LevelSearchViewLayer::keyDown(enumKeyCodes key){
    switch(key){
        case KEY_Left:
        case CONTROLLER_Left:
            if(m_prevBtn->isVisible() == true) onPrev(nullptr);
            break;
        case KEY_Right:
        case CONTROLLER_Right:
            if(m_nextBtn->isVisible() == true) onNext(nullptr);
            break;
        default:
            CCLayer::keyDown(key);
    }
}
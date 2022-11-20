#include "LevelBrowserEndLayer.h"

#include <cocos2d.h>
#include <gd.h>

using namespace cocos2d;
using namespace gd;

LevelBrowserEndLayer* LevelBrowserEndLayer::create(LevelBrowserLayer* levelBrowserLayer){
    auto ret = new LevelBrowserEndLayer();
    levelBrowserLayer->retain();
    ret->levelBrowserLayer = levelBrowserLayer;
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

void LevelBrowserEndLayer::onClose(cocos2d::CCObject* sender)
{
    levelBrowserLayer->searchObject->m_nPage = min;
    if(levelBrowserLayer != nullptr) levelBrowserLayer->loadPage(levelBrowserLayer->searchObject);
    if(levelBrowserLayer != nullptr) levelBrowserLayer->release();
    if(circle != nullptr) circle->fadeAndRemove();

    auto GLM = GameLevelManager::sharedState();
    GLM->m_pOnlineListDelegate = nullptr;

    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void LevelBrowserEndLayer::onOK(cocos2d::CCObject* sender){
    levelBrowserLayer->searchObject->m_nPage = 1;
    
    getOnlineLevels(levelBrowserLayer->searchObject);

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    circle = LoadingCircle::create();
    //m_pButtonMenu->addChild(circle);
    circle->setPosition({0, -55});
    circle->setScale(.475f);
    circle->show();
    circle->setZOrder(999);

    goBtn->setVisible(false);

    this->getScheduler()->scheduleSelector(schedule_selector(LevelBrowserEndLayer::onTimer), this, 1, false);
}

bool LevelBrowserEndLayer::init(){
    bool init = createBasics({230.0f, 160.0f}, menu_selector(LevelBrowserEndLayer::onClose), 0.8f);
    if(!init) return false;

    createTextLabel("Finding Last Page", {0,63}, 0.7f, m_pButtonMenu, "goldFont.fnt");
    //textLabel = createTextLabel("Minimum: 0\nMaximum: 0\nTesting: 0", {-90,38}, 1.f, m_pButtonMenu, "chatFont.fnt");
    //textLabel = createTextLabel("Press Go to start searching.\nThis might take several minutes\nwith certain filter combinations.", {-90,38.5}, .95f, m_pButtonMenu, "chatFont.fnt");
    //textLabel->setAnchorPoint({0,1});

    textLabel = gd::TextArea::create("chatFont.fnt", false, "Press <cg>Go</c> to start searching.\nThis might take several\nminutes with certain\nfilter combinations.", 1, 295, 20, {0,1});
    //description->setScale(descDelimiter);
    textLabel->setAnchorPoint({0,1});
    textLabel->setPosition({-90,44});
    //textLabel->setScale(0.85f);
    m_pButtonMenu->addChild(textLabel);

    /*textNode = CCTextInputNode::create("Num", this, 
            "bigFont.fnt", 50, 30);
    textNode->setLabelPlaceholderColor({0x75, 0xAA, 0xF0});
    textNode->setAllowedChars("0123456789");
    textNode->setMaxLabelScale(0.7f);
    textNode->setPosition({0,6});
    m_pButtonMenu->addChild(textNode);*/

    cocos2d::extension::CCScale9Sprite* infoBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    infoBg->setContentSize({320,140});
    infoBg->setColor({123,60,31});
    m_pButtonMenu->addChild(infoBg, -1);
    infoBg->setPosition({0,6});
    infoBg->setScale(0.6f);

    auto buttonSprite = gd::ButtonSprite::create("Go", 40, true, "goldFont.fnt", "GJ_button_01.png", 30, 0.8f);
    goBtn = gd::CCMenuItemSpriteExtra::create(
        buttonSprite,
        this,
        menu_selector(LevelBrowserEndLayer::onOK)
    );
    goBtn->setSizeMult(1.2f);
    goBtn->setPosition({0,-55});
    m_pButtonMenu->addChild(goBtn);

    timer = CCLabelBMFont::create("99", "bigFont.fnt");
    timer->setPosition({110,-73});
    timer->setAnchorPoint({1,0});
    timer->setScale(.5f);
    m_pButtonMenu->addChild(timer);
    timer->setVisible(false);

    return true;
}

CCLabelBMFont* LevelBrowserEndLayer::createTextLabel(const std::string text, const CCPoint& position, const float scale, CCNode* menu, const char* font){
    CCLabelBMFont* bmFont = CCLabelBMFont::create(text.c_str(), font);
    bmFont->setPosition(position);
    bmFont->setScale(scale);
    menu->addChild(bmFont);
    return bmFont;
}

void LevelBrowserEndLayer::loadListFinished(cocos2d::CCArray*, const char* test){
    min = levelBrowserLayer->searchObject->m_nPage;

    if(max == 0) {
        levelBrowserLayer->searchObject->m_nPage *= 2;

        getOnlineLevels(levelBrowserLayer->searchObject);

        updateDisplay();
        return;
    }

    if(max - min == 1 && max != 0) {
        onClose(nullptr);
        return;
    }

    levelBrowserLayer->searchObject->m_nPage = levelBrowserLayer->searchObject->m_nPage + ((max - min) / 2);

    getOnlineLevels(levelBrowserLayer->searchObject);

    updateDisplay();
}
void LevelBrowserEndLayer::loadListFailed(const char* test){
    max = levelBrowserLayer->searchObject->m_nPage;
    if(requestsToMax == 0) {
        requestsToMax = requests;
        //maxReached = std::time(nullptr);
        maxReached = lastLoad;
    }

    if(max - min == 1) {
        onClose(nullptr);
        return;
    };

    levelBrowserLayer->searchObject->m_nPage = levelBrowserLayer->searchObject->m_nPage - ((max - min) / 2);

    getOnlineLevels(levelBrowserLayer->searchObject);

    updateDisplay();
}
void LevelBrowserEndLayer::setupPageInfo(std::string, const char*){
    
}

void LevelBrowserEndLayer::updateDisplay(){
    if(!updateLabel) return;

    lastLoad = std::time(nullptr);

    auto maximumStr = requestsToMax <= 0 ? "" : CCString::createWithFormat(" / %i", requestsToMax * 2)->getCString();

    textLabel->setString(
        CCString::createWithFormat("<cg>Minimum</c>: %i\n<cy>Current</c>: %i\n<cr>Maximum</c>: %i\n<cl>Requests</c>: %i%s", min, levelBrowserLayer->searchObject->m_nPage, max, ++requests, maximumStr)->getCString()
    );
    textLabel->setScale(1.f);
}

void LevelBrowserEndLayer::onTimer(float dt) {
    if(requests > requestsToMax && requestsToMax > 0) {
        double timePerRequest = (double) (lastLoad - maxReached) / (requests - requestsToMax);
        int requestsRemaining = (requestsToMax * 2) - requests;
        time_t timeElapsed = std::time(nullptr) - lastLoad;
        timer->setString(std::format("{:.0f}", (timePerRequest * requestsRemaining) - timeElapsed).c_str());
        timer->setVisible(true);
    }
}

void LevelBrowserEndLayer::getOnlineLevels(GJSearchObject* searchObj) {
    auto GLM = GameLevelManager::sharedState();
    GLM->m_pOnlineListDelegate = this;
    auto storedLevels = GLM->getStoredOnlineLevels(searchObj->getKey());
    if(storedLevels) {
        updateLabel = false;
        loadListFinished(storedLevels, "");
    } else {
        updateLabel = true;
        this->getScheduler()->scheduleSelector(schedule_selector(LevelBrowserEndLayer::onQueueDownload), this, 1, 0, 0.25f, false);
    }
}

void LevelBrowserEndLayer::onQueueDownload(float dt) {
    auto GLM = GameLevelManager::sharedState();
    GLM->getOnlineLevels(levelBrowserLayer->searchObject);
}
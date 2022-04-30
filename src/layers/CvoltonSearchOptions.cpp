#include "CvoltonSearchOptions.h"
#include "../managers/CvoltonManager.h"

#include <cocos2d.h>
#include <gd.h>

using namespace cocos2d;
using namespace gd;

const int completedMax = 6;

CvoltonSearchOptions* CvoltonSearchOptions::create(){
    auto ret = new CvoltonSearchOptions();
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

void CvoltonSearchOptions::onClose(cocos2d::CCObject* sender)
{
    destroyToggles();
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void CvoltonSearchOptions::onPrev(cocos2d::CCObject* sender)
{
    auto layer = MoreSearchLayer::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer);
    onClose(sender);
}

bool CvoltonSearchOptions::init(){
    bool init = createBasics({440.0f, 290.0f}, menu_selector(CvoltonSearchOptions::onClose), 0.8f);
    if(!init) return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    createTextLabel("BI Advanced Options", {(winSize.width / 2), winSize.height - 35}, 1.f, m_pLayer, "bigFont.fnt");
    createButton("GJ_arrow_03_001.png", {- (winSize.width / 2) + 30, 0}, menu_selector(CvoltonSearchOptions::onPrev));

    drawToggles();

    return true;
}

void CvoltonSearchOptions::createToggle(const char* option, const char* name, float x, float y){
    auto CM = CvoltonManager::sharedState();

    auto buttonSprite = CCSprite::createWithSpriteFrameName(CM->getOption(option) ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png");
    buttonSprite->setScale(0.8f);
    auto button = gd::CCMenuItemSpriteExtra::create(
        buttonSprite,
        this,
        menu_selector(CvoltonSearchOptions::onToggle)
    );
    m_pButtonMenu->addChild(button);
    button->setPosition({x, y});
    auto optionString = CCString::create(option);
    optionString->retain();
    button->setUserData(optionString);
    button->setSizeMult(1.2f);

    auto label = createTextLabel(name, {x + 20, y}, 0.5f, m_pButtonMenu);
    label->setAnchorPoint({0,0.5f});
    label->limitLabelWidth(80, 0.5f, 0);
}

void CvoltonSearchOptions::destroyToggles(){
    //starting at 1 because 0 is the close button and 1 is the prev button
    unsigned int totalMembers = m_pButtonMenu->getChildrenCount();
    for(unsigned int i = 2; i < totalMembers; i++){
        //static index 1 because we're actively moving the elements
        auto object = static_cast<CCNode*>(m_pButtonMenu->getChildren()->objectAtIndex(2));
        auto userData = object->getUserData();
        if(userData != nullptr) static_cast<CCString*>(userData)->release();
        //m_pButtonMenu->removeChild(object, false);
        object->removeFromParent();
    }
    toggleCount = 0;
}

void CvoltonSearchOptions::drawToggles(){
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    createToggle("search_contains", "Name Contains", -170, 75); //40 -60, 170 -60, 300 -60, 40 -110
    createToggle("search_no_id", "No ID Search", -40, 75);
    createToggle("search_surround_percent", "No Forced Star", 90, 75);
    createToggle("search_trim", "Trim Spaces", -170, 25);

    createTextLabel("Completed Mode:", {0, - (winSize.height / 2) + 65}, 0.5f, m_pButtonMenu, "goldFont.fnt");
    createButton("edit_leftBtn_001.png", {-120, - (winSize.height / 2) + 40}, menu_selector(CvoltonSearchOptions::onCompletedPrev), 1.2f);
    auto label = createTextLabel(getCompletedString(), {0, - (winSize.height / 2) + 40}, 1, m_pButtonMenu, "bigFont.fnt");
    label->limitLabelWidth(200, 0.8f, 0);
    createButton("edit_rightBtn_001.png", {120, - (winSize.height / 2) + 40}, menu_selector(CvoltonSearchOptions::onCompletedNext), 1.2f);
}

void CvoltonSearchOptions::onCompletedPrev(cocos2d::CCObject* sender)
{
    auto CM = CvoltonManager::sharedState();
    int value = CM->getOptionInt("search_completed") - 1;
    if(value < 0) value = completedMax - 1;
    CM->setOptionInt("search_completed", value);
    destroyToggles();
    drawToggles();
}

void CvoltonSearchOptions::onCompletedNext(cocos2d::CCObject* sender)
{
    auto CM = CvoltonManager::sharedState();
    CM->setOptionInt("search_completed", (CM->getOptionInt("search_completed") + 1) % completedMax);
    destroyToggles();
    drawToggles();
}

std::string CvoltonSearchOptions::getCompletedString(){
    auto CM = CvoltonManager::sharedState();
    int type = CM->getOptionInt("search_completed");
    const char* types[] = {"Default", "Completed Levels", "Completed Orbs", "Completed Leaderboard", "Completed With Coins", "Completed Without Coins"};
    return std::to_string(type) + ": " + types[type % completedMax];
}
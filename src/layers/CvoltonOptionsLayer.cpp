#include "CvoltonOptionsLayer.h"
#include "../managers/CvoltonManager.h"

#include <cocos2d.h>
#include <gd.h>

using namespace cocos2d;
using namespace gd;

CvoltonOptionsLayer* CvoltonOptionsLayer::create(){
    auto ret = new CvoltonOptionsLayer();
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

void CvoltonOptionsLayer::onClose(cocos2d::CCObject* sender)
{   
    auto CM = CvoltonManager::sharedState();
    CM->save();
    destroyToggles();
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

bool CvoltonOptionsLayer::init(){
    bool init = createBasics({370.0f, 240.0f}, menu_selector(CvoltonOptionsLayer::onClose), 0.8f);
    if(!init) return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    createTextLabel("Mod Settings", {(winSize.width / 2),(winSize.height / 2) + 78 + 20}, 0.7f, m_pLayer, "goldFont.fnt");

    drawToggles();

    return true;
}

CCLabelBMFont* CvoltonOptionsLayer::createTextLabel(const std::string text, const CCPoint& position, const float scale, CCNode* menu, const char* font){
    CCLabelBMFont* bmFont = CCLabelBMFont::create(text.c_str(), font);
    bmFont->setPosition(position);
    bmFont->setScale(scale);
    menu->addChild(bmFont);
    return bmFont;
}

void CvoltonOptionsLayer::onToggle(cocos2d::CCObject* sender)
{
    sender->retain();
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto CM = CvoltonManager::sharedState();
    CM->toggleOption(static_cast<CCString*>(button->getUserData())->getCString());

    destroyToggles();
    drawToggles();
    sender->release();
}

void CvoltonOptionsLayer::createToggle(const char* option, const char* name){
    auto CM = CvoltonManager::sharedState();

    auto buttonSprite = CCSprite::createWithSpriteFrameName(CM->getOption(option) ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png");
    buttonSprite->setScale(0.8f);
    auto button = gd::CCMenuItemSpriteExtra::create(
        buttonSprite,
        this,
        menu_selector(CvoltonOptionsLayer::onToggle)
    );
    m_pButtonMenu->addChild(button);
    float y = 65.f - (toggleCount++ * 40.f);
    button->setPosition({-157, y});
    auto optionString = CCString::create(option);
    optionString->retain();
    button->setUserData(optionString);
    button->setSizeMult(1.2f);

    auto label = createTextLabel(name, {-137, y}, 0.5f, m_pButtonMenu);
    label->setAnchorPoint({0,0.5f});
}

void CvoltonOptionsLayer::createButtonToggle(const char* option, CCNode* sprite, float x, float y, float scale){
    auto CM = CvoltonManager::sharedState();

    //auto buttonSprite = CCSprite::createWithSpriteFrameName(sprite);
    sprite->setScale(scale);
    auto button = gd::CCMenuItemSpriteExtra::create(
        sprite,
        this,
        menu_selector(CvoltonOptionsLayer::onToggle)
    );
    m_pButtonMenu->addChild(button);
    button->setPosition({x, y});
    if(!CM->getOption(option)) button->setColor({125,125,125});
    auto optionString = CCString::create(option);
    optionString->retain();
    button->setUserData(optionString);
    button->setSizeMult(1.2f);
}

void CvoltonOptionsLayer::destroyToggles(){
    //starting at 1 because 0 is the close button
    unsigned int totalMembers = m_pButtonMenu->getChildrenCount();
    for(unsigned int i = 1; i < totalMembers; i++){
        //static index 1 because we're actively moving the elements
        auto object = static_cast<CCNode*>(m_pButtonMenu->getChildren()->objectAtIndex(1));
        auto userData = object->getUserData();
        if(userData != nullptr) static_cast<CCString*>(userData)->release();
        //m_pButtonMenu->removeChild(object, false);
        object->removeFromParent();
    }
    toggleCount = 0;
}

void CvoltonOptionsLayer::drawToggles(){
    //createToggle("no_update_check", "Disable Update Check");
    createToggle("no_green_user", "Disable Green Username Fix");
    createToggle("no_level_info", "Disable Extended Level Info");
    createToggle("white_id", "White Level ID text");
    createToggle("auto_submit", "Auto submit to level leaderboards");
    createToggle("alt_button", "Alt BI button texture (req. restart)");
}
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
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

bool CvoltonOptionsLayer::init(){
    bool init = createBasics({300.0f, 200.0f}, menu_selector(CvoltonOptionsLayer::onClose), 0.8f);
    if(!init) return false;

    createTextLabel("Mod Settings", {0,78}, 0.7f, m_pButtonMenu, "goldFont.fnt");

    createToggle("cv_test", "Test toggle :D");
    createToggle("no_update_check", "Disable Update Check");
    createToggle("no_green_user", "Disable Green Username Fix");
    createToggle("no_level_info", "Disable Extended Level Info");

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
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto CM = CvoltonManager::sharedState();
    CM->toggleOption(static_cast<CCString*>(button->getUserData())->getCString());
    
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
    float y = 45.f - (toggleCount++ * 40.f);
    button->setPosition({-127, y});
    auto optionString = CCString::create(option);
    optionString->retain();
    button->setUserData(optionString);
    button->setSizeMult(1.2f);

    auto label = createTextLabel(name, {-107, y}, 0.5f, m_pButtonMenu);
    label->setAnchorPoint({0,0.5f});
}
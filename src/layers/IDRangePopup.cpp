#include "IDRangePopup.h"
#include "../managers/CvoltonManager.h"

#include <cocos2d.h>
#include <gd.h>

using namespace cocos2d;
using namespace gd;

IDRangePopup* IDRangePopup::create(IDRangeDelegate* delegate, int min, int max, const char* text){
    auto ret = new IDRangePopup();
    if (ret && ret->init(delegate, min, max, text)) {
        //robert 1 :D
        ret->autorelease();
    } else {
        //robert -1
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void IDRangePopup::onClose(cocos2d::CCObject* sender)
{
    if(delegate) delegate->onIDRangeFinished(minID(), maxID());
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

bool IDRangePopup::init(IDRangeDelegate* delegate, int min, int max, const char* text){
    bool init = createBasics({240.0f, 150.0f}, menu_selector(IDRangePopup::onClose), .8f, {0x00, 0x00, 0x00, 0x96});
    if(!init) return false;

    this->delegate = delegate;

    auto CM = CvoltonManager::sharedState();
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    createTitle(text, .45f, .9f);

    minNode = CCTextInputNode::create("Min", this, "bigFont.fnt", 100, 30);
    minNode->setLabelPlaceholderColor({0x75, 0xAA, 0xF0});
    if(min != 0 || max != 0) minNode->setString(std::to_string(min).c_str());
    minNode->setAllowedChars("0123456789");
    minNode->setMaxLabelScale(0.7f);
    minNode->setPosition({0,6});
    m_pButtonMenu->addChild(minNode);

    maxNode = CCTextInputNode::create("Max", this, "bigFont.fnt", 100, 30);
    maxNode->setLabelPlaceholderColor({0x75, 0xAA, 0xF0});
    if(min != 0 || max != 0) maxNode->setString(std::to_string(max).c_str());
    maxNode->setAllowedChars("0123456789");
    maxNode->setMaxLabelScale(0.7f);
    maxNode->setPosition({0,-37});
    m_pButtonMenu->addChild(maxNode);

    auto minBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    minBg->setContentSize({200,60});
    minBg->setColor({123,60,31});
    m_pButtonMenu->addChild(minBg, -1);
    minBg->setPosition({0,6});
    minBg->setScale(0.6f);

    auto maxBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    maxBg->setContentSize({200,60});
    maxBg->setColor({123,60,31});
    m_pButtonMenu->addChild(maxBg, -1);
    maxBg->setPosition({0,-37});
    maxBg->setScale(0.6f);
    //createTextLabel("Advanced Options", {(winSize.width / 2), (winSize.height / 2) + 125}, 1.f, m_pLayer, "bigFont.fnt");

    return true;
}

int IDRangePopup::minID(){
    int ret = 0;
    try{
        ret = std::stoi(minNode->getString());
    }catch(...){}
    return ret;
}

int IDRangePopup::maxID(){
    int ret = 0;
    try{
        ret = std::stoi(maxNode->getString());
    }catch(...){}
    return ret;
}
#include "JumpToPageLayer.h"

#include <cocos2d.h>
#include <gd.h>

using namespace cocos2d;
using namespace gd;

JumpToPageLayer* JumpToPageLayer::create(gd::InfoLayer* infoLayer){
    auto ret = new JumpToPageLayer();
    infoLayer->retain();
    ret->infoLayer = infoLayer;
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

JumpToPageLayer* JumpToPageLayer::create(PageNumberDelegate* pageNumberDelegate){
    auto ret = new JumpToPageLayer();
    ret->pageNumberDelegate = pageNumberDelegate;
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

void JumpToPageLayer::onClose(cocos2d::CCObject* sender)
{
    if(infoLayer != nullptr) infoLayer->release();
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void JumpToPageLayer::onPrev(cocos2d::CCObject* sender)
{
    int newPage = pageNumber() - 1;
    if(newPage < 1) newPage = 1;
    textNode->setString(std::to_string(newPage).c_str());
}

void JumpToPageLayer::onNext(cocos2d::CCObject* sender)
{
    textNode->setString(std::to_string(pageNumber() + 1).c_str());
}

void JumpToPageLayer::onReset(cocos2d::CCObject* sender)
{
    textNode->setString("1");
}

int JumpToPageLayer::pageNumber(){
    int pageNumber = 1;
    try{
        pageNumber = std::stoi(textNode->getString());
    }catch(...){}
    return pageNumber;
}

void JumpToPageLayer::onOK(cocos2d::CCObject* sender){
    if(infoLayer != nullptr) infoLayer->loadPage(pageNumber()-1, false);
    if(pageNumberDelegate != nullptr) pageNumberDelegate->loadPage(pageNumber()-1);
    onClose(sender);
}

bool JumpToPageLayer::init(){
    bool init = createBasics({220.0f, 150.0f}, menu_selector(JumpToPageLayer::onClose), 0.8f);
    if(!init) return false;

    createTextLabel("Go to Page", {0,57}, 0.7f, m_pButtonMenu, "goldFont.fnt");

    textNode = CCTextInputNode::create("Num", this, 
            "bigFont.fnt", 50, 30);
    textNode->setLabelPlaceholderColor({0x75, 0xAA, 0xF0});
    if(infoLayer != nullptr) textNode->setString(std::to_string(infoLayer->m_nPageNumber+1).c_str());
    if(pageNumberDelegate != nullptr) textNode->setString(std::to_string(pageNumberDelegate->getPage()+1).c_str());
    textNode->setAllowedChars("0123456789");
    textNode->setMaxLabelScale(0.7f);
    textNode->setPosition({0,6});
    m_pButtonMenu->addChild(textNode);

    cocos2d::extension::CCScale9Sprite* infoBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    infoBg->setContentSize({100,60});
    infoBg->setColor({123,60,31});
    m_pButtonMenu->addChild(infoBg, -1);
    infoBg->setPosition({0,6});
    infoBg->setScale(0.6f);

    auto buttonSprite = gd::ButtonSprite::create("Go", 40, true, "goldFont.fnt", "GJ_button_01.png", 30, 0.8f);
    auto buttonButton = gd::CCMenuItemSpriteExtra::create(
        buttonSprite,
        this,
        menu_selector(JumpToPageLayer::onOK)
    );
    buttonButton->setSizeMult(1.2f);
    buttonButton->setPosition({0,-50});
    m_pButtonMenu->addChild(buttonButton);

    createButton("edit_leftBtn_001.png", {-50, 6}, menu_selector(JumpToPageLayer::onPrev), 1.1f);
    createButton("edit_rightBtn_001.png", {50, 6}, menu_selector(JumpToPageLayer::onNext), 1.1f);
    createButton("GJ_resetBtn_001.png", {93, 57}, menu_selector(JumpToPageLayer::onReset));

    return true;
}

CCLabelBMFont* JumpToPageLayer::createTextLabel(const std::string text, const CCPoint& position, const float scale, CCNode* menu, const char* font){
    CCLabelBMFont* bmFont = CCLabelBMFont::create(text.c_str(), font);
    bmFont->setPosition(position);
    bmFont->setScale(scale);
    menu->addChild(bmFont);
    return bmFont;
}
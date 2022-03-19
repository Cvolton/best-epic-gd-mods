#include "CvoltonAlertLayerStub.h"

using namespace cocos2d;
using namespace gd;

bool CvoltonAlertLayerStub::createBasics(CCPoint contentSize, cocos2d::SEL_MenuHandler onClose){
	bool init = cocos2d::CCLayerColor::initWithColor({0x00, 0x00, 0x00, 0x4B});
    if(!init) return false;

    alertSize = contentSize;

    cocos2d::CCDirector* director = cocos2d::CCDirector::sharedDirector();
    director->getTouchDispatcher()->incrementForcePrio(2);

    setTouchEnabled(true);
    setKeypadEnabled(true);

    m_pLayer = cocos2d::CCLayer::create();

    this->addChild(m_pLayer);

	cocos2d::CCSize winSize = director->getWinSize();
	cocos2d::extension::CCScale9Sprite* bg = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    bg->setContentSize(alertSize);
    m_pLayer->addChild(bg, -1);
    bg->setPosition({ winSize.width / 2, winSize.height / 2 });

    m_pButtonMenu = CCMenu::create();
    m_pLayer->addChild(m_pButtonMenu, 10);

    createButton("GJ_closeBtn_001.png", {-((alertSize.x)/2) + 9.5f, (alertSize.y / 2) - 10}, onClose);

    return true;
}

void CvoltonAlertLayerStub::createTitle(std::string text){
	auto userName = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
    userName->setPosition({0, (alertSize.y/2) - 22});
    m_pButtonMenu->addChild(userName);

    auto separator = CCSprite::createWithSpriteFrameName("floorLine_001.png");
    separator->setPosition({0, (alertSize.y/2) - 46});
    separator->setScaleX(0.75f);
    separator->setOpacity(100);
    m_pButtonMenu->addChild(separator);
}

void CvoltonAlertLayerStub::createButton(const char* texture, CCPoint position, cocos2d::SEL_MenuHandler callback, float textureScale, float sizeMult){
	auto buttonSprite = CCSprite::createWithSpriteFrameName(texture);
    buttonSprite->setScale(textureScale);
	auto button = gd::CCMenuItemSpriteExtra::create(
        buttonSprite,
        this,
        callback
    );
    m_pButtonMenu->addChild(button);
    button->setPosition(position);
    button->setSizeMult(sizeMult);
}
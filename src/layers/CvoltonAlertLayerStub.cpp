#include "CvoltonAlertLayerStub.h"

using namespace cocos2d;
using namespace gd;

bool CvoltonAlertLayerStub::createBasics(CCPoint contentSize, cocos2d::SEL_MenuHandler onClose, float closeBtnScale, const ccColor4B &color){
	bool init = cocos2d::CCLayerColor::initWithColor(color);
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

    closeBtn = createButton("GJ_closeBtn_001.png", {-((alertSize.x)/2) + 9.5f, (alertSize.y / 2) - 10}, onClose, closeBtnScale);

    return true;
}

void CvoltonAlertLayerStub::createTitle(std::string text, float separatorScale, float usernameScale){
	auto userName = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
    userName->setPosition({0, (alertSize.y/2) - 22});
    userName->setScale(usernameScale);
    m_pButtonMenu->addChild(userName);

    auto separator = CCSprite::createWithSpriteFrameName("floorLine_001.png");
    separator->setPosition({0, (alertSize.y/2) - 46});
    separator->setScaleX(separatorScale);
    separator->setOpacity(100);
    m_pButtonMenu->addChild(separator);
}

CCMenuItemSpriteExtra* CvoltonAlertLayerStub::createButton(const char* texture, CCPoint position, cocos2d::SEL_MenuHandler callback, float textureScale, float sizeMult){
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
    return button;
}

void CvoltonAlertLayerStub::keyBackClicked() {
    onClose(nullptr);
}
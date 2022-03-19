#include "UnregisteredProfileLayer.h"

#include <cocos2d.h>
#include <gd.h>

using namespace cocos2d;
using namespace gd;

UnregisteredProfileLayer* UnregisteredProfileLayer::create(gd::GJUserScore* score){
    auto ret = new UnregisteredProfileLayer();
    score->retain();
    ret->score = score;
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

void UnregisteredProfileLayer::onClose(cocos2d::CCObject* sender)
{
    score->release();
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void UnregisteredProfileLayer::displayProfile(int userID, std::string userName){
    gd::GJUserScore* score = gd::GJUserScore::create();
    score->setUserID(userID);
    score->setPlayerName(userName);
    UnregisteredProfileLayer* profileLayer = UnregisteredProfileLayer::create(score);
    profileLayer->show();
}

void UnregisteredProfileLayer::doSearch(int userID) {
    auto searchObject = gd::GJSearchObject::create(gd::SearchType::kSearchTypeUsersLevels, std::to_string(userID));//, contentStream.str());
    auto browserLayer = LevelBrowserLayer::scene(searchObject);

    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
    
    CCDirector::sharedDirector()->pushScene(transitionFade);
}

void UnregisteredProfileLayer::onMyLevels(CCObject* sender) {
    doSearch(score->getUserID());
}

void UnregisteredProfileLayer::onCommentHistory(CCObject* sender) {
    InfoLayer* infoLayer = InfoLayer::create(nullptr, score);
    infoLayer->show();
}

bool UnregisteredProfileLayer::init(){
    bool init = cocos2d::CCLayerColor::initWithColor({0x00, 0x00, 0x00, 0x4B});
    if(!init) return false;

    cocos2d::CCDirector* director = cocos2d::CCDirector::sharedDirector();
    director->getTouchDispatcher()->incrementForcePrio(2);

    setTouchEnabled(true);
    setKeypadEnabled(true);

    cocos2d::CCSize winSize = director->getWinSize();
    m_pLayer = cocos2d::CCLayer::create();

    this->addChild(m_pLayer);

    cocos2d::extension::CCScale9Sprite* bg = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    bg->setContentSize({ 360.0f, 180.0f });
    m_pLayer->addChild(bg, -1);
    bg->setPosition({ winSize.width / 2, winSize.height / 2 });

    auto closeButton = gd::CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"),
        this,
        menu_selector(UnregisteredProfileLayer::onClose)
    );

    m_pButtonMenu = CCMenu::create();
    m_pLayer->addChild(m_pButtonMenu, 10);
    m_pButtonMenu->addChild(closeButton);
    closeButton->setPosition({-170.5f, 79});
    closeButton->setSizeMult(1.2f);

    auto userName = CCLabelBMFont::create(score->getPlayerName().c_str(), "bigFont.fnt");
    userName->setPosition({285,227});

    m_pLayer->addChild(userName);

    auto GM = gd::GameManager::sharedState();

    gd::SimplePlayer* icon = gd::SimplePlayer::create(score->getIconID());
    icon->updatePlayerFrame(score->getIconID(), score->getIconType());
    icon->setColor(GM->colorForIdx(score->getPlayerColor1()));
    icon->setSecondColor(GM->colorForIdx(score->getPlayerColor2()));
    icon->updateColors();

    icon->setPosition({440,224});
    m_pLayer->addChild(icon);

    auto separator = CCSprite::createWithSpriteFrameName("floorLine_001.png");
    separator->setPosition({285,202});
    separator->setScaleX(0.75f);
    separator->setOpacity(100);
    m_pLayer->addChild(separator);

    auto levelsSprite = CCSprite::createWithSpriteFrameName("accountBtn_myLevels_001.png");
    levelsSprite->setScale(0.8f);
    auto levelsButton = gd::CCMenuItemSpriteExtra::create(
        levelsSprite,
        this,
        menu_selector(UnregisteredProfileLayer::onMyLevels)
    );
    m_pButtonMenu->addChild(levelsButton);
    levelsButton->setPosition({154, -62});
    //levelsButton->setScale(0.8f);
    levelsButton->setSizeMult(1.2f);

    auto levelsText = CCSprite::createWithSpriteFrameName("GJ_myLevelsTxt_001.png");
    levelsText->setScale(0.8f);
    levelsText->setPosition({104, -62});
    m_pButtonMenu->addChild(levelsText);

    auto commentsSprite = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");
    commentsSprite->setScale(0.8f);
    auto commentsButton = gd::CCMenuItemSpriteExtra::create(
        commentsSprite,
        this,
        menu_selector(UnregisteredProfileLayer::onCommentHistory)
    );
    m_pButtonMenu->addChild(commentsButton);
    commentsButton->setPosition({154, 0});
    //commentsButton->setScale(0.8f);
    commentsButton->setSizeMult(1.2f);

    std::ostringstream userIDText;
    userIDText << "User ID: " << score->getUserID() << "\nAccount ID: None";

    auto userIDTextNode = CCLabelBMFont::create(userIDText.str().c_str(), "bigFont.fnt");
    userIDTextNode->setPosition({-168, -75.5f});
    userIDTextNode->setAnchorPoint({0,0});
    userIDTextNode->setScale(0.45f);
    m_pButtonMenu->addChild(userIDTextNode);

    return true;
}
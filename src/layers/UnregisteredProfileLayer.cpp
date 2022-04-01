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
    //infoLayer->setParentFLAlert(this);
    //this->m_pTarget = infoLayer;
    infoLayer->show();
    onClose(sender);
}

bool UnregisteredProfileLayer::init(){
    bool init = createBasics({360,180}, menu_selector(UnregisteredProfileLayer::onClose));
    if(!init) return false;

    createTitle(score->getPlayerName());

    auto GM = gd::GameManager::sharedState();

    gd::SimplePlayer* icon = gd::SimplePlayer::create(score->getIconID());
    icon->updatePlayerFrame(score->getIconID(), score->getIconType());
    icon->setColor(GM->colorForIdx(score->getPlayerColor1()));
    icon->setSecondColor(GM->colorForIdx(score->getPlayerColor2()));
    icon->updateColors();

    //icon->setPosition({440,224});
    icon->setPosition({155, (alertSize.y/2) - 24});
    m_pButtonMenu->addChild(icon);

    createButton(
        "accountBtn_myLevels_001.png",
        {154, -62},
        menu_selector(UnregisteredProfileLayer::onMyLevels),
        0.8f
    );

    auto levelsText = CCSprite::createWithSpriteFrameName("GJ_myLevelsTxt_001.png");
    levelsText->setScale(0.8f);
    levelsText->setPosition({104, -62});
    m_pButtonMenu->addChild(levelsText);

    createButton(
        "GJ_chatBtn_001.png",
        {154, 0},
        menu_selector(UnregisteredProfileLayer::onCommentHistory),
        0.8f
    );

    std::ostringstream userIDText;
    userIDText << "User ID: " << score->getUserID() << "\nAccount ID: None";

    auto userIDTextNode = CCLabelBMFont::create(userIDText.str().c_str(), "bigFont.fnt");
    userIDTextNode->setPosition({-168, -75.5f});
    userIDTextNode->setAnchorPoint({0,0});
    userIDTextNode->setScale(0.45f);
    m_pButtonMenu->addChild(userIDTextNode);

    return true;
}
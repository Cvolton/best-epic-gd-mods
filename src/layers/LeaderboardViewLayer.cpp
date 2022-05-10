#include "LeaderboardViewLayer.h"
#include "DailyListView.h"
#include "JumpToPageLayer.h"
#include "../managers/CvoltonManager.h"

using namespace gd;
using namespace cocos2d;
using namespace cocos2d::extension;

LeaderboardViewLayer* LeaderboardViewLayer::create(int accountID) {
    auto ret = new LeaderboardViewLayer();
    if (ret && ret->init(accountID)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool LeaderboardViewLayer::init(int accountID) {

    auto CM = CvoltonManager::sharedState();
    CM->loadTextures();
    this->accountID = accountID;

    auto GLM = gd::GameLevelManager::sharedState();
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto backgroundSprite = CCSprite::create("game_bg_14_001.png");
    bool controllerConnected = CCApplication::sharedApplication()->getControllerConnected();

    backgroundSprite->setScale(winSize.width / backgroundSprite->getContentSize().width);
    backgroundSprite->setAnchorPoint({0, 0});
    backgroundSprite->setPosition({0,-75});
    backgroundSprite->setColor({164, 0, 255}); //purple
    backgroundSprite->setZOrder(-2);
    addChild(backgroundSprite);

    auto menu = CCMenu::create();
    addChild(menu);
    
    auto backBtn = gd::CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(LeaderboardViewLayer::onBack)
    );

    backBtn->setPosition({(-winSize.width / 2) + 25, (winSize.height / 2) - 25});
    menu->addChild(backBtn);

    /*auto menuBack = CCMenu::create();
    menuBack->addChild(backBtn);
    menuBack->setPosition({25, winSize.height - 25});

    addChild(menuBack);*/

    setTouchEnabled(true);
    setKeypadEnabled(true);

    scores = CCArray::create();
    scores->retain();

    //corners
    auto cornerBL = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    cornerBL->setPosition({0,0});
    cornerBL->setAnchorPoint({0,0});
    addChild(cornerBL, -1);

    auto cornerBR = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    cornerBR->setPosition({winSize.width,0});
    cornerBR->setAnchorPoint({0,0});
    cornerBR->setRotation(270);
    addChild(cornerBR, -1);

    loadPage();
    startLoading();

    return true;
}

void LeaderboardViewLayer::startLoading(){
    CCHttpRequest* request = new CCHttpRequest;
    request->setUrl("http://www.boomlings.com/database/getGJScores20.php");
    request->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
    request->setResponseCallback(this, httpresponse_selector(LeaderboardViewLayer::onLoadFinished));
    //TODO: make this slightly more dynamic
    auto postData = CCString::createWithFormat("gameVersion=21&binaryVersion=35&gdw=0&accountID=%i&type=relative&secret=Wmfd2893gb7", accountID);
    request->setRequestData(
        postData->getCString(), strlen(postData->getCString())
    );
    CCHttpClient::getInstance()->send(request);
    //request->release();
}

void LeaderboardViewLayer::onLoadFinished(CCHttpClient* client, CCHttpResponse* response){
    if(!(response->isSucceed())) return;

    std::vector<char>* responseData = response->getResponseData();
    std::string responseString(responseData->begin(), responseData->end());

    generateScores(responseString);

    //auto test = CCLabelBMFont::create(responseString.c_str(), "goldFont.fnt");
    //addChild(test);

    //scores = GameLevelManager::sharedState()->createAndGetScores(responseString, 0);
    //scores->retain();
    loadPage();
    //FLAlertLayer::create(nullptr, "User Info", "OK", nullptr, 300, responseString.c_str())->show();
    //FLAlertLayer::create(nullptr, "User Info", "OK", nullptr, 300, "mogus")->show();
}

void LeaderboardViewLayer::loadPage(){

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if(listLayer != nullptr) listLayer->removeFromParentAndCleanup(true);

    leaderboardView = LeaderboardListView::create(scores, 356.f, 220.f);
    listLayer = GJListLayer::create(leaderboardView, "Scores", {191, 114, 62, 255}, 356.f, 220.f);
    listLayer->setPosition(winSize / 2 - listLayer->getScaledContentSize() / 2 - CCPoint(0,5));
    addChild(listLayer);
}

void LeaderboardViewLayer::keyBackClicked() {
    setTouchEnabled(false);
    setKeypadEnabled(false);
    scores->release();
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}


void LeaderboardViewLayer::onBack(CCObject* object) {
    keyBackClicked();
}

CCScene* LeaderboardViewLayer::scene(int accountID) {
    auto layer = LeaderboardViewLayer::create(accountID);
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

void LeaderboardViewLayer::generateScores(const std::string& response){

    std::stringstream responseStream(response);
    std::string current;

    while(getline(responseStream, current, '|')){
        
        auto CM = CvoltonManager::sharedState();

        auto score = GJUserScore::create(
            CM->responseToDict(current)
        );

        scores->addObject(score);
    }

}
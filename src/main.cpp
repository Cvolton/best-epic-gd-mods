#define WIN32_LEAN_AND_MEAN
#include "mapped-hooks.hpp"
#include "Windows.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cocos2d.h>
#include <gd.h>
#include <typeinfo>
#include <cstring>
#include <deque>
#include "utils.hpp"

#include "layers/UnregisteredProfileLayer.h"
#include "layers/JumpToPageLayer.h"
#include "layers/ExtendedLevelInfo.h"
#include "layers/CustomCreatorLayer.h"
#include "layers/DailyViewLayer.h"
#include "managers/CvoltonManager.h"

using namespace cocos2d;
using namespace gd;

const int cvoltonID = 6330800;
const int commentPageBtnTag = 863390891;
const int questBtnExMarkTag = 863390892;
const int randomBtnTag = 863390893;

class CustomLevelSearchLayer : public gd::FLAlertLayer {
    gd::GJGameLevel* level;
public:
    static CustomLevelSearchLayer* create(gd::GJGameLevel* level){
        auto ret = new CustomLevelSearchLayer();
        level->retain();
        ret->level = level;
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

    void onClose(cocos2d::CCObject* sender)
    {
        level->release();
        setKeypadEnabled(false);
        removeFromParentAndCleanup(true);
    }

    void onSimilar(CCObject* sender) {
        auto searchObject = gd::GJSearchObject::create(gd::SearchType::kSearchTypeSimilar, std::to_string(level->levelID));
        auto browserLayer = LevelBrowserLayer::scene(searchObject);

        auto transitionFade = CCTransitionFade::create(0.5, browserLayer);

        CCDirector::sharedDirector()->pushScene(transitionFade);
    }

    void onSong(CCObject* sender) {
        bool customSong = level->songID > 0;
        int songID = customSong ? level->songID : level->audioTrack;

        auto searchObject = gd::GJSearchObject::create(gd::SearchType::kSearchTypeMostLiked);
        searchObject->setSongInfo(customSong, songID);

        auto browserLayer = LevelBrowserLayer::scene(searchObject);

        auto transitionFade = CCTransitionFade::create(0.5, browserLayer);

        CCDirector::sharedDirector()->pushScene(transitionFade);
    }

    void onNeighbors(CCObject* sender) {
        std::ostringstream query;
        bool addSeparator = false;
        for(int i = (level->levelID) - 50; i < ((level->levelID) + 50); i++){
            if(addSeparator) query << ",";
            query << i;
            addSeparator = true;
        }

        auto searchObject = gd::GJSearchObject::create(gd::SearchType::kSearchType19, query.str());
        auto browserLayer = LevelBrowserLayer::scene(searchObject);
        auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
        CCDirector::sharedDirector()->pushScene(transitionFade);
    }

    bool init(){
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
            menu_selector(CustomLevelSearchLayer::onClose)
        );

        m_pButtonMenu = CCMenu::create();
        m_pLayer->addChild(m_pButtonMenu, 10);
        m_pButtonMenu->addChild(closeButton);
        closeButton->setPosition({-170.5f, 79});
        closeButton->setSizeMult(1.2f);

        auto searchTitle = CCLabelBMFont::create("Search", "bigFont.fnt");
        searchTitle->setPosition({0,66});

        m_pButtonMenu->addChild(searchTitle);

        auto separator = CCSprite::createWithSpriteFrameName("floorLine_001.png");
        separator->setPosition({285,202});
        separator->setScaleX(0.75f);
        separator->setOpacity(100);
        m_pLayer->addChild(separator);

        /*auto buttonSprite = gd::ButtonSprite::create("Neighbors", (int)(120*0.6), true, "bigFont.fnt", "GJ_button_01.png", 44*0.6f, 0.6f);
        auto buttonButton = gd::CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(CustomLevelSearchLayer::onNeighbors)
        );
        buttonButton->setSizeMult(1.2f);
        buttonButton->setPosition({0,0});
        m_pButtonMenu->addChild(buttonButton);*/

        /*auto neighborButton = createButton(m_pButtonMenu, "Neighbors", menu_selector(CustomLevelSearchLayer::onNeighbors), 0, 4, (int)(120*0.6), 44*0.6f, 0.6f);
        auto similarButton = createButton(m_pButtonMenu, "Similar", menu_selector(CustomLevelSearchLayer::onSimilar), 0, -48, (int)(120*0.6), 44*0.6f, 0.6f);*/

        auto similarButton = createButton(m_pButtonMenu, "Similar", menu_selector(CustomLevelSearchLayer::onSimilar), -75, 8, (int)(120*0.6), 44*0.6f, 0.6f);
        auto neighborButton = createButton(m_pButtonMenu, "Neighbors", menu_selector(CustomLevelSearchLayer::onNeighbors), 75, 8, (int)(120*0.6), 44*0.6f, 0.6f);
        auto songButton = createButton(m_pButtonMenu, "Same song", menu_selector(CustomLevelSearchLayer::onSong), 0, -48, (int)(120*0.6), 44*0.6f, 0.6f);

        return true;
    }

    gd::CCMenuItemSpriteExtra* createButton(CCNode* menu, const char* text, cocos2d::SEL_MenuHandler handler, float x, float y, int width, float height, float scale){
        auto buttonSprite = gd::ButtonSprite::create(text, width, true, "bigFont.fnt", "GJ_button_01.png", height, scale);
        auto buttonButton = gd::CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            handler
        );
        buttonButton->setSizeMult(1.2f);
        buttonButton->setPosition({x,y});
        menu->addChild(buttonButton);

        return buttonButton;
    }

};

class StaticStringHelper {
public:
    static std::string getFriendRequestType(int type){
        switch(type){
            case 0: return "Enabled";
            case 1: return "Disabled";
            default: return "Unknown";
        }
    }

    static std::string getMessageType(int type){
        switch(type){
            case 0: return "Enabled";
            case 1: return "Friends Only";
            case 2: return "Disabled";
            default: return "Unknown";
        }
    }

    static std::string getIconType(int type){
        switch(type){
            case 0: return "Cube";
            case 1: return "Ship";
            case 2: return "Ball";
            case 3: return "UFO";
            case 4: return "Wave";
            case 5: return "Robot";
            case 6: return "Spider";
            case 7: return "Swingcopter";
            default: return "None";
        }
    }
};

class GamingButton {
public:
    void onProfilePage(CCObject* sender){
        auto layer = cast<CommentCell*>(this);
        layer->comment->m_pUserScore->setUserID(layer->comment->m_nAuthorPlayerID);
        UnregisteredProfileLayer* profileLayer = UnregisteredProfileLayer::create(layer->comment->m_pUserScore, layer->getParent()->getParent()->getParent()->getParent()->getParent()->getParent());
        profileLayer->show();
    }

    void onProfilePageInfo(CCObject* sender){
        auto layer = cast<ProfilePage*>(this);

        auto score = layer->score;
        auto GM = GameManager::sharedState();

        std::ostringstream contentStream;
        if(score->getUserID() == GM->m_nPlayerUserID) contentStream << "User ID: " << score->getUserID()
            << "\nAccount ID: " << score->getAccountID()
            << "\n\n";//Leaderboard Icon: " << StaticStringHelper::getIconType(score->getIconType()) (is not sent)
        contentStream << "Friend Requests: " << StaticStringHelper::getFriendRequestType(score->getFriendStatus())
            << "\nPrivate Messages: " << StaticStringHelper::getMessageType(score->getMessageState())
            << "\nComment History: " << StaticStringHelper::getMessageType(score->getCommentHistoryStatus());
        if(score->getUserID() == GM->m_nPlayerUserID) contentStream << "\n\nBootups: " << GM->m_nBootups;

        //if(score->getUserID() == cvoltonID) contentStream << "\n\nThis user is epic!";

        gd::FLAlertLayer::create(nullptr, "User Info", "OK", nullptr, contentStream.str())->show();
    }

    void onProfilePageReload(CCObject* sender){
        //TODO: improve this™️
        auto self = cast<ProfilePage*>(this);

        auto GLM = GameLevelManager::sharedState();

        auto score = self->score;
        GLM->resetStoredUserInfo(score->getAccountID());
        GLM->resetAccountComments(score->getAccountID());

        GLM->getGJUserInfo(self->something);
    }

    void onCustomSearch(CCObject* sender){
        auto self = cast<InfoLayer*>(this);

        auto level = self->m_pLevel;

        CustomLevelSearchLayer* layer = CustomLevelSearchLayer::create(level);
        layer->show();
    }

    void onLevelInfoNoLoad(CCObject* sender){
        auto layer = cast<CommentCell*>(this);

        auto level = gd::GJGameLevel::create();
        level->levelID_rand = layer->comment->m_nLevelID;
        level->levelID_seed = 0;
        level->levelID = layer->comment->m_nLevelID;

        InfoLayer* infoLayer = InfoLayer::create(level, nullptr);
        infoLayer->setParentFLAlert(layer->getParent()->getParent()->getParent()->getParent()->getParent()->getParent());
        infoLayer->show();

        //gd::FLAlertLayer::create(nullptr, "Level Stats", "OK", nullptr, std::to_string(layer->comment->m_nLevelID))->show();
    }

    void onCustomCreatorLayer(CCObject* sender){
        auto browserLayer = CustomCreatorLayer::scene();
        auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
        CCDirector::sharedDirector()->pushScene(transitionFade);
    }

    void onJumpToPageLayer(CCObject* sender){
        auto layer = cast<InfoLayer*>(this);
        JumpToPageLayer* jumpLayer = JumpToPageLayer::create(layer);
        jumpLayer->show();
    }

    void onLeaderboardDaily(CCObject* sender){
        auto self = cast<LevelLeaderboard*>(this);
        auto level = gd::GJGameLevel::create();
        level->levelID_rand = self->level->levelID_rand;
        level->levelID_seed = self->level->levelID_seed;
        level->levelID = self->level->levelID;
        level->levelName = self->level->levelName;
        level->dailyID_rand = !self->level->dailyID;
        level->dailyID_seed = 0;
        level->dailyID = !self->level->dailyID;
        //GameLevelManager::sharedState()->m_pTimerDict->setObject(CCString::create("0"), "upd_11");
        //GameLevelManager::sharedState()->m_pTimerDict->writeToFile("c:/users/brabe/documents/test.plist");
        GameLevelManager::sharedState()->m_pTimerDict->removeAllObjects(); //this is equivalent to GLM::resetAllTimers() which I suspect is inlined on windows but i didn't bother checking
        //auto obj = dynamic_cast<CCString*>(GameLevelManager::sharedState()->m_pTimerDict->objectForKey("upd_ll"));
        //gd::FLAlertLayer::create(nullptr, "Level Stats", "OK", nullptr, std::to_string(GameLevelManager::sharedState()->m_weeklyTimeLeft))->show();
        auto newLeaderboard = LevelLeaderboard::create(level, self->leaderboardType);
        //newLeaderboard->show();
        newLeaderboard->onUpdate(sender);
        CCDirector::sharedDirector()->getRunningScene()->addChild(newLeaderboard);
        self->onClose(sender);
        /*self->level->release();
        level->retain();
        self->level = level;
        self->onUpdate(sender);*/
    }

    void onLevelBrowserRandom(CCObject* sender){
        auto layer = cast<LevelBrowserLayer*>(this);

        if(layer->searchObject == nullptr) return;

        int pageMax = layer->total / 10;
        //int pageToLoad = std::rand() % pageMax;

        int pageToLoad = CvoltonManager::sharedState()->randomNumber(0, pageMax);

        layer->searchObject->m_nPage = pageToLoad;
        layer->loadPage(layer->searchObject);
    }

    void onDailyHistory(CCObject* sender){
        auto self = cast<DailyLevelPage*>(this);
        auto layer = DailyViewLayer::scene(self->isWeekly);
        auto transitionFade = CCTransitionFade::create(0.5, layer);
        CCDirector::sharedDirector()->pushScene(transitionFade);
        //CCDirector::sharedDirector()->getRunningScene()->addChild(layer);
    }

    void onLeaderboardRefresh(CCObject* sender){
        auto GLM = gd::GameLevelManager::sharedState();
        GLM->updateUserScore();
        GLM->storedLevels->removeObjectForKey("leaderboard_creator");
        GLM->storedLevels->removeObjectForKey("leaderboard_friends");
        GLM->storedLevels->removeObjectForKey("leaderboard_global");
        GLM->storedLevels->removeObjectForKey("leaderboard_top");

        auto layer = LeaderboardsLayer::create(GLM->leaderboardState);
        auto scene = CCScene::create();
        scene->addChild(layer);
        CCDirector::sharedDirector()->replaceScene(scene);
    }

};

bool __fastcall InfoLayer_init(CCLayer* self, void* a, gd::GJGameLevel* level, void* c) {
    if (!MHook::getOriginal(InfoLayer_init)(self, a, level, c)) return false;

    auto layer = cast<CCLayer*>(self->getChildren()->objectAtIndex(0));
    CCMenu* menu = cast<CCMenu*>(layer->getChildren()->objectAtIndex(1));

    gd::CCMenuItemSpriteExtra* playerName = cast<gd::CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(0));
    playerName->setEnabled(true);

    auto buttonSprite = gd::ButtonSprite::create("1", 12, true, "bigFont.fnt", "GJ_button_02.png", 25, 0.4f);
    auto buttonButton = gd::CCMenuItemSpriteExtra::create(
        buttonSprite,
        self,
        menu_selector(GamingButton::onJumpToPageLayer)
    );
    buttonButton->setSizeMult(1.2f);
    buttonButton->setPosition({195,34});
    buttonButton->setTag(commentPageBtnTag);
    menu->addChild(buttonButton);

    if(level == nullptr) return true;

    auto searchSprite = CCSprite::createWithSpriteFrameName("gj_findBtn_001.png");
    searchSprite->setScale(0.8f);
    auto searchButton = gd::CCMenuItemSpriteExtra::create(
        searchSprite,
        self,
        menu_selector(GamingButton::onCustomSearch)
    );
    menu->addChild(searchButton);
    searchButton->setPosition({195, 68});
    searchButton->setSizeMult(1.2f);

    /*if(gd::GameManager::sharedState()->getGameVariable("0089")){
        //createButton(self, menu, "Similar", menu_selector(GamingButton::onSimilar), -118, 97, (int)(90*0.45), 44*0.45f);
        auto searchSprite = CCSprite::createWithSpriteFrameName("gj_findBtn_001.png");
        searchSprite->setScale(0.65f);
        auto searchButton = gd::CCMenuItemSpriteExtra::create(
            searchSprite,
            self,
            menu_selector(GamingButton::onCustomSearch)
        );
        menu->addChild(searchButton);
        searchButton->setPosition({-162, 128});
        searchButton->setSizeMult(1.2f);
    }else{
        //createButton(self, menu, "Search", menu_selector(GamingButton::onCustomSearch), -143, 97, (int)(90*0.45), 44*0.45f);

        
    }*/


    return true;
}

bool __fastcall InfoLayer_onMore(InfoLayer* self, void* a, CCObject* b) {
    if(self->m_pLevel->accountID == 0) {
        UnregisteredProfileLayer::displayProfile(self->m_pLevel->userID, self->m_pLevel->userName);
        return true;
    }

    if (!MHook::getOriginal(InfoLayer_onMore)(self, a, b)) return false;

    return true;
}

bool __fastcall LevelInfoLayer_setupProgressBars(CCLayer* self) {
    if (!MHook::getOriginal(LevelInfoLayer_setupProgressBars)(self)) return false;

    CCMenu* menu = cast<CCMenu*>(self->getChildren()->objectAtIndex(2));
    gd::CCMenuItemSpriteExtra* playerName = cast<gd::CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(0));
    playerName->setEnabled(true);


    return true;
}

bool __fastcall LevelInfoLayer_onViewProfile(LevelInfoLayer* self, void* a, CCObject* b) {

    if(self->level->accountID == 0) {
        UnregisteredProfileLayer::displayProfile(self->level->userID, self->level->userName);
        return true;
    }

    if (!MHook::getOriginal(LevelInfoLayer_onViewProfile)(self, a, b)) return false;

    return true;
}

bool __fastcall LevelCell_onViewProfile(LevelCell* self, void* a, CCObject* b) {

    if(self->level->accountID == 0) {
        UnregisteredProfileLayer::displayProfile(self->level->userID, self->level->userName);
        return true;
    }

    if (!MHook::getOriginal(LevelCell_onViewProfile)(self, a, b)) return false;

    return true;
}

void __fastcall LevelCell_loadCustomLevelCell(LevelCell* self) {
    MHook::getOriginal(LevelCell_loadCustomLevelCell)(self);
    
    auto layer = cast<CCLayer*>(self->getChildren()->objectAtIndex(1));
    for(unsigned int i = 0; i < layer->getChildrenCount(); i++){
        auto menu = dynamic_cast<CCMenu*>(layer->getChildren()->objectAtIndex(i));
        if(menu != nullptr){
            auto playerName = cast<gd::CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(1));
            playerName->setEnabled(true);

            std::ostringstream idText;
            idText << "#" << self->level->levelID;
            auto idTextNode = CCLabelBMFont::create(idText.str().c_str(), "chatFont.fnt");
            idTextNode->setPosition({33,33});
            idTextNode->setAnchorPoint({1,0});
            idTextNode->setScale(0.6f);
            idTextNode->setColor({51,51,51});
            idTextNode->setOpacity(152);
            menu->addChild(idTextNode);
            if(self->level->dailyID > 0){
                idTextNode->setColor({255,255,255});
                idTextNode->setOpacity(200);

                const int maxDaily = 100000;

                std::ostringstream dailyText;
                dailyText << ((self->level->dailyID >= maxDaily) ? "Weekly" : "Daily") << " #" << (self->level->dailyID % maxDaily);
                auto dailyTextNode = CCLabelBMFont::create(dailyText.str().c_str(), "chatFont.fnt");
                dailyTextNode->setPosition({33,43});
                dailyTextNode->setAnchorPoint({1,0});
                dailyTextNode->setScale(0.6f);
                dailyTextNode->setColor({255,255,255});
                dailyTextNode->setOpacity(200);
                menu->addChild(dailyTextNode);

            }

            break;
        }
    }
}

void __fastcall ProfilePage_loadPageFromUserInfo(ProfilePage* self, void* a, gd::GJUserScore* a2){
    MHook::getOriginal(ProfilePage_loadPageFromUserInfo)(self, a, a2);

    auto layer = cast<CCLayer*>(self->getChildren()->objectAtIndex(0));

    auto menu = self->m_pButtonMenu;

    auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
    infoSprite->setScale(0.7f);
    auto infoButton = gd::CCMenuItemSpriteExtra::create(
        infoSprite,
        self,
        menu_selector(GamingButton::onProfilePageInfo)
    );
    menu->addChild(infoButton);
    infoButton->setPosition({16, -135});
    //infoButton->setScale(0.8f);
    infoButton->setSizeMult(1.2f);

    self->objectsInMenu->addObject(infoButton);

    if(a2->getUserID() != GameManager::sharedState()->m_nPlayerUserID){
        auto refreshSprite = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
        //refreshSprite->setScale(0.7f);
        auto refreshButton = gd::CCMenuItemSpriteExtra::create(
            refreshSprite,
            self,
            menu_selector(GamingButton::onProfilePageReload)
        );
        menu->addChild(refreshButton);
        refreshButton->setPosition({0, -269});
        //refreshButton->setPosition({16,-175});
        //refreshButton->setScale(0.8f);
        refreshButton->setSizeMult(1.2f);

        self->objectsInMenu->addObject(refreshButton);

        auto userIDNode = CCLabelBMFont::create(CCString::createWithFormat("User ID: %i", a2->getUserID())->getCString(), "chatFont.fnt");
        userIDNode->setPosition({38,-248});
        userIDNode->setAnchorPoint({0,1});
        userIDNode->setScale(0.6f);
        userIDNode->setColor({255,255,255});
        userIDNode->setOpacity(220);
        menu->addChild(userIDNode);
        self->objectsInMenu->addObject(userIDNode);

        auto accountIDNode = CCLabelBMFont::create(CCString::createWithFormat("Account ID: %i", a2->getAccountID())->getCString(), "chatFont.fnt");
        accountIDNode->setPosition({38,-258});
        accountIDNode->setAnchorPoint({0,1});
        accountIDNode->setScale(0.6f);
        accountIDNode->setColor({255,255,255});
        accountIDNode->setOpacity(220);
        menu->addChild(accountIDNode);
        self->objectsInMenu->addObject(accountIDNode);
    }

}

bool __fastcall ProfilePage_init(ProfilePage* self, void* a, int id, bool a2){
    if(!MHook::getOriginal(ProfilePage_init)(self, a, id, a2)) return false;

    auto layer = cast<CCLayer*>(self->getChildren()->objectAtIndex(0));

    //auto menu = self->m_pButtonMenu;

    for(unsigned int i = 0; i < self->m_pButtonMenu->getChildrenCount(); i++){
        CCNode* node = dynamic_cast<CCNode*>(self->m_pButtonMenu->getChildren()->objectAtIndex(i));
        if(node != nullptr && node->getPositionX() == 12 && node->getPositionY() == -258) node->setPosition(16, -224); //node->setPosition({52, -258}); // //
    }

    CCNode* followTxt = dynamic_cast<CCNode*>(layer->getChildren()->objectAtIndex(6));
    if(followTxt->getPositionY() == 35) followTxt->setVisible(false); //followTxt->setPositionX(followTxt->getPositionX() + 40);

    /*auto menu = CCMenu::create();
    menu->setPosition({ 0, 0 });
    menu->setZOrder(1000);
    for (int i = 0; i < layer->getChildrenCount(); i++) {
        auto text = CCLabelBMFont::create(std::to_string(i).c_str(), "bigFont.fnt");
        auto node = (CCNode*)layer->getChildren()->objectAtIndex(i);
        text->setAnchorPoint(node->getAnchorPoint());
        text->setScale(0.5f);
        text->setPosition(node->getPosition());
        text->setZOrder(1000);
        menu->addChild(text);
    }
    layer->addChild(menu);*/

    return true;
}

/*void* __fastcall ProfilePage_getUserInfoFailed(ProfilePage* self, void* a, gd::GJUserScore* a2){
    void* returnValue = MHook::getOriginal(ProfilePage_getUserInfoFailed)(self, a, a2);
    gd::GJUserScore* score = gd::GJUserScore::create();
    score->setPlayerName("amongus");
    score->setPlayerCube(6);
    //self->loadPageFromUserInfo(score);
    return returnValue;
}*/

void __fastcall CommentCell_loadFromComment(CommentCell* self, void* a, GJComment* b) {
    MHook::getOriginal(CommentCell_loadFromComment)(self, a, b);

    //if(b->m_bHasLevelID) return;

    auto layer = cast<CCLayer*>(self->getChildren()->objectAtIndex(1));

    CCMenu* menu = nullptr;

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    bool smallCommentsMode = gd::GameManager::sharedState()->getGameVariable("0088");

    for(unsigned int i = 0; i < layer->getChildrenCount(); i++){
        menu = dynamic_cast<CCMenu*>(layer->getChildren()->objectAtIndex(i));
        if(menu != nullptr) break;
    }

    if(menu == nullptr) return;

    if(!(b->m_bHasLevelID)){

        if(b->m_nAuthorAccountID != 0) return;

        auto originalNameNode = cast<CCLabelBMFont*>(layer->getChildren()->objectAtIndex(2));
        //std::string userName = originalNameNode->getString();
        
        if(strlen(originalNameNode->getString()) == 0){
            std::stringstream contentStream;
            contentStream << "- (ID: " << b->m_nAuthorPlayerID << ")";
            originalNameNode->setString(contentStream.str().c_str());
        }
        layer->removeChild(originalNameNode);
        //originalNameNode->setAnchorPoint({0,0});

        //auto newPlayerName = CCLabelBMFont::create(userName.c_str(), "goldFont.fnt");

        auto buttonButton = gd::CCMenuItemSpriteExtra::create(
            originalNameNode,
            self,
            menu_selector(GamingButton::onProfilePage)
        );
        buttonButton->setSizeMult(1.2f);
        buttonButton->setPosition(37 - (winSize.width / 2), smallCommentsMode ? 18.5f - (winSize.height / 2) : 50.5f - (winSize.height / 2) );
        buttonButton->setAnchorPoint({0.1f,0});
        buttonButton->setEnabled(true);
        menu->addChild(buttonButton);

    }else{

        auto commentsSprite = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");
        commentsSprite->setScale(smallCommentsMode ? 0.35f : 0.5f);
        auto commentsButton = gd::CCMenuItemSpriteExtra::create(
            commentsSprite,
            self,
            menu_selector(GamingButton::onLevelInfoNoLoad)
        );
        menu->addChild(commentsButton);
        //commentsButton->setPosition({-79,-136});
        CCPoint smallPosition({206 - (winSize.width / 2), 24.5f - (winSize.height / 2) });
        CCPoint largePosition({154 - (winSize.width / 2), 60 - (winSize.height / 2) });
        commentsButton->setPosition(smallCommentsMode ? smallPosition : largePosition);
        //commentsButton->setScale(0.8f);
        commentsButton->setSizeMult(1.2f);

    }

}

void __fastcall LevelInfoLayer_onLevelInfo(LevelInfoLayer* self, void* a, CCObject* sender) {
    ExtendedLevelInfo::showProgressDialog(self->level);
}

void __fastcall EditLevelLayer_onLevelInfo(EditLevelLayer* self, void* a, CCObject* sender) {
    ExtendedLevelInfo::showProgressDialog(self->m_pLevel);
}

/*void __fastcall LevelPage_onInfo(LevelPage* self, void* a, CCObject* sender) {
    ExtendedLevelInfo::showProgressDialog(self->m_pLevel);
}*/

void __fastcall InfoLayer_onLevelInfo(InfoLayer* self, void* a, CCObject* sender) {
    auto CM = CvoltonManager::sharedState();
    if(CM->getOption("no_level_info")){
        MHook::getOriginal(InfoLayer_onLevelInfo)(self, a, sender);
        return;
    }

    auto level = self->m_pLevel;

    ExtendedLevelInfo* layer = ExtendedLevelInfo::create(level);
    layer->show();

}

std::string* __fastcall GameLevelManager_userNameForUserID(void* a, void* b, std::string* userName, int userID) {
    auto original = MHook::getOriginal(GameLevelManager_userNameForUserID)(a, b, userName, userID);
    //if i don't clone the string then unmodified strings have a slight chance of crashing the game and i have no idea why
    //auto newString = std::string(*userName);

    auto CM = CvoltonManager::sharedState();

    if(!CM->getOption("no_green_user") && (*userName == "" || *userName == "-")){
        auto CM = CvoltonManager::sharedState();
        *userName = CM->getUserName(userID);
    }
    //std::cout << userID << " " << newString << std::endl;

    //*userName = newString;

    /*switch(userID){
        case 248868:
            *userName = "roadbose";
            break;
        case 35:
            *userName = "lich";
            break;
        case 32:
            *userName = "chad";
            break;
        case 65:
            *userName = "Carbonelite";
            break;
        case 90:
            *userName = "angrybirds94";
            break;
        case 1732:
            *userName = "darnoc2";
            break;
        case 46587:
            *userName = "diamond";
            break;
    }*/

    return original;
}

void __fastcall InfoLayer_setupCommentsBrowser(InfoLayer* self, void* a, CCArray* a3) {
    MHook::getOriginal(InfoLayer_setupCommentsBrowser)(self, a, a3);

    if(self->m_nTotalItems >= 999) self->m_pNextPageBtn->setVisible(true);
}

void __fastcall InfoLayer_loadPage(InfoLayer* self, void* a, int page, bool reload) {
    MHook::getOriginal(InfoLayer_loadPage)(self, a, page, reload);

    CCMenu* menu = cast<CCMenu*>(self->m_pCommentsBtn->getParent());

    std::cout << menu->getChildrenCount() << std::endl;

    for(unsigned int i = 0; i < menu->getChildrenCount(); i++){
        auto commentBtn = cast<CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(i));
        std::cout << i << std::endl;
        if(commentBtn == nullptr) continue;

        std::cout << commentBtn->getTag() << std::endl;


        if(commentBtn->getTag() == commentPageBtnTag){

            auto commentBtnSprite = cast<ButtonSprite*>(commentBtn->getChildren()->objectAtIndex(0));
            if(commentBtnSprite == nullptr) continue;

            commentBtnSprite->setString(std::to_string(page+1).c_str());
        }
    }
}

void __fastcall LevelBrowserLayer_updateLevelsLabel(LevelBrowserLayer* self, void* a) {
    MHook::getOriginal(LevelBrowserLayer_updateLevelsLabel)(self, a);

    if(self->total == 9999) self->nextBtn->setVisible(true);

    CCMenu* menu = cast<CCMenu*>(self->nextBtn->getParent());
    for(unsigned int i = 0; i < menu->getChildrenCount(); i++){
        if(menu->getChildren()->objectAtIndex(i) != nullptr && menu->getChildren()->objectAtIndex(i)->getTag() == randomBtnTag) return;
    }

    CvoltonManager::sharedState()->loadTextures();

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    bool isLocal = self->searchObject->m_nScreenID == SearchType::kSearchTypeMyLevels || self->searchObject->m_nScreenID == SearchType::kSearchTypeSavedLevels;

    auto randomSprite = CCSprite::createWithSpriteFrameName("BI_randomBtn_001.png");
    randomSprite->setScale(0.9f);
    auto randomBtn = gd::CCMenuItemSpriteExtra::create(
        randomSprite,
        self,
        menu_selector(GamingButton::onLevelBrowserRandom)
    );
    randomBtn->setPosition({ (winSize.width / 2) - 23, (winSize.height / 2) - 72});
    randomBtn->setTag(randomBtnTag);
    if(isLocal) randomBtn->setPosition({(winSize.width / 2) - 58, (winSize.height / 2) - 38.5f});

    menu->addChild(randomBtn);
}

/*bool __fastcall LevelBrowserLayer_init(LevelBrowserLayer* self, void* a, GJSearchObject* searchObject) {
    if(!MHook::getOriginal(LevelBrowserLayer_init)(self, a, searchObject)) return false;

    //TODO: load the textures in a proper place
    

    return true;
}*/

void showQuestExclamationMark(CCLayer* creator){
    //if(creator == nullptr || creator->getChildrenCount() < 2) return;
    auto menu = dynamic_cast<CCMenu*>(creator->getChildren()->objectAtIndex(1));
    auto GSM = GameStatsManager::sharedState();

    bool showExclamation = false;
    for(int i = 1; i < 4; i++){
        GJChallengeItem* item = GSM->getChallenge(i);
        if(item != nullptr && item->m_bCanClaim) showExclamation = true;
    }

    if(menu == nullptr || !showExclamation) return;

    auto questBtn = static_cast<CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(3));
    if(questBtn == nullptr) return;

    auto existingNode = questBtn->getChildByTag(questBtnExMarkTag);
    if(existingNode != nullptr){
        existingNode->setVisible(true);
        return;
    }

    auto exMark = CCSprite::createWithSpriteFrameName("exMark_001.png");
    exMark->setPosition({16.5,75});
    exMark->setScale(0.7f);
    exMark->setTag(questBtnExMarkTag);
    questBtn->addChild(exMark);
}

bool __fastcall CreatorLayer_init(CCLayer* self) {
    if(!MHook::getOriginal(CreatorLayer_init)(self)) return false;

    //update check
    auto CM = CvoltonManager::sharedState();
    CM->doUpdateCheck();
    CM->setActiveCreator(self);

    //betterinfo btn
    auto menu = cast<CCMenu*>(self->getChildren()->objectAtIndex(1));

    auto door = cast<CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(12));

    auto buttonSprite = gd::ButtonSprite::create("Better\nInfo", (int)(100*0.45), true, "bigFont.fnt", "GJ_button_01.png", 120*0.45f, 0.45f);
    auto buttonButton = gd::CCMenuItemSpriteExtra::create(
        buttonSprite,
        self,
        menu_selector(GamingButton::onCustomCreatorLayer)
    );
    buttonButton->setSizeMult(1.2f);
    buttonButton->setPosition({door->getPositionX() - 2,0});
    menu->addChild(buttonButton);

    //showQuestExclamationMark(self);

    return true;
}

void __fastcall CreatorLayer_onChallenge(CCLayer* self, void* a, CCMenuItemSpriteExtra* sender) {
    for(unsigned int i = 0; i < sender->getChildrenCount(); i++){
        auto child = dynamic_cast<CCSprite*>(sender->getChildren()->objectAtIndex(i));
        if(child != nullptr && child->getTag() == questBtnExMarkTag) child->setVisible(false);
    }
    MHook::getOriginal(CreatorLayer_onChallenge)(self, a, sender);
}

void __fastcall CreatorLayer_onBack(CCLayer* self, void* a, CCMenuItemSpriteExtra* sender) {
    auto CM = CvoltonManager::sharedState();
    CM->setActiveCreator(nullptr);

    MHook::getOriginal(CreatorLayer_onBack)(self, a, sender);
}

void _fastcall CreatorLayer_sceneWillResume(uint8_t* self){
    MHook::getOriginal(CreatorLayer_sceneWillResume)(self);

    showQuestExclamationMark(reinterpret_cast<CCLayer*>(self - sizeof(CCLayer)));
}

void __fastcall DailyLevelPage_updateTimers(DailyLevelPage* self, void* a, float something) {
    MHook::getOriginal(DailyLevelPage_updateTimers)(self, a, something);

    CCLayer* layer = cast<CCLayer*>(self->getChildren()->objectAtIndex(0));

    if(layer->getChildrenCount() > 11) return;

    auto GM = GameLevelManager::sharedState();
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    std::ostringstream currentDaily;
    currentDaily << "Current: #" << ((self->isWeekly) ? GM->m_weeklyID % 100000 : GM->m_dailyID);
    auto currentDailyNode = CCLabelBMFont::create(currentDaily.str().c_str(), "chatFont.fnt");
    currentDailyNode->setPosition({(winSize.width / 2) + 183, (winSize.height / 2) + 51});
    currentDailyNode->setAnchorPoint({1,0});
    currentDailyNode->setScale(0.6f);
    currentDailyNode->setColor({255,255,255});
    currentDailyNode->setOpacity(200);
    layer->addChild(currentDailyNode);
}

bool __fastcall DailyLevelPage_init(DailyLevelPage* self, void* a, bool isWeekly) { //type is usually an enum but i dont have that rn
    if(!MHook::getOriginal(DailyLevelPage_init)(self, a, isWeekly)) return false;

    CvoltonManager::sharedState()->loadTextures();

    CCMenu* menu = self->m_pButtonMenu;

    auto historySprite = CCSprite::createWithSpriteFrameName("BI_historyBtn_001.png");
    historySprite->setScale(0.8f);
    auto historyBtn = gd::CCMenuItemSpriteExtra::create(
        historySprite,
        self,
        menu_selector(GamingButton::onDailyHistory)
    );
    historyBtn->setPosition({13, -247});
    menu->addChild(historyBtn);

    return true;
}

bool __fastcall LevelLeaderboard_init(LevelLeaderboard* self, void* a, GJGameLevel* level, int type) { //type is usually an enum but i dont have that rn
    if(!MHook::getOriginal(LevelLeaderboard_init)(self, a, level, type)) return false;

    CCLayer* layer = cast<CCLayer*>(self->getChildren()->objectAtIndex(0));

    //if(layer->getChildrenCount() > 11) return;
    CCMenu* menu = cast<CCMenu*>(layer->getChildren()->objectAtIndex(5));

    auto buttonSprite = gd::ButtonSprite::create((level->dailyID ? "Daily" : "Normal"), 20, true, "bigFont.fnt", "GJ_button_01.png", 30, 0.5);
    auto buttonButton = gd::CCMenuItemSpriteExtra::create(
        buttonSprite,
        self,
        menu_selector(GamingButton::onLeaderboardDaily)
    );
    buttonButton->setSizeMult(1.2f);
    buttonButton->setPosition({196,-87});
    menu->addChild(buttonButton);

    return true;
}

bool __fastcall LeaderboardsLayer_init(ProfilePage* self, void* a, int state){
    if(!MHook::getOriginal(LeaderboardsLayer_init)(self, a, state)) return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto layer = cast<CCLayer*>(self->getChildren()->objectAtIndex(0));

    auto refreshBtn = gd::CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png"),
        self,
        menu_selector(GamingButton::onLeaderboardRefresh)
    );

    auto menuRefresh = CCMenu::create();
    menuRefresh->addChild(refreshBtn);
    menuRefresh->setPosition({winSize.width - 26.75f, 26.75f});
    menuRefresh->setZOrder(2);

    self->addChild(menuRefresh);

    return true;
}

/*void __fastcall GameStatsManager_incrementChallenge(void* self, void* a, int challengeType, int count){
    MHook::getOriginal(GameStatsManager_incrementChallenge)(self, a, challengeType, count);

    auto CM = CvoltonManager::sharedState();
    showQuestExclamationMark(CM->getActiveCreator());
}*/

void setupPageLimitBypass(){
    auto proc = GetCurrentProcess();
    auto winapiBase = reinterpret_cast<char*>(base);
    //This patches the maximum number for SetIDPopup to INT_MAX
    unsigned char patch[] = {0x68, 0xFF, 0xFF, 0xFF, 0x7F};
    WriteProcessMemory(proc, winapiBase + 0x1431F6, patch, 5, NULL);
    //This patches the amount of characters allowed in the text input in SetIDPopup to 6 characters
    unsigned char patch2[] = {0x06};
    WriteProcessMemory(proc, winapiBase + 0x14371C, patch2, 1, NULL);
}

void setupDailyNew(){
    auto proc = GetCurrentProcess();
    auto winapiBase = reinterpret_cast<char*>(base);
    //This moves the position of the New! displayed when there's a new daily from 182 (0x43360000) to 130 (0x43020000)
    unsigned char patch[] = {0xC7, 0x04, 0x24, 0x00, 0x00, 0x02, 0x43};
    WriteProcessMemory(proc, winapiBase + 0x6C709, patch, 7, NULL);
}

DWORD WINAPI my_thread(void* hModule) {

    /*AllocConsole();
    std::ofstream conout("CONOUT$", std::ios::out);
    std::ifstream conin("CONIN$", std::ios::in);
    std::cout.rdbuf(conout.rdbuf());
    std::cin.rdbuf(conin.rdbuf());*/


    MH_Initialize();

    /*auto gdshare = reinterpret_cast<uintptr_t>(GetModuleHandle("GDShare-v0.3.4.dll"));
    auto betteredit = reinterpret_cast<uintptr_t>(GetModuleHandle("BetterEdit-v4.0.5.dll"));*/

    //base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    
    MHook::registerHook(base + 0x14F5A0, InfoLayer_init);
    MHook::registerHook(base + 0x151500, InfoLayer_onMore);
    MHook::registerHook(base + 0x151850, InfoLayer_onLevelInfo);
    MHook::registerHook(base + 0x152270, InfoLayer_setupCommentsBrowser);
    MHook::registerHook(base + 0x151E70, InfoLayer_loadPage);
    //setupProgressBars = very bad workaround for interoperability with gdshare lol (help how do i hook something thats already hooked)
    MHook::registerHook(base + 0x15C350, LevelBrowserLayer_updateLevelsLabel);

    /*bool LevelBrowserLayer_hook = false;
    if(!MHook::registerHook(base + 0x15A040, LevelBrowserLayer_init)){
        //gdshare conflict
        if(gdshare != 0) LevelBrowserLayer_hook = MHook::registerHook(gdshare + 0xFBD0, LevelBrowserLayer_init);
        //betteredit conflict
        if(!LevelBrowserLayer_hook && betteredit != 0) MHook::registerHook(betteredit + 0x3FF50, LevelBrowserLayer_init);
    }*/
    MHook::registerHook(base + 0x177FC0, LevelInfoLayer_setupProgressBars);
    MHook::registerHook(base + 0x17AC90, LevelInfoLayer_onViewProfile);
    MHook::registerHook(base + 0x17ACF0, LevelInfoLayer_onLevelInfo);
    MHook::registerHook(base + 0x70660, EditLevelLayer_onLevelInfo);
    //MHook::registerHook(base + 0x1865E0, LevelPage_onInfo);
    MHook::registerHook(base + 0x5C790, LevelCell_onViewProfile);
    MHook::registerHook(base + 0x5A020, LevelCell_loadCustomLevelCell);
    MHook::registerHook(base + 0x5F3D0, CommentCell_loadFromComment);
    MHook::registerHook(base + 0x210040, ProfilePage_loadPageFromUserInfo);
    MHook::registerHook(base + 0x20EF00, ProfilePage_init);
    MHook::registerHook(base + 0xA1C20, GameLevelManager_userNameForUserID);
    MHook::registerHook(base + 0x4DE40, CreatorLayer_init);
    MHook::registerHook(base + 0x4F1B0, CreatorLayer_onChallenge);
    MHook::registerHook(base + 0x4FAE0, CreatorLayer_onBack);
    MHook::registerHook(base + 0x4FB50, CreatorLayer_sceneWillResume); //onLeaderboards 0x4ED20
    MHook::registerHook(base + 0x6BEF0, DailyLevelPage_updateTimers);
    MHook::registerHook(base + 0x6A900, DailyLevelPage_init);
    MHook::registerHook(base + 0x17C4F0, LevelLeaderboard_init); //0x17D090 onChangeType
    MHook::registerHook(base + 0x1587B0, LeaderboardsLayer_init);
    //MHook::registerHook(base + 0xF9AE0, GameStatsManager_incrementChallenge);
    //MHook::registerHook(base + 0x2133E0, ProfilePage_getUserInfoFailed);

    setupPageLimitBypass();
    setupDailyNew();

    MH_EnableHook(MH_ALL_HOOKS);


    /*std::getline(std::cin, std::string());

    MH_Uninitialize();
    conout.close();
    conin.close();
    FreeConsole();
    FreeLibraryAndExitThread(cast<HMODULE>(hModule), 0);*/

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, my_thread, hModule, 0, 0);
    return TRUE;
}
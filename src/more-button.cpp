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
#include "utils.hpp"

using namespace cocos2d;

uintptr_t base;

//members stolen from wylies gd decomp
class InfoLayer : public gd::FLAlertLayer {
public:
    PAD(472 - sizeof(gd::FLAlertLayer));
    gd::GJGameLevel* m_pLevel;
    gd::GJUserScore* m_pScore;
    std::string m_sCommentKey;
    gd::LoadingCircle* m_pLoadingCircle;
    cocos2d::CCLabelBMFont* m_pPageLabel;
    cocos2d::CCLabelBMFont* m_pCommentsGoldLabel;
    void* m_pCommentList; //gd::GJCommentListLayer
    gd::CCMenuItemSpriteExtra* m_pNextPageBtn;
    gd::CCMenuItemSpriteExtra* m_pPrevPageBtn;
    gd::CCMenuItemSpriteExtra* m_pLikeIcon;
    gd::CCMenuItemSpriteExtra* m_pTimeIcon;
    gd::CCMenuItemSpriteExtra* m_pReportBtn;
    gd::CCMenuItemSpriteExtra* m_pCommentsBtn;
    gd::CCMenuItemSpriteExtra* m_pRefreshCommentsBtn;
    int m_nPageStartIdx;
    int m_nPageEndIdx;
    int m_nTotalItems;
    int m_nPageNumber;
    bool m_bCanUpdateUserScore;
    bool m_bCommentHistory;

    static InfoLayer* create(gd::GJGameLevel* level, gd::GJUserScore* score) {
        return reinterpret_cast<InfoLayer*(__fastcall*)(gd::GJGameLevel*, gd::GJUserScore*)>(
            gd::base + 0x14F4F0
        )(level, score);
    }
};

class LevelBrowserLayer : public CCLayer {
public:

    static CCScene* scene(gd::GJSearchObject* searchObject) {
        return reinterpret_cast<CCScene*(__fastcall*)(gd::GJSearchObject*)>(
            gd::base + 0x159F60
        )(searchObject);
    }
};

class UnregisteredProfileLayer : public gd::FLAlertLayer {
    gd::GJUserScore* score;
public:
    static UnregisteredProfileLayer* create(gd::GJUserScore* score){
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

    void onClose(cocos2d::CCObject* sender)
    {
        score->release();
        setKeypadEnabled(false);
        removeFromParentAndCleanup(true);
    }

    static void displayProfile(int userID, std::string userName){
        gd::GJUserScore* score = gd::GJUserScore::create();
        score->setUserID(userID);
        score->setPlayerName(userName);
        UnregisteredProfileLayer* profileLayer = UnregisteredProfileLayer::create(score);
        profileLayer->show();
    }

    static void doSearch(int userID) {
        auto searchObject = gd::GJSearchObject::create(gd::SearchType::kSearchTypeUsersLevels, std::to_string(userID));//, contentStream.str());
        auto browserLayer = LevelBrowserLayer::scene(searchObject);

        auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
        
        CCDirector::sharedDirector()->replaceScene(transitionFade);
    }

    void onMyLevels(CCObject* sender) {
        doSearch(score->getUserID());
    }

    void onCommentHistory(CCObject* sender) {
        InfoLayer* infoLayer = InfoLayer::create(nullptr, score);
        infoLayer->show();
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
};

class ExtendedLevelInfo : public gd::FLAlertLayer {
    gd::GJGameLevel* level;
public:
    static ExtendedLevelInfo* create(gd::GJGameLevel* level){
        auto ret = new ExtendedLevelInfo();
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

    static std::string getGameVersionName(int version){
        if(version > 99) return std::string("Hacked");

        switch(version){
            case 6:
                return std::string("1.6 or earlier");
            case 10:
                return std::string("1.7");
            case 11:
                return std::string("1.7 or 1.8");
        }

        std::stringstream contentStream;

        if(version > 17){
            double newVersion = (double)version / 10.0;
            contentStream << std::fixed << std::setprecision(1) << newVersion;
        }else{
            contentStream << "1." << (version-1);
        }
        return contentStream.str();
    }

    static std::string stringDate(std::string date){
        if(date == "") return "NA";
        std::ostringstream stream;
        stream << date << " ago";
        return stream.str();
    }

    static const char* getDifficultyIcon(int stars){
        switch(stars){
            case 1: 
                return "difficulty_auto_btn_001.png";
            case 2:
                return "difficulty_01_btn_001.png";
            case 3:
                return "difficulty_02_btn_001.png";
            case 4:
            case 5:
                return "difficulty_03_btn_001.png";
            case 6:
            case 7:
                return "difficulty_04_btn_001.png";
            case 8:
            case 9:
                return "difficulty_05_btn_001.png";
            case 10:
                return "difficulty_06_btn_001.png";
            default:
                return "difficulty_00_btn_001.png";
        }
    }

    static std::string passwordString(int password){
        if(password == 0) return "NA";
        if(password == 1) return "Free Copy";
        return std::to_string(password - 1000000);
    }

    static std::string zeroIfNA(int value){
        if(value == 0) return "NA";
        return std::to_string(value);
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
        bg->setContentSize({ 440.0f, 290.0f });
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
        closeButton->setPosition({-210, 136});
        closeButton->setSizeMult(1.2f);

        auto levelName = CCLabelBMFont::create(level->levelName.c_str(), "bigFont.fnt");
        levelName->setPosition({0,125});
        m_pButtonMenu->addChild(levelName);

        std::ostringstream userNameText;
        userNameText << "By " << level->userName;
        auto userName = CCLabelBMFont::create(userNameText.str().c_str(), "goldFont.fnt");
        userName->setPosition({0,99});
        userName->setScale(0.8f);
        m_pButtonMenu->addChild(userName);

        cocos2d::extension::CCScale9Sprite* descBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
        descBg->setContentSize({340,55});
        descBg->setColor({130,64,33});
        m_pButtonMenu->addChild(descBg, -1);
        descBg->setPosition({0,52});

        auto descText = level->getUnpackedLevelDescription();
        size_t descLength = descText.length();
        float descDelimiter = 1;
        if(descLength > 140) descLength = 140;
        if(descLength > 70) descDelimiter = ((((140 - descLength) / 140) * 0.3f) + 0.7f);
        auto description = gd::TextArea::create("chatFont.fnt", false, descText, 1, 295 / descDelimiter, 20, {0.5f,0.5f});
        description->setScale(descDelimiter);
        description->setPosition({0,52});
        m_pButtonMenu->addChild(description);

        cocos2d::extension::CCScale9Sprite* infoBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
        infoBg->setContentSize({180,148});
        //infoBg->setColor({130,64,33});
        //infoBg->setColor({191,114,62});
        infoBg->setColor({123,60,31});
        m_pButtonMenu->addChild(infoBg, -1);
        infoBg->setPosition({-80,-57});

        int levelPassword = level->password_rand - level->password_seed;
        std::ostringstream infoText;
        infoText << "\n<cj>Uploaded</c>: " << stringDate(level->uploadDate)
            << "\n<cj>Updated</c>: " << stringDate(level->updateDate)
            //<< "\n<cy>Stars Requested</c>: " << level->starsRequested
            << "\n<cg>Original</c>: " << zeroIfNA(level->originalLevel)
            << "\n<cy>Game Version</c>: " << getGameVersionName(level->gameVersion)
            //<< "\nFeature Score</c>: " << level->featured
            << "\n<co>Password</c>: " << passwordString(levelPassword)
            << "\n<cr>Working time</c>: " << zeroIfNA(level->workingTime)
            << "\n<cr>Working time 2</c>: " << zeroIfNA(level->workingTime2);

        auto info = gd::TextArea::create("chatFont.fnt", false, infoText.str(), 1, 295, 20, {0,1});
        info->setPosition({403,-39});
        info->setScale(0.95f);
        m_pButtonMenu->addChild(info);

        /*std::ostringstream uploadedText;
        uploadedText << "Uploaded: " << level->uploadDate << " ago";
        createTextLabel(uploadedText.str(), {0,0}, 0.5f, m_pButtonMenu);*/

        createTextLabel("Requested Stars:", {93, 10}, 0.49f, m_pButtonMenu);

        auto diffSprite = CCSprite::createWithSpriteFrameName(getDifficultyIcon(level->starsRequested));
        diffSprite->setPosition({93,-32});
        m_pButtonMenu->addChild(diffSprite, 1);

        if(level->starsRequested > 0){
            auto featureSprite = CCSprite::createWithSpriteFrameName("GJ_featuredCoin_001.png");
            featureSprite->setPosition({93,-32});
            m_pButtonMenu->addChild(featureSprite);
            //infoSprite->setScale(0.7f);

            createTextLabel(std::to_string(level->starsRequested), {86, -62}, 0.4f, m_pButtonMenu);

            auto diffSprite = CCSprite::createWithSpriteFrameName("star_small01_001.png");
            diffSprite->setPosition({99,-62});
            m_pButtonMenu->addChild(diffSprite);
        }

        return true;
    }

    static CCLabelBMFont* createTextLabel(const std::string text, const CCPoint& position, const float scale, CCNode* menu, const char* font = "bigFont.fnt"){
        CCLabelBMFont* bmFont = CCLabelBMFont::create(text.c_str(), font);
        bmFont->setPosition(position);
        bmFont->setScale(scale);
        menu->addChild(bmFont);
        return bmFont;
    }
};

class ProfilePage : public gd::FLAlertLayer {
public:
    //PAD(472 - sizeof(gd::FLAlertLayer));
    PAD(488 - sizeof(gd::FLAlertLayer));
    gd::GJUserScore* score; //488-492
    int something;
    //564

    /*static ProfilePage* create(int accountID, bool a2) {
        return reinterpret_cast<ProfilePage*(__fastcall*)(int, bool)>(
            gd::base + 0x20EE50
        )(accountID, a2);
    }

    ProfilePage* loadPageFromUserInfo(gd::GJUserScore* score) {
        return reinterpret_cast<ProfilePage*(__fastcall*)(ProfilePage*, gd::GJUserScore*)>(
            gd::base + 0x210040
        )(this, score);
    }*/
};

class LevelInfoLayer : public CCLayer {
    public:
        PAD(324 - sizeof(CCLayer));
        gd::GJGameLevel* level;
};

class LevelCell : public CCLayer {
    public:
        PAD(89+4);
        gd::GJGameLevel* level;
};

//i stole gjcomment from wylies gd decompiled
class GJComment : public CCNode {
    public:
        std::string m_sComment;
        std::string m_sUsername;
        int m_nCommentID;
        int m_nAuthorPlayerID;
        int m_nLikes;
        int m_nLevelID;
        bool m_bIsSpam;
        int m_nAuthorAccountID;
        std::string m_sCommentAge;
        bool m_bCommentDeleted;
        int m_nPercentage;
        int m_nModBadge;
        cocos2d::ccColor3B m_cColor;
        bool m_bHasLevelID;
        gd::GJUserScore* m_pUserScore;
};

class CommentCell : public CCLayer {
    public:
        PAD(89+16);
        GJComment* comment;
        bool accountComment;
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
    void onSimilar(CCObject* sender) {
        auto layer = cast<InfoLayer*>(this);

        std::stringstream contentStream;
        contentStream << layer->m_pLevel->levelID;

        auto searchObject = gd::GJSearchObject::create(gd::SearchType::kSearchTypeSimilar, contentStream.str());//, contentStream.str());
        auto browserLayer = LevelBrowserLayer::scene(searchObject);

        auto transitionFade = CCTransitionFade::create(0.5, browserLayer);

        CCDirector::sharedDirector()->replaceScene(transitionFade);
    }

    void onProfilePage(CCObject* sender){
        auto layer = cast<CommentCell*>(this);
        layer->comment->m_pUserScore->setUserID(layer->comment->m_nAuthorPlayerID);
        UnregisteredProfileLayer* profileLayer = UnregisteredProfileLayer::create(layer->comment->m_pUserScore);
        profileLayer->show();
    }

    void onProfilePageInfo(CCObject* sender){
        auto layer = cast<ProfilePage*>(this);

        auto score = layer->score;

        std::ostringstream contentStream;
        contentStream << "User ID: " << score->getUserID()
            << "\nAccount ID: " << score->getAccountID()
            << "\n"//Leaderboard Icon: " << StaticStringHelper::getIconType(score->getIconType()) (is not sent)
            << "\nFriend Requests: " << StaticStringHelper::getFriendRequestType(score->getFriendStatus())
            << "\nPrivate Messages: " << StaticStringHelper::getMessageType(score->getMessageState())
            << "\nComment History: " << StaticStringHelper::getMessageType(score->getCommentHistoryStatus());

        if(score->getUserID() == 6330800) contentStream << "\n\nThis user is epic!";

        gd::FLAlertLayer::create(nullptr, "User Info", "OK", nullptr, contentStream.str())->show();

        //auto GLM = gd::GameLevelManager::sharedState();
        //GLM->getGJUserInfo(something);
    }
};

void createButton(CCLayer* self, CCNode* menu, const char* text, cocos2d::SEL_MenuHandler handler, float x, float y, int width, float height){
    auto buttonSprite = gd::ButtonSprite::create(text, width, true, "bigFont.fnt", "GJ_button_01.png", height, 0.45f);
    auto buttonButton = gd::CCMenuItemSpriteExtra::create(
        buttonSprite,
        self,
        handler
    );
    buttonButton->setSizeMult(1.2f);
    buttonButton->setPosition({x,y});
    menu->addChild(buttonButton);
}

bool __fastcall InfoLayer_init(CCLayer* self, void* a, gd::GJGameLevel* level, void* c) {
    if (!MHook::getOriginal(InfoLayer_init)(self, a, level, c)) return false;

    auto layer = cast<CCLayer*>(self->getChildren()->objectAtIndex(0));
    CCMenu* menu = cast<CCMenu*>(layer->getChildren()->objectAtIndex(1));

    gd::CCMenuItemSpriteExtra* playerName = cast<gd::CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(0));
    playerName->setEnabled(true);

    if(!gd::GameManager::sharedState()->getGameVariable("0089") && level != nullptr)
        createButton(self, menu, "Similar", menu_selector(GamingButton::onSimilar), -138, 97, (int)(90*0.45), 44*0.45f);


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

void __fastcall LevelCell_loadCustomLevelCell(CCNode* self) {
    MHook::getOriginal(LevelCell_loadCustomLevelCell)(self);
    
    auto layer = cast<CCLayer*>(self->getChildren()->objectAtIndex(1));
    for(unsigned int i = 0; i < layer->getChildrenCount(); i++){
        auto menu = dynamic_cast<CCMenu*>(layer->getChildren()->objectAtIndex(i));
        if(menu != nullptr){
            auto playerName = cast<gd::CCMenuItemSpriteExtra*>(menu->getChildren()->objectAtIndex(1));
            playerName->setEnabled(true);
            break;
        }
    }
}

void* __fastcall ProfilePage_loadPageFromUserInfo(ProfilePage* self, void* a, gd::GJUserScore* a2){
    void* returnValue = MHook::getOriginal(ProfilePage_loadPageFromUserInfo)(self, a, a2);

    auto layer = cast<CCLayer*>(self->getChildren()->objectAtIndex(0));

    for(unsigned int i = 0; i < layer->getChildrenCount(); i++){
        auto menu = dynamic_cast<CCMenu*>(layer->getChildren()->objectAtIndex(i));
        if(menu != nullptr){
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
        }
    }

    //self->reloadButton->setVisible(true);

    return returnValue;
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

    if(b->m_bHasLevelID) return;

    auto layer = cast<CCLayer*>(self->getChildren()->objectAtIndex(1));
    auto playerName = cast<CCLabelBMFont*>(layer->getChildren()->objectAtIndex(2));
    if(b->m_nAuthorAccountID == 0){
        if(strlen(playerName->getString()) == 0){
            std::stringstream contentStream;
            contentStream << "- (ID: " << b->m_nAuthorPlayerID << ")";
            playerName->setString(contentStream.str().c_str());
        }
        layer->removeChild(playerName);

        for(unsigned int i = 0; i < layer->getChildrenCount(); i++){
            auto menu = dynamic_cast<CCMenu*>(layer->getChildren()->objectAtIndex(i));
            if(menu != nullptr){
                bool smallCommentsMode = gd::GameManager::sharedState()->getGameVariable("0088");

                auto buttonButton = gd::CCMenuItemSpriteExtra::create(
                    playerName,
                    self,
                    menu_selector(GamingButton::onProfilePage)
                );
                buttonButton->setSizeMult(1.2f);
                buttonButton->setPosition(-254, smallCommentsMode ? -141.5f : -109.5f);
                buttonButton->setAnchorPoint(CCPoint(0,0));
                buttonButton->setEnabled(true);
                menu->addChild(buttonButton);
            }
        }
    }
}

std::string printableProgress(std::string personalBests){
    std::ostringstream contentStream;
    int percentage = 0;

    std::stringstream bestsStream(personalBests);
    std::string currentBest;
    while(getline(bestsStream, currentBest, ',')){
        try {
            percentage += std::stoi(currentBest);
            contentStream << percentage << "% ";
        }catch(...){}
    }

    return contentStream.str();
}

void __fastcall LevelInfoLayer_onLevelInfo(LevelInfoLayer* self, void* a, CCObject* sender) {
    auto level = self->level;
    std::ostringstream contentStream;
    contentStream << "<cg>Total Attempts</c>: " << level->attempts
        << "\n<cl>Total Jumps</c>: " << level->jumps
        //<< "\n<co>Total Clicks</c>: " << level->clicks // the contents of this variable make no sense to me
        << "\n<cp>Normal</c>: " << level->normalPercent
        << "%\n<co>Practice</c>: " << level->practicePercent << "%";

    if(level->orbCompletion != level->newNormalPercent2) contentStream << "\n<cj>2.1 Normal</c>: " << level->orbCompletion << "%";
    if(level->newNormalPercent2 != level->normalPercent) contentStream << "\n<cr>2.11 Normal</c>: " << level->newNormalPercent2 << "%";
    if(level->personalBests != "") contentStream << "\n\n<cy>Progresses</c>: " << printableProgress(level->personalBests);
    //contentStream << "\n\nProgresses: " << level->personalBests;

    //if(score->getUserID() == 6330800) contentStream << "\n\nThis user is epic!";

    gd::FLAlertLayer::create(nullptr, "Level Stats", "OK", nullptr, contentStream.str())->show();
    
}

void __fastcall InfoLayer_onLevelInfo(InfoLayer* self, void* a, CCObject* sender) {
    //MHook::getOriginal(CommentCell_loadFromComment)(self, a, b);

    auto level = self->m_pLevel;

    ExtendedLevelInfo* layer = ExtendedLevelInfo::create(level);
    layer->show();
}

DWORD WINAPI my_thread(void* hModule) {
    MH_Initialize();

    base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    
    MHook::registerHook(base + 0x14F5A0, InfoLayer_init);
    MHook::registerHook(base + 0x151500, InfoLayer_onMore);
    MHook::registerHook(base + 0x151850, InfoLayer_onLevelInfo);
    //setupProgressBars = very bad workaround for interoperability with gdshare lol (help how do i hook something thats already hooked)
    MHook::registerHook(base + 0x177FC0, LevelInfoLayer_setupProgressBars);
    MHook::registerHook(base + 0x17AC90, LevelInfoLayer_onViewProfile);
    MHook::registerHook(base + 0x17ACF0, LevelInfoLayer_onLevelInfo);
    MHook::registerHook(base + 0x5C790, LevelCell_onViewProfile);
    MHook::registerHook(base + 0x5A020, LevelCell_loadCustomLevelCell);
    MHook::registerHook(base + 0x5F3D0, CommentCell_loadFromComment);
    MHook::registerHook(base + 0x210040, ProfilePage_loadPageFromUserInfo);
    //MHook::registerHook(base + 0x2133E0, ProfilePage_getUserInfoFailed);

    MH_EnableHook(MH_ALL_HOOKS);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, my_thread, hModule, 0, 0);
    return TRUE;
}
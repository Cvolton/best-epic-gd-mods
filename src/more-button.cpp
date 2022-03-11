#define WIN32_LEAN_AND_MEAN
#include "mapped-hooks.hpp"
#include "Windows.h"
#include <fstream>
#include <iostream>
#include <cocos2d.h>
#include <gd.h>
#include <typeinfo>
#include <cstring>
#include "utils.hpp"

using namespace cocos2d;

uintptr_t base;

class InfoLayer : CCLayer {
public:
    PAD(472 - sizeof(CCLayer));
    gd::GJGameLevel* level;
};

class LevelBrowserLayer : public CCLayer {
public:

    static CCScene* scene(gd::GJSearchObject* searchObject) {
        return reinterpret_cast<CCScene*(__fastcall*)(gd::GJSearchObject*)>(
            gd::base + 0x159F60
        )(searchObject);
    }
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
};

class GamingButton {
public:
    void onSimilar(CCObject* sender) {
        auto layer = cast<InfoLayer*>(this);

        std::stringstream contentStream;
        contentStream << layer->level->levelID;

        auto searchObject = gd::GJSearchObject::create(gd::SearchType::kSearchTypeSimilar, contentStream.str());//, contentStream.str());
        auto browserLayer = LevelBrowserLayer::scene(searchObject);

        auto transitionFade = CCTransitionFade::create(0.5, browserLayer);

        CCDirector::sharedDirector()->replaceScene(transitionFade);
    }

    void onMoreComment(CCObject* sender) {
        auto layer = cast<CommentCell*>(this);
        
        std::stringstream contentStream;
        contentStream << layer->comment->m_nAuthorPlayerID;

        auto searchObject = gd::GJSearchObject::create(gd::SearchType::kSearchTypeUsersLevels, contentStream.str());//, contentStream.str());
        auto browserLayer = LevelBrowserLayer::scene(searchObject);

        auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
        
        CCDirector::sharedDirector()->replaceScene(transitionFade);
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

void doSearch(int userID) {
    std::stringstream contentStream;
    contentStream << userID;

    auto searchObject = gd::GJSearchObject::create(gd::SearchType::kSearchTypeUsersLevels, contentStream.str());//, contentStream.str());
    auto browserLayer = LevelBrowserLayer::scene(searchObject);

    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);
    
    CCDirector::sharedDirector()->replaceScene(transitionFade);
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
    if(self->level->accountID == 0) {
        doSearch(self->level->userID);
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
    std::stringstream contentStream;
    contentStream << "accid " << self->level->accountID << "\nusrid " << self->level->userID;

    if(self->level->accountID == 0) {
        doSearch(self->level->userID);
        return true;
    }

    if (!MHook::getOriginal(LevelInfoLayer_onViewProfile)(self, a, b)) return false;

    return true;
}

bool __fastcall LevelCell_onViewProfile(LevelCell* self, void* a, CCObject* b) {

    std::stringstream contentStream;
    contentStream << "accid " << self->level->accountID << "\nusrid " << self->level->userID;

    if(self->level->accountID == 0) {
        doSearch(self->level->userID);
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

void __fastcall CommentCell_loadFromComment(CommentCell* self, void* a, GJComment* b) {
    MHook::getOriginal(CommentCell_loadFromComment)(self, a, b);

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
                    menu_selector(GamingButton::onMoreComment)
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

DWORD WINAPI my_thread(void* hModule) {
    MH_Initialize();

    base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    
    MHook::registerHook(base + 0x14F5A0, InfoLayer_init);
    MHook::registerHook(base + 0x151500, InfoLayer_onMore);
    //setupProgressBars = very bad workaround for interoperability with gdshare lol (help how do i hook something thats already hooked)
    MHook::registerHook(base + 0x177FC0, LevelInfoLayer_setupProgressBars);
    MHook::registerHook(base + 0x17AC90, LevelInfoLayer_onViewProfile);
    MHook::registerHook(base + 0x5C790, LevelCell_onViewProfile);
    MHook::registerHook(base + 0x5A020, LevelCell_loadCustomLevelCell);
    MHook::registerHook(base + 0x5F3D0, CommentCell_loadFromComment);

    MH_EnableHook(MH_ALL_HOOKS);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, my_thread, hModule, 0, 0);
    return TRUE;
}
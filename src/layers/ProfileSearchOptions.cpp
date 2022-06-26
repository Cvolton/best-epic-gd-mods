#include "ProfileSearchOptions.h"
#include "ProfileSearchOptionsSongSelect.h"
#include "../managers/CvoltonManager.h"

#include <cocos2d.h>
#include <gd.h>

using namespace cocos2d;
using namespace gd;

ProfileSearchOptions* ProfileSearchOptions::create(gd::LevelBrowserLayer* levelBrowserLayer){
    auto ret = new ProfileSearchOptions();
    ret->levelBrowserLayer = levelBrowserLayer;
    if(levelBrowserLayer != nullptr) levelBrowserLayer->retain();
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

void ProfileSearchOptions::onClose(cocos2d::CCObject* sender)
{
    auto CM = CvoltonManager::sharedState();
    CM->save();
    destroyToggles();
    if(levelBrowserLayer != nullptr) levelBrowserLayer->loadPage(levelBrowserLayer->searchObject);
    if(levelBrowserLayer != nullptr) levelBrowserLayer->release();
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

void ProfileSearchOptions::onPrev(cocos2d::CCObject* sender)
{
    page -= 1;
    if(page < 0) page = 0;
    destroyToggles();
    drawToggles();
}

void ProfileSearchOptions::onSong(cocos2d::CCObject* sender)
{
    ProfileSearchOptionsSongSelect::create(this)->show();
}

void ProfileSearchOptions::onNext(cocos2d::CCObject* sender)
{
    page = (page + 1) % 2;
    destroyToggles();
    drawToggles();
}

bool ProfileSearchOptions::init(){
    bool init = createBasics({440.0f, 290.0f}, menu_selector(ProfileSearchOptions::onClose), 1.f, {0x00, 0x00, 0x00, 0x96});
    if(!init) return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    createTextLabel("Saved Level Filters", {(winSize.width / 2), (winSize.height / 2) + 125}, 1.f, m_pLayer, "bigFont.fnt");

    lengthBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    lengthBg->setContentSize({700,55});
    lengthBg->setColor({123,60,31});
    m_pButtonMenu->addChild(lengthBg, -1);
    lengthBg->setPosition({0,-120});
    lengthBg->setScale(0.6f);

    diffBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    diffBg->setContentSize({700,92});
    diffBg->setColor({123,60,31});
    m_pButtonMenu->addChild(diffBg, -1);
    diffBg->setPosition({0,-70});
    diffBg->setScale(0.6f);

    demonDiffBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    demonDiffBg->setContentSize({700,93});
    demonDiffBg->setColor({123,60,31});
    m_pButtonMenu->addChild(demonDiffBg, -1);
    demonDiffBg->setPosition({0,-105.5});
    demonDiffBg->setScale(0.6f);

    auto sprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    sprite->setFlipX(true);

    prevBtn = createButton("GJ_arrow_03_001.png", {- (winSize.width / 2) + 30, 0}, menu_selector(ProfileSearchOptions::onPrev));
    nextBtn = gd::CCMenuItemSpriteExtra::create(
        sprite,
        this,
        menu_selector(ProfileSearchOptions::onNext)
    );
    nextBtn->setPosition({(winSize.width / 2) - 30, 0});
    m_pButtonMenu->addChild(nextBtn);

    drawToggles();

    return true;
}

void ProfileSearchOptions::createToggle(const char* option, const char* name, float x, float y){
    auto CM = CvoltonManager::sharedState();

    auto buttonSprite = CCSprite::createWithSpriteFrameName(CM->getOption(option) ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png");
    buttonSprite->setScale(0.8f);
    auto button = gd::CCMenuItemSpriteExtra::create(
        buttonSprite,
        this,
        menu_selector(ProfileSearchOptions::onToggle)
    );
    m_pButtonMenu->addChild(button);
    button->setPosition({x, y});
    auto optionString = CCString::create(option);
    optionString->retain();
    button->setUserData(optionString);
    button->setSizeMult(1.2f);

    auto label = createTextLabel(name, {x + 20, y}, 0.5f, m_pButtonMenu);
    label->setAnchorPoint({0,0.5f});
    label->limitLabelWidth(80, 0.5f, 0);
}

void ProfileSearchOptions::destroyToggles(){
    //starting at 1 because 0 is the close button and 1 is the prev button
    unsigned int totalMembers = m_pButtonMenu->getChildrenCount();
    for(unsigned int i = 6; i < totalMembers; i++){
        //static index 1 because we're actively moving the elements
        auto object = static_cast<CCNode*>(m_pButtonMenu->getChildren()->objectAtIndex(6));
        auto userData = object->getUserData();
        if(userData != nullptr) static_cast<CCString*>(userData)->release();
        //m_pButtonMenu->removeChild(object, false);
        object->removeFromParent();
    }
    toggleCount = 0;
}

void ProfileSearchOptions::createButtonToggle(const char* option, CCNode* sprite, float x, float y, float scale){
    auto CM = CvoltonManager::sharedState();

    //auto buttonSprite = CCSprite::createWithSpriteFrameName(sprite);
    sprite->setScale(scale);
    auto button = gd::CCMenuItemSpriteExtra::create(
        sprite,
        this,
        menu_selector(ProfileSearchOptions::onToggle)
    );
    m_pButtonMenu->addChild(button);
    button->setPosition({x, y});
    if(!CM->getOption(option)) button->setColor({125,125,125});
    auto optionString = CCString::create(option);
    optionString->retain();
    button->setUserData(optionString);
    button->setSizeMult(1.2f);
}

void ProfileSearchOptions::drawToggles(){
    if(page % 2 == 0) drawTogglesPrimary();
    else drawTogglesSecondary();

    if(levelBrowserLayer != nullptr) levelBrowserLayer->loadPage(levelBrowserLayer->searchObject);
}

void ProfileSearchOptions::drawTogglesPrimary(){
    lengthBg->setVisible(true);
    prevBtn->setVisible(false);
    nextBtn->setVisible(true);
    demonDiffBg->setVisible(false);
    diffBg->setVisible(true);

    auto CM = CvoltonManager::sharedState();
    auto winSize = CCDirector::sharedDirector()->getWinSize();


    auto timeIcon = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
    m_pButtonMenu->addChild(timeIcon);
    timeIcon->setPosition({-193, -120});
    createButtonToggle("user_search_len_00", CCLabelBMFont::create("Tiny", "bigFont.fnt"), -142, -119, 0.6f);
    createButtonToggle("user_search_len_01", CCLabelBMFont::create("Short", "bigFont.fnt"), -69, -119, 0.6f);
    createButtonToggle("user_search_len_02", CCLabelBMFont::create("Medium", "bigFont.fnt"), 16, -119, 0.6f);
    createButtonToggle("user_search_len_03", CCLabelBMFont::create("Long", "bigFont.fnt"), 93.5f, -119, 0.6f);
    createButtonToggle("user_search_len_04", CCLabelBMFont::create("XL", "bigFont.fnt"), 150, -119, 0.6f);
    createButtonToggle("user_search_star", CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png"), 193, -120);

    for(unsigned int i = 0; i <= 6; i++){
        createButtonToggle(
            CCString::createWithFormat("user_search_diff_%02u", i)->getCString(),
            CCSprite::createWithSpriteFrameName(CCString::createWithFormat("difficulty_%02u_btn_001.png", i)->getCString()),
            -187 + (i * 53.5f),
            -70,
            .9f
        );
    }
    createButtonToggle("user_search_diff_auto", CCSprite::createWithSpriteFrameName("difficulty_auto_btn_001.png"), 187, -70, .9f);

    createToggle("user_search_uncompleted", "Uncompleted", -170, 80); //40 -60, 170 -60, 300 -60, 40 -110
    createToggle("user_search_completed", "Completed", -40, 80);
    createToggle("user_search_featured", "Featured", 90, 80);
    createToggle("user_search_original", "Original", -170, 35);
    createToggle("user_search_epic", "Epic", -40, 35);
    createToggle("user_search_song", "Song", 90, 35);
    if(CM->getOption("user_search_song")) createButton("GJ_plusBtn_001.png", {175, 35}, menu_selector(ProfileSearchOptions::onSong), .65f);
    createToggle("user_search_nostar", "No Star", -170, -10);
    createToggle("user_search_coins", "Coins", -40, -10);
    createToggle("user_search_twoplayer", "2-Player", 90, -10);

    //createToggle("user_search_advanced", "Enable Advanced Options", -170, 75);

    /*createTextLabel("Completed Mode:", {0, - (winSize.height / 2) + 65}, 0.5f, m_pButtonMenu, "goldFont.fnt");
    createButton("edit_leftBtn_001.png", {-120, - (winSize.height / 2) + 40}, menu_selector(ProfileSearchOptions::onCompletedPrev), 1.2f);
    auto label = createTextLabel(getCompletedString(), {0, - (winSize.height / 2) + 40}, 1, m_pButtonMenu, "bigFont.fnt");
    label->limitLabelWidth(200, 0.8f, 0);
    createButton("edit_rightBtn_001.png", {120, - (winSize.height / 2) + 40}, menu_selector(ProfileSearchOptions::onCompletedNext), 1.2f);*/
}

void ProfileSearchOptions::drawTogglesSecondary(){
    lengthBg->setVisible(false);
    prevBtn->setVisible(true);
    nextBtn->setVisible(false);
    demonDiffBg->setVisible(true);
    diffBg->setVisible(false);

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    for(unsigned int i = 0, diffSprite = 7; i <= 4; i++, diffSprite++){
        if(i == 2) diffSprite = 6;
        if(i == 3) diffSprite = 9;

        createButtonToggle(
            CCString::createWithFormat("user_search_demon_%02u", i)->getCString(),
            CCSprite::createWithSpriteFrameName(CCString::createWithFormat("difficulty_%02u_btn2_001.png", diffSprite)->getCString()),
            -133.5f + (i * 66.875f),
            -102,
            .9f
        );
    }
    //createToggle("user_search_advanced", "Enable Advanced Options", -170, 75);

    /*createTextLabel("Completed Mode:", {0, - (winSize.height / 2) + 65}, 0.5f, m_pButtonMenu, "goldFont.fnt");
    createButton("edit_leftBtn_001.png", {-120, - (winSize.height / 2) + 40}, menu_selector(ProfileSearchOptions::onCompletedPrev), 1.2f);
    auto label = createTextLabel(getCompletedString(), {0, - (winSize.height / 2) + 40}, 1, m_pButtonMenu, "bigFont.fnt");
    label->limitLabelWidth(200, 0.8f, 0);
    createButton("edit_rightBtn_001.png", {120, - (winSize.height / 2) + 40}, menu_selector(ProfileSearchOptions::onCompletedNext), 1.2f);*/
}

void ProfileSearchOptions::onDialogClosed(){
    if(levelBrowserLayer != nullptr) levelBrowserLayer->loadPage(levelBrowserLayer->searchObject);
}
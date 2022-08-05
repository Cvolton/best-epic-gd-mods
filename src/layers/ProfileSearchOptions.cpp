#include "ProfileSearchOptions.h"
#include "ProfileSearchOptionsSongSelect.h"
#include "IDRangePopup.h"
#include "../managers/CvoltonManager.h"

#include <cocos2d.h>
#include <gd.h>

using namespace cocos2d;
using namespace gd;

ProfileSearchOptions* ProfileSearchOptions::create(gd::LevelBrowserLayer* levelBrowserLayer, const std::string& prefix){
    auto ret = new ProfileSearchOptions();
    if (ret && ret->init(levelBrowserLayer, prefix)) {
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

void ProfileSearchOptions::onIdRange(cocos2d::CCObject* sender)
{
    auto CM = CvoltonManager::sharedState();
    IDRangePopup::create(this, getOptionInt("idrange_min"), getOptionInt("idrange_max"), "ID Range")->show();
}

void ProfileSearchOptions::onNext(cocos2d::CCObject* sender)
{
    page = (page + 1) % 2;
    destroyToggles();
    drawToggles();
}

void ProfileSearchOptions::onSecondaryInfo(cocos2d::CCObject* sender){
    FLAlertLayer::create(
        nullptr, 
        "Saved Level Filters", 
        "OK", 
        nullptr,
        350,
        "<cl>Demon Difficulty</c> filters are only active if you have <cg>enabled</c> the main <cy>Demon</c> filter on the 1st page."
    )->show();
}

bool ProfileSearchOptions::init(gd::LevelBrowserLayer* levelBrowserLayer, const std::string& prefix){
    bool init = createBasics({440.0f, 290.0f}, menu_selector(ProfileSearchOptions::onClose), 1.f, {0x00, 0x00, 0x00, 0x96});
    if(!init) return false;

    this->levelBrowserLayer = levelBrowserLayer;
    if(levelBrowserLayer != nullptr) levelBrowserLayer->retain();
    this->prefix = prefix;

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

    auto buttonSprite = CCSprite::createWithSpriteFrameName(getOption(option) ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png");
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

void ProfileSearchOptions::createToggle(const char* option, const char* name, float x, float y, SEL_MenuHandler additional){
    auto CM = CvoltonManager::sharedState();

    auto buttonSprite = CCSprite::createWithSpriteFrameName(getOption(option) ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png");
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
    label->limitLabelWidth(60, 0.5f, 0);

    if(getOption(option)) createButton("GJ_plusBtn_001.png", {x + 98, y}, additional, .65f);
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
    createButtonToggle("len_00", CCLabelBMFont::create("Tiny", "bigFont.fnt"), -142, -119, 0.6f);
    createButtonToggle("len_01", CCLabelBMFont::create("Short", "bigFont.fnt"), -69, -119, 0.6f);
    createButtonToggle("len_02", CCLabelBMFont::create("Medium", "bigFont.fnt"), 16, -119, 0.6f);
    createButtonToggle("len_03", CCLabelBMFont::create("Long", "bigFont.fnt"), 93.5f, -119, 0.6f);
    createButtonToggle("len_04", CCLabelBMFont::create("XL", "bigFont.fnt"), 150, -119, 0.6f);
    createButtonToggle("star", CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png"), 193, -120);

    for(unsigned int i = 0; i <= 6; i++){
        createButtonToggle(
            CCString::createWithFormat("diff_%02u", i)->getCString(),
            CCSprite::createWithSpriteFrameName(CCString::createWithFormat("difficulty_%02u_btn_001.png", i)->getCString()),
            -187 + (i * 53.5f),
            -70,
            .9f
        );
    }
    createButtonToggle("diff_auto", CCSprite::createWithSpriteFrameName("difficulty_auto_btn_001.png"), 187, -70, .9f);

    createToggle("uncompleted", "Uncompleted", -170, 80); //40 -60, 170 -60, 300 -60, 40 -110
    createToggle("completed", "Completed", -40, 80);
    createToggle("featured", "Featured", 90, 80);
    createToggle("original", "Original", -170, 35);
    createToggle("epic", "Epic", -40, 35);
    createToggle("song", "Song", 90, 35, menu_selector(ProfileSearchOptions::onSong));
    createToggle("nostar", "No Star", -170, -10);
    createToggle("coins", "Coins", -40, -10);
    createToggle("twoplayer", "2-Player", 90, -10);
}

void ProfileSearchOptions::drawTogglesSecondary(){
    lengthBg->setVisible(false);
    prevBtn->setVisible(true);
    nextBtn->setVisible(false);
    demonDiffBg->setVisible(true);
    diffBg->setVisible(false);

    auto CM = CvoltonManager::sharedState();
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    for(unsigned int i = 0, diffSprite = 7; i <= 4; i++, diffSprite++){
        if(i == 2) diffSprite = 6;
        if(i == 3) diffSprite = 9;

        createButtonToggle(
            CCString::createWithFormat("demon_%02u", i)->getCString(),
            CCSprite::createWithSpriteFrameName(CCString::createWithFormat("difficulty_%02u_btn2_001.png", diffSprite)->getCString()),
            -133.5f + (i * 66.875f),
            -102,
            .9f
        );
    }

    createButton("GJ_infoIcon_001.png", {203, 128}, menu_selector(ProfileSearchOptions::onSecondaryInfo));

    createToggle("copied", "Copied", -170, 80);
    createToggle("downloaded", "Downloaded", -40, 80);
    createToggle("ldm", "LDM", 90, 80);
    createToggle("idrange", "ID Range", -170, 35, menu_selector(ProfileSearchOptions::onIdRange));
    createToggle("copy", "Copyable", -40, 35);
    createToggle("copy_free", "Free Copy", 90, 35);
    createToggle("nofeatured", "Not Featured", -170, -10);
    createToggle("noepic", "Not Epic", -40, -10);
    
}

void ProfileSearchOptions::onDialogClosed(){
    if(levelBrowserLayer != nullptr) levelBrowserLayer->loadPage(levelBrowserLayer->searchObject);
}

void ProfileSearchOptions::onIDRangeFinished(int min, int max) {
    setOptionInt("idrange_min", min);
    setOptionInt("idrange_max", max);
    if(levelBrowserLayer != nullptr) levelBrowserLayer->loadPage(levelBrowserLayer->searchObject);
}

bool ProfileSearchOptions::getOption(const std::string& option) {
    return CvoltonManager::sharedState()->getOption(std::format("{}_{}", prefix, option));
}

int ProfileSearchOptions::getOptionInt(const std::string& option) {
    return CvoltonManager::sharedState()->getOptionInt(std::format("{}_{}", prefix, option));
}

void ProfileSearchOptions::onToggle(cocos2d::CCObject* sender)
{
    sender->retain();
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto CM = CvoltonManager::sharedState();
    toggleOption(static_cast<CCString*>(button->getUserData())->getCString());

    destroyToggles();
    drawToggles();
    sender->release();
}

bool ProfileSearchOptions::toggleOption(const std::string& option) {
    return CvoltonManager::sharedState()->toggleOption(std::format("{}_{}", prefix, option));
}

void ProfileSearchOptions::setOption(const std::string& option, bool value) {
    CvoltonManager::sharedState()->setOption(std::format("{}_{}", prefix, option), value);
}

void ProfileSearchOptions::setOptionInt(const std::string& option, int value) {
    CvoltonManager::sharedState()->setOptionInt(std::format("{}_{}", prefix, option), value);
}

BISearchObject ProfileSearchOptions::getSearchObject() {
    BISearchObject searchObj;

    if(getOption("diff_auto")) searchObj.difficulty.insert(-1);
    for(int i = 0; i <= 6; i++) {
        auto diff = CCString::createWithFormat("diff_%02i", i)->getCString();
        if(getOption(diff)) searchObj.difficulty.insert(i);
    }

    for(int i = 0; i <= 4; i++) {
        auto len = CCString::createWithFormat("len_%02i", i)->getCString();
        if(getOption(len)) searchObj.length.insert(i);
    }

    for(int i = 0; i <= 4; i++) {
        auto len = CCString::createWithFormat("demon_%02i", i)->getCString();
        if(getOption(len)) searchObj.length.insert(i);
    }

    searchObj.star = getOption("star");
    searchObj.noStar = getOption("nostar");
    searchObj.uncompleted = getOption("uncompleted");
    searchObj.completed = getOption("completed");
    searchObj.completedOrbs = false;
    searchObj.completedLeaderboard = false;
    searchObj.featured = getOption("featured");
    searchObj.original = getOption("original");
    searchObj.twoPlayer = getOption("twoplayer");
    searchObj.coins = getOption("coins");
    searchObj.epic = getOption("epic");
    searchObj.folder = 0;
    searchObj.song = getOption("song");
    searchObj.songCustom = false;
    searchObj.songID = 0;
    searchObj.copied = getOption("copied");
    searchObj.downloaded = getOption("downloaded");
    searchObj.ldm = getOption("ldm");
    searchObj.idRangeMin = getOptionInt("idrange_min");
    searchObj.idRangeMax = getOptionInt("idrange_max");
    searchObj.copyable = getOption("copy");
    searchObj.freeCopy = getOption("copy_free");
    searchObj.unfeatured = getOption("nofeatured");
    searchObj.unepic = getOption("noepic");
    searchObj.starRangeMin = 0;
    searchObj.starRangeMax = 0;
    searchObj.gameVersionMin = 0;
    searchObj.gameVersionMax = 0;

    return searchObj;
}

void ProfileSearchOptions::setSearchObject(const BISearchObject& searchObj) {
    for(int i = -1; i <= 6; i++) {
        setOption(
            i == -1 ? "diff_auto" : CCString::createWithFormat("diff_%02i", i)->getCString(),
            searchObj.difficulty.find(i) != searchObj.difficulty.end()
        );
    }

    for(int i = 0; i <= 4; i++) {
        setOption(
            CCString::createWithFormat("len_%02i", i)->getCString(),
            searchObj.length.find(i) != searchObj.length.end()
        );
    }

    for(int i = 0; i <= 4; i++) {
        setOption(
            CCString::createWithFormat("demon_%02i", i)->getCString(),
            searchObj.demonDifficulty.find(i) != searchObj.demonDifficulty.end()
        );
    }

    setOption("star", searchObj.star);
    setOption("nostar", searchObj.noStar);
    setOption("uncompleted", searchObj.uncompleted);
    setOption("completed", searchObj.completed);
    //searchObj.completedOrbs = false;
    //searchObj.completedLeaderboard = false;
    setOption("featured", searchObj.featured);
    setOption("original", searchObj.original);
    setOption("twoplayer", searchObj.twoPlayer);
    setOption("coins", searchObj.coins);
    setOption("epic", searchObj.epic);
    //searchObj.folder = 0;
    setOption("song", searchObj.song);
    //searchObj.songCustom = false;
    //searchObj.songID = 0;
    setOption("copied", searchObj.copied);
    setOption("downloaded", searchObj.downloaded);
    setOption("ldm", searchObj.ldm);
    setOptionInt("idrange_min", searchObj.idRangeMin);
    setOptionInt("idrange_max", searchObj.idRangeMax);
    setOption("copy", searchObj.copyable);
    setOption("copy_free", searchObj.freeCopy);
    setOption("nofeatured", searchObj.unfeatured);
    setOption("noepic", searchObj.unepic);
    /*searchObj.starRangeMin = 0;
    searchObj.starRangeMax = 0;
    searchObj.gameVersionMin = 0;
    searchObj.gameVersionMax = 0;*/
}
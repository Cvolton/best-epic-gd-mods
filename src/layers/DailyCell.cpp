#include "DailyCell.h"
#include "ExtendedLevelInfo.h"
#include "../managers/CvoltonManager.h"

using namespace gd;
using namespace cocos2d;

void DailyCell::loadFromLevel(GJGameLevel* level) {
    this->level = level;

    auto GLM = gd::GameLevelManager::sharedState();
    auto levelFromSaved = static_cast<GJGameLevel*>(GLM->m_onlineLevels->objectForKey(std::to_string(level->levelID).c_str()));

    auto diffSprite = CCSprite::createWithSpriteFrameName(ExtendedLevelInfo::getDifficultyIcon(level->stars));
    diffSprite->setPosition({22.f, 32.f});
    diffSprite->setScale(0.8f);
    diffSprite->setZOrder(1);
    this->m_pLayer->addChild(diffSprite);

    auto featureSprite = CCSprite::createWithSpriteFrameName("GJ_featuredCoin_001.png");
    featureSprite->setPosition({22.f, 32.f});
    featureSprite->setScale(0.8f);
    this->m_pLayer->addChild(featureSprite);
    //infoSprite->setScale(0.7f);

    auto starsLabel = CCLabelBMFont::create(std::to_string(level->stars).c_str(), "bigFont.fnt");
    starsLabel->setPosition({22, 8});
    starsLabel->setScale(.34f);
    starsLabel->setAnchorPoint({1, 0.5f});
    this->m_pLayer->addChild(starsLabel);

    auto starsSprite = CCSprite::createWithSpriteFrameName("star_small01_001.png");
    starsSprite->setPosition({23, 8});
    starsSprite->setAnchorPoint({0, 0.5f});
    starsSprite->setScale(0.8f);
    this->m_pLayer->addChild(starsSprite);

    auto title = CCLabelBMFont::create((levelFromSaved == nullptr) ? "Unknown" : levelFromSaved->levelName.c_str(), "bigFont.fnt");
    title->setAnchorPoint({ 0.0f, 0.5f });
    title->setPosition(44.0f, 40.0f);
    title->setScale(.7f);
    this->m_pLayer->addChild(title);

    auto numberSprite = CCSprite::createWithSpriteFrameName("GJ_sRecentIcon_001.png");
    numberSprite->setPosition({44, 9});
    numberSprite->setAnchorPoint({0, 0.5f});
    numberSprite->setScale(0.8f);
    this->m_pLayer->addChild(numberSprite);

    auto number = CCLabelBMFont::create(CCString::createWithFormat("%i", level->dailyID)->getCString(), "bigFont.fnt");
    number->setAnchorPoint({ 0.0f, 0.5f });
    number->setPosition(60.0f, 10.0f);
    number->setScale(.4f);
    this->m_pLayer->addChild(number);

    auto menu = CCMenu::create();

    auto viewSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
    viewSprite->setScale(.5f);
    auto viewBtn = gd::CCMenuItemSpriteExtra::create(
        viewSprite,
        this,
        menu_selector(DailyCell::onView)
    );
    viewBtn->setPosition({42, -132});
    menu->addChild(viewBtn);

    auto leaderboardSprite = CCSprite::createWithSpriteFrameName("GJ_levelLeaderboardBtn_001.png");
    leaderboardSprite->setScale(.85f);
    auto leaderboardBtn = gd::CCMenuItemSpriteExtra::create(
        leaderboardSprite,
        this,
        menu_selector(DailyCell::onLeaderboards)
    );
    leaderboardBtn->setPosition({0, -132});

    menu->addChild(leaderboardBtn);
    this->m_pLayer->addChild(menu);


    this->m_pLayer->setVisible(true);
    this->m_pBGLayer->setOpacity(255);
}

void DailyCell::draw() {
    // just call StatsCell::draw, no one will notice
    // this is stolen from betteredit blame fod
    reinterpret_cast<void(__thiscall*)(DailyCell*)>(
        base + 0x59d40
    )(this);
}

DailyCell::DailyCell(const char* name, CCSize size) :
    TableViewCell(name, 356.f, size.height) {} //356 should be size.width but its getting 220 for some reason and i cant be bothered to troubleshoot this rn //TODO: fix this properly

DailyCell* DailyCell::create(const char* key, CCSize size) {
    auto ret = new DailyCell(key, size);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void DailyCell::onView(CCObject* sender){
    auto searchObject = gd::GJSearchObject::create(gd::SearchType::kSearchTypeSearch, std::to_string(level->levelID));
    auto browserLayer = LevelBrowserLayer::scene(searchObject);

    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);

    CCDirector::sharedDirector()->pushScene(transitionFade);
}

void DailyCell::onLeaderboards(CCObject* sender){
    auto GM = gd::GameManager::sharedState();
    auto newLeaderboard = LevelLeaderboard::create(level, GM->getIntGameVariableDefault("0098", 1));
    newLeaderboard->show();
}
#include "DailyCell.h"
#include "ExtendedLevelInfo.h"
#include "../managers/CvoltonManager.h"
#include "../managers/BetterInfoCache.h"

using namespace gd;
using namespace cocos2d;

void DailyCell::loadFromLevel(GJGameLevel* level) {
    this->level = level;

    auto biCache = BetterInfoCache::sharedState();
    auto GLM = gd::GameLevelManager::sharedState();
    auto GSM = gd::GameStatsManager::sharedState();
    auto winSize = CCDirector::sharedDirector()->getWinSize();

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

    auto title = CCLabelBMFont::create(biCache->getLevelName(level->levelID), "bigFont.fnt");
    title->setAnchorPoint({ 0.0f, .5f });
    title->setPosition(43.0f, 42.f);
    title->limitLabelWidth(170, .7f, .4f);
    this->m_pLayer->addChild(title);

    /*CCNode* percentage = CCLabelBMFont::create(CCString::createWithFormat("%i%%", level->normalPercent)->getCString(), "goldFont.fnt");
    if(level->normalPercent == 100) percentage = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
    percentage->setAnchorPoint({ 0.0f, 0.5f });
    percentage->setPosition(title->getPositionX() + ((title->getContentSize().width) * title->getScaleX()) + 5, 40.0f);
    percentage->setScale(.5f);
    this->m_pLayer->addChild(percentage);*/

    //row 2
    auto numberSprite = CCSprite::createWithSpriteFrameName("GJ_sRecentIcon_001.png");
    numberSprite->setPosition({44, 9});
    numberSprite->setAnchorPoint({0, 0.5f});
    numberSprite->setScale(0.7f);
    this->m_pLayer->addChild(numberSprite);

    auto number = CCLabelBMFont::create(CCString::createWithFormat("%i", level->dailyID % 100000)->getCString(), "bigFont.fnt");
    number->setAnchorPoint({ 0.0f, 0.5f });
    number->setPosition(57.5f, 10.0f);
    number->setScale(.325f);
    this->m_pLayer->addChild(number);

    auto diamondSprite = CCSprite::createWithSpriteFrameName("diamond_small01_001.png");
    diamondSprite->setPosition({number->getPositionX() + ((number->getContentSize().width) * number->getScaleX()) + 5, 9});
    diamondSprite->setAnchorPoint({0, 0.5f});
    diamondSprite->setScale(0.7f);
    this->m_pLayer->addChild(diamondSprite);

    auto diamond = CCLabelBMFont::create(CCString::createWithFormat("%i/%i", getAwardedDiamonds(), getTotalDiamonds())->getCString(), "bigFont.fnt");
    diamond->setAnchorPoint({ 0.0f, 0.5f });
    diamond->setPosition(diamondSprite->getPositionX() + 11.f, 10.0f);
    diamond->setScale(.325f);
    if(getAwardedDiamonds() == getTotalDiamonds()) diamond->setColor({100,255,255});
    this->m_pLayer->addChild(diamond);

    auto orbSprite = CCSprite::createWithSpriteFrameName("currencyOrbIcon_001.png");
    orbSprite->setPosition({diamond->getPositionX() + ((diamond->getContentSize().width) * diamond->getScaleX()) + 5, 9});
    orbSprite->setAnchorPoint({0, 0.5f});
    orbSprite->setScale(0.5f);
    this->m_pLayer->addChild(orbSprite);

    int orbsMax = (GSM->getAwardedCurrencyForLevel(level) * 125) / 100;
    int orbsCollectible = GSM->getBaseCurrencyForLevel(level);
    auto orb = CCLabelBMFont::create(CCString::createWithFormat("%i/%i", orbsCollectible, orbsMax)->getCString(), "bigFont.fnt");
    if(orbsCollectible == orbsMax){ 
        orb = CCLabelBMFont::create(CCString::createWithFormat("%i", orbsCollectible)->getCString(), "bigFont.fnt");
        orb->setColor({100, 255, 255});
    }
    orb->setAnchorPoint({ 0.0f, 0.5f });
    orb->setPosition(orbSprite->getPositionX() + 11.5f, 10.0f);
    orb->setScale(.325f);
    this->m_pLayer->addChild(orb);

    //row 1
    auto percentSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    percentSprite->setFlipX(true);
    percentSprite->setPosition({44, 23});
    percentSprite->setAnchorPoint({0, 0.5f});
    percentSprite->setScale(0.35f);
    this->m_pLayer->addChild(percentSprite);

    auto percent = CCLabelBMFont::create(CCString::createWithFormat("%i%%", level->normalPercent)->getCString(), "bigFont.fnt");
    percent->setAnchorPoint({ 0.0f, 0.5f });
    percent->setPosition(57.5f, 24.0f);
    percent->setScale(.325f);
    if(level->normalPercent == 100) percent->setColor({255,255,128});
    this->m_pLayer->addChild(percent);

    auto practiceSprite = CCSprite::createWithSpriteFrameName("checkpoint_01_001.png");
    practiceSprite->setPosition({percent->getPositionX() + ((percent->getContentSize().width) * percent->getScaleX()) + 5, 23});
    practiceSprite->setAnchorPoint({0, 0.5f});
    practiceSprite->setScale(0.35f);
    this->m_pLayer->addChild(practiceSprite);

    auto practice = CCLabelBMFont::create(CCString::createWithFormat("%i%%", level->practicePercent)->getCString(), "bigFont.fnt");
    practice->setAnchorPoint({ 0.0f, 0.5f });
    practice->setPosition(practiceSprite->getPositionX() + 8.f, 24.0f);
    practice->setScale(.325f);
    if(level->practicePercent == 100) practice->setColor({255,255,128});
    this->m_pLayer->addChild(practice);

    /*auto coinSprite = CCSprite::createWithSpriteFrameName("GJ_coinsIcon2_001.png");
    coinSprite->setPosition({practice->getPositionX() + ((practice->getContentSize().width) * practice->getScaleX()) + 5, 23});
    coinSprite->setAnchorPoint({0, 0.5f});
    coinSprite->setScale(0.35f);
    this->m_pLayer->addChild(coinSprite);

    int coinsCollected = level->firstCoinVerified + level->secondCoinVerified + level->thirdCoinVerified;
    auto coin = CCLabelBMFont::create(CCString::createWithFormat("%i", coinsCollected)->getCString(), "bigFont.fnt");
    coin->setAnchorPoint({ 0.0f, 0.5f });
    coin->setPosition(coinSprite->getPositionX() + 8.f, 24.0f);
    coin->setScale(.325f);
    this->m_pLayer->addChild(coin);*/

    auto coinDict = GameStatsManager::sharedState()->m_verifiedUserCoins;

    float coinX = practice->getPositionX() + ((practice->getContentSize().width) * practice->getScaleX());
    for(int i = 1; i <= biCache->getCoinCount(level->levelID); i++){
        bool isCollected = coinDict->objectForKey(level->getCoinKey(i)) == nullptr;
        auto coinSprite = CCSprite::createWithSpriteFrameName("usercoin_small01_001.png");
        if(isCollected) coinSprite->setColor({165, 165, 165});
        coinSprite->setPosition({coinX += 6, 23});
        coinSprite->setAnchorPoint({0, 0.5f});
        coinSprite->setScale(0.7f);
        //coinSprite->setScale(0.35f);
        this->m_pLayer->addChild(coinSprite);
    }

    //buttons
    auto menu = CCMenu::create();

    auto viewSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
    viewSprite->setScale(.5f);
    auto viewBtn = gd::CCMenuItemSpriteExtra::create(
        viewSprite,
        this,
        menu_selector(DailyCell::onView)
    );
    //viewBtn->setPosition({42, -132});
    viewBtn->setPosition(CCPoint(327, 28) - (winSize / 2));
    menu->addChild(viewBtn);

    auto leaderboardSprite = CCSprite::createWithSpriteFrameName("GJ_levelLeaderboardBtn_001.png");
    leaderboardSprite->setScale(.85f);
    auto leaderboardBtn = gd::CCMenuItemSpriteExtra::create(
        leaderboardSprite,
        this,
        menu_selector(DailyCell::onLeaderboards)
    );
    leaderboardBtn->setPosition(viewBtn->getPosition() - CCPoint(40, 0));
    menu->addChild(leaderboardBtn);

    auto infoSprite = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
    infoSprite->setScale(.85f);
    auto infoBtn = gd::CCMenuItemSpriteExtra::create(
        infoSprite,
        this,
        menu_selector(DailyCell::onInfo)
    );
    infoBtn->setPosition(leaderboardBtn->getPosition() - CCPoint(40, 0));
    menu->addChild(infoBtn);

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
    TableViewCell(name, size.width, size.height) {}

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

void DailyCell::onInfo(CCObject* sender){
    ExtendedLevelInfo::showProgressDialog(level);
}

int DailyCell::getTotalDiamonds(){
    if(level->stars > 10) return 0;
    return level->stars + 2;
}

int DailyCell::getAwardedDiamonds(){
    return (getTotalDiamonds() * level->normalPercent) / 100;
}
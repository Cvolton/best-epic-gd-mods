#include "RewardCell.h"
#include "../managers/CvoltonManager.h"
#include "../utils.hpp"

using namespace gd;
using namespace cocos2d;

void RewardCell::loadFromReward(GJRewardItem* reward) {
    this->level = level;

    const float rowX = 48.f;
    const float rowY = 10.5f;

    CCSprite* chest = CCSprite::createWithSpriteFrameName(reward->m_bUnk ? "chest_02_02_001.png" : "chest_01_02_001.png");
    chest->setPosition({25, 21});
    chest->setScale(0.35f);
    this->m_pLayer->addChild(chest);

    auto title = CCLabelBMFont::create(CCString::createWithFormat("Daily Chest %i", reward->m_nChestID)->getCString(), "bigFont.fnt");
    title->setAnchorPoint({ 0.0f, .5f });
    title->setPosition(rowX, 31.5f);
    title->limitLabelWidth(356-rowX, .65f, .4f);
    this->m_pLayer->addChild(title);


    CCObject* obj;
    CCSprite* lastSprite = nullptr;
    CCLabelBMFont* lastText = nullptr;
    CCARRAY_FOREACH(reward->m_pRewardObjects, obj){
        auto rewardObj = static_cast<GJRewardObject*>(obj);

        const char* textureName = "GJ_sRecentIcon_001.png";
        float scale = 1;
        switch(rewardObj->m_specialRewardItem) {
            case SpecialRewardItem::FireShard: textureName = "fireShardSmall_001.png"; break;
            case SpecialRewardItem::IceShard: textureName = "iceShardSmall_001.png"; break;
            case SpecialRewardItem::PoisonShard: textureName = "poisonShardSmall_001.png"; break;
            case SpecialRewardItem::ShadowShard: textureName = "shadowShardSmall_001.png"; break;
            case SpecialRewardItem::LavaShard: textureName = "lavaShardSmall_001.png"; break;
            case SpecialRewardItem::BonusKey: textureName = "GJ_bigKey_001.png"; scale = .5f; break;
            case SpecialRewardItem::Orbs: textureName = "currencyOrbIcon_001.png"; scale = .7f; break;
            case SpecialRewardItem::Diamonds: textureName = "diamond_small01_001.png"; break;
            case SpecialRewardItem::CustomItem: textureName = "collaborationIcon_001.png"; scale = .5f; break;
            default: scale = 0.7f;
        }

        lastSprite = BetterInfo::createWithBISpriteFrameName(textureName);
        if(rewardObj->m_specialRewardItem == SpecialRewardItem::CustomItem) {
            lastSprite = GJItemIcon::createBrowserIcon(rewardObj->m_unlockType, rewardObj->m_itemID);
        }
        if(lastText == nullptr) lastSprite->setPosition({rowX + 1, rowY});
        else lastSprite->setPosition({lastText->getPositionX() + (lastText->getContentSize().width * lastText->getScaleX()) + 11.f, rowY});
        lastSprite->setAnchorPoint({0, 0.5f});
        lastSprite->setScale(scale);
        this->m_pLayer->addChild(lastSprite);

        lastText = CCLabelBMFont::create(CCString::createWithFormat("%i", rewardObj->m_total)->getCString(), "bigFont.fnt");
        lastText->setAnchorPoint({ 0.0f, 0.5f });
        lastText->setPosition({lastSprite->getPositionX() + ((lastSprite->getContentSize().width) * lastSprite->getScaleX()) + 5, rowY});
        lastText->setScale(.325f);
        this->m_pLayer->addChild(lastText);
    }

    this->m_pLayer->setVisible(true);
    this->m_pBGLayer->setOpacity(255);
}

void RewardCell::draw() {
    // just call StatsCell::draw, no one will notice
    // this is stolen from betteredit blame fod
    reinterpret_cast<void(__thiscall*)(RewardCell*)>(
        base + 0x59d40
    )(this);
}

RewardCell::RewardCell(const char* name, CCSize size) :
    TableViewCell(name, size.width, size.height) {}

RewardCell* RewardCell::create(const char* key, CCSize size) {
    auto ret = new RewardCell(key, size);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}
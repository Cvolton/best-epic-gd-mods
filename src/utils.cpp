#include <cocos2d.h>
#include <gd.h>
#include "utils.hpp"

#include <array>

using namespace cocos2d;
using namespace gd;

constexpr int levelsPerPageLow = 10;
constexpr int levelsPerPageHigh = 20;

CCSprite* BetterInfo::createWithBISpriteFrameName(const char* name){
        auto sprite = CCSprite::createWithSpriteFrameName(name);
        if(sprite) return sprite;

        return createPlaceholder();
}

CCSprite* BetterInfo::createBISprite(const char* name){
        auto sprite = CCSprite::create(name);
        if(sprite) return sprite;

        return createPlaceholder();
}

CCSprite* BetterInfo::createPlaceholder(){
        auto sprite = CCSprite::create();

        auto subSprite1 = CCSprite::create("square02b_001.png");
        auto subSprite2 = CCSprite::create("square02b_001.png");
        auto subSprite3 = CCSprite::create("square02b_001.png");
        auto subSprite4 = CCSprite::create("square02b_001.png");

        std::array<CCSprite*, 4> sprites = {subSprite1, subSprite2, subSprite3, subSprite4};

        subSprite1->setAnchorPoint({0,0});
        subSprite2->setAnchorPoint({1,0});
        subSprite3->setAnchorPoint({0,1});
        subSprite4->setAnchorPoint({1,1});

        subSprite2->setColor({251,64,249});
        subSprite1->setColor({0,0,0});
        subSprite3->setColor({251,64,249});
        subSprite4->setColor({0,0,0});

        for(auto subSprite : sprites){
                subSprite->setScale(.25f);
                subSprite->setPosition(subSprite->getContentSize() / 2);
                sprite->addChild(subSprite);

        }

        sprite->setContentSize(subSprite1->getContentSize());

        return sprite;
}

bool BetterInfo::isLocal(GJSearchObject* searchObject){
        return searchObject->m_nScreenID == SearchType::kSearchTypeMyLevels 
                || searchObject->m_nScreenID == SearchType::kSearchTypeSavedLevels 
                || searchObject->m_nScreenID == SearchType::kSearchTypeFavorite;
}

bool BetterInfo::isFalseTotal(GJSearchObject* searchObject){
        return searchObject->m_nScreenID == SearchType::kSearchType19
                || searchObject->m_nScreenID == SearchType::kSearchTypeFeatured
                || searchObject->m_nScreenID == SearchType::kSearchTypeHallOfFame;
}

int BetterInfo::levelsPerPage(GJSearchObject* searchObject){
        return ((isLocal(searchObject) && GameManager::sharedState()->getGameVariable("0093")) ? levelsPerPageHigh : levelsPerPageLow);
}

const char* BetterInfo::rankIcon(int position){
        if (position == 1) return "rankIcon_1_001.png";
        else if (position > 1000 || position <= 0) return "rankIcon_all_001.png";
        else if (position <= 10) return "rankIcon_top10_001.png";
        else if (position <= 50) return "rankIcon_top50_001.png";
        else if (position <= 100) return "rankIcon_top100_001.png";
        else if (position <= 200) return "rankIcon_top200_001.png";
        else if (position <= 500) return "rankIcon_top500_001.png";
        return "rankIcon_top1000_001.png";
}
#pragma once

#include <gd.h>

class RewardCell : public gd::TableViewCell {
        gd::GJGameLevel* level;
        void draw() override;
    
    public:
        RewardCell(const char* name, cocos2d::CCSize size);
        void loadFromReward(gd::GJRewardItem* reward);
        static RewardCell* create(const char* key, cocos2d::CCSize size);
};
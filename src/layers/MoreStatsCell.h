#pragma once

#include <gd.h>

class MoreStatsCell : public gd::TableViewCell {
        MoreStatsCell(const char* name, cocos2d::CCSize size);
        void draw() override;
    
    public:
        void loadFromKey(cocos2d::CCString* key);
        static MoreStatsCell* create(const char* key, cocos2d::CCSize size);
};
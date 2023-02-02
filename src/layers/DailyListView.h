#pragma once

#include "CvoltonListView.h"
#include <gd.h>

class DailyListView : public CvoltonListView {
protected:
    void setupList() override;
    gd::TableViewCell* getListCell(const char* key) override;
    void loadCell(gd::TableViewCell* cell, unsigned int index) override;
    
public:
    static cocos2d::CCRect calculateNodeCoverage(cocos2d::CCArray* nodes);
    static DailyListView* create(cocos2d::CCArray* levels, float width, float height);
};
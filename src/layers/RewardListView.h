#pragma once

#include "CvoltonListView.h"
#include <gd.h>

class RewardListView : public CvoltonListView {
protected:
    void setupList() override;
    gd::TableViewCell* getListCell(const char* key) override;
    void loadCell(gd::TableViewCell* cell, unsigned int index) override;
    
public:
    static RewardListView* create(cocos2d::CCArray* rewards, float width, float height);
};
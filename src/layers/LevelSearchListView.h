#pragma once

#include "CvoltonListView.h"
#include <gd.h>

class LevelSearchListView : public CvoltonListView {
protected:
    void setupList() override;
    gd::TableViewCell* getListCell(const char* key) override;
    void loadCell(gd::TableViewCell* cell, unsigned int index) override;
    
public:
    static LevelSearchListView* create(cocos2d::CCArray* levels, float width, float height);
};
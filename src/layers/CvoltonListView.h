#pragma once

#include <gd.h>

/**
 * @brief      This class only exists as a bugfix to CustomListView
 */
class CvoltonListView : public gd::CustomListView {
protected:
    bool init(cocos2d::CCArray* entries, int btype, float width, float height);
    virtual ~CvoltonListView();
};
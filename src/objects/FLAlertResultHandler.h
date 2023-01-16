#pragma once
#include <gd.h>


class FLAlertResultHandler : public cocos2d::CCNode, public gd::FLAlertLayerProtocol {
    gd::InfoLayer* m_layer = nullptr;
public:
    static FLAlertResultHandler* create();
    void FLAlert_Clicked(gd::FLAlertLayer*, bool);
    bool init();
};
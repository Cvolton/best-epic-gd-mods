#pragma once
#include <gd.h>
#include "CvoltonOptionsLayer.h"
#include "../delegates/IDRangeDelegate.h"

class IDRangePopup : public CvoltonAlertLayerStub {
    IDRangeDelegate* delegate;
    gd::CCTextInputNode* minNode = nullptr;
    gd::CCTextInputNode* maxNode = nullptr;
    int additional = 0;
public:
    static IDRangePopup* create(IDRangeDelegate* delegate, int min, int max, const char* text, int additional = 0);
    void onClose(cocos2d::CCObject* sender);
    bool init(IDRangeDelegate* delegate, int min, int max, const char* text, int additional = 0);
    int minID();
    int maxID();
};
#pragma once
#include <gd.h>
#include "CvoltonOptionsLayer.h"
#include "../delegates/DialogCloseDelegate.h"

class ProfileSearchOptionsSongSelect : public CvoltonOptionsLayer {
    DialogCloseDelegate* delegate;
    gd::CCTextInputNode* textNode = nullptr;
public:
    static ProfileSearchOptionsSongSelect* create(DialogCloseDelegate* delegate);
    void onClose(cocos2d::CCObject* sender);
    bool init(DialogCloseDelegate* delegate);
    void destroyToggles();
    void drawToggles();
    int songID();
    void createToggle(const char* option, const char* name, float x, float y);
};
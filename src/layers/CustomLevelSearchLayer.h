#pragma once

#include <gd.h>

class CustomLevelSearchLayer : public gd::FLAlertLayer {
    gd::GJGameLevel* level;
public:
    static CustomLevelSearchLayer* create(gd::GJGameLevel* level);
    void onClose(cocos2d::CCObject* sender);
    void onSimilar(cocos2d::CCObject* sender);
    void onSong(cocos2d::CCObject* sender);
    void onNeighbors(cocos2d::CCObject* sender);
    bool init();
    gd::CCMenuItemSpriteExtra* createButton(cocos2d::CCNode* menu, const char* text, cocos2d::SEL_MenuHandler handler, float x, float y, int width, float height, float scale);
};
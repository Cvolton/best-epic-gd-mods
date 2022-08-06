#pragma once

#include <gd.h>

class LevelCategorySearchAlert : public gd::FLAlertLayer {
public:
    static LevelCategorySearchAlert* create();
    void onClose(cocos2d::CCObject* sender);
    void onPlayed(cocos2d::CCObject* sender);
    void onPercentage(cocos2d::CCObject* sender);
    void onCompleted(cocos2d::CCObject* sender);
    void onOrbs(cocos2d::CCObject* sender);
    void onLeaderboard(cocos2d::CCObject* sender);
    void onCoins(cocos2d::CCObject* sender);
    void onNoCoins(cocos2d::CCObject* sender);
    bool init();
    gd::CCMenuItemSpriteExtra* createButton(cocos2d::CCNode* menu, const char* text, cocos2d::SEL_MenuHandler handler, float x, float y, int width, float height, float scale);
};
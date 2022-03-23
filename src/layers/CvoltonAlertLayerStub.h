#pragma once
#include <cocos2d.h>
#include <gd.h>

class CvoltonAlertLayerStub : public gd::FLAlertLayer {
public:
	//cocos2d::CCMenu* m_pButtonMenu;
	cocos2d::CCPoint alertSize;
	gd::CCMenuItemSpriteExtra* closeBtn;

	bool createBasics(cocos2d::CCPoint contentSize, cocos2d::SEL_MenuHandler onClose, float closeBtnScale = 1);
	void createTitle(std::string text);
	gd::CCMenuItemSpriteExtra* CvoltonAlertLayerStub::createButton(const char* texture, cocos2d::CCPoint position, cocos2d::SEL_MenuHandler callback, float textureScale = 1.0f, float sizeMult = 1.2f);
};
#pragma once
#include <cocos2d.h>
#include <gd.h>

class CvoltonAlertLayerStub : public gd::FLAlertLayer {
public:
	//cocos2d::CCMenu* m_pButtonMenu;
	cocos2d::CCPoint alertSize;
	gd::CCMenuItemSpriteExtra* closeBtn;

	bool createBasics(cocos2d::CCPoint contentSize, cocos2d::SEL_MenuHandler onClose, float closeBtnScale = 1, const cocos2d::ccColor4B& color = {0x00, 0x00, 0x00, 0x4B});
	void createTitle(std::string text, float separatorScale = .75f, float usernameScale = 1);
	gd::CCMenuItemSpriteExtra* CvoltonAlertLayerStub::createButton(const char* texture, cocos2d::CCPoint position, cocos2d::SEL_MenuHandler callback, float textureScale = 1.0f, float sizeMult = 1.2f);
};
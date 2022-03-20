#pragma once
#include <gd.h>

class CvoltonManager : cocos2d::CCNode {
	inline static CvoltonManager* manager = nullptr;
	CvoltonManager();


public:
	bool hasDoneUpdateCheck = false;

	bool init();
	static CvoltonManager* sharedState();
	void onHttpResponse(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
	void processHttpRequest();
	void doUpdateCheck();
};
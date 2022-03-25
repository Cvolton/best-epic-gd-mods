#pragma once
#include <gd.h>
#include "../layers/CvoltonUpdateLayer.h"

class CvoltonManager : cocos2d::CCNode {
	inline static CvoltonManager* cvoltonManager = nullptr;
	CvoltonManager();


public:
	inline static const char* version = "v2.0.0-beta\n";
	inline static const char* modName = "Cvolton's Tweaks";
	std::string latestVer;
	std::string changelog;
	cocos2d::CCDictionary* nameDict;
	cocos2d::CCDictionary* settingsDict;
	bool hasDoneUpdateCheck = false;

	bool init();
	static CvoltonManager* sharedState(){
	    if(cvoltonManager == nullptr){
	        cvoltonManager = new CvoltonManager;
	        cvoltonManager->init();
	        cvoltonManager->retain();
	    }
	    return cvoltonManager;
	}
	void onUpdateHttpResponse(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
	void onChangelogHttpResponse(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
	void doUpdateHttpRequest();
	void doUpdateCheck();
	void downloadChangelog(CvoltonUpdateLayer* updateLayer);
	bool isUpToDate();
	const char* getUserName(int id);
};
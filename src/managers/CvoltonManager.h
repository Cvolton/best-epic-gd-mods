#pragma once
#include <gd.h>

class CvoltonManager : cocos2d::CCNode {
	inline static CvoltonManager* cvoltonManager = nullptr;
	CvoltonManager();


public:
	bool hasDoneUpdateCheck = false;
	cocos2d::CCDictionary* nameDict;

	bool init();
	static CvoltonManager* sharedState(){
	    if(cvoltonManager == nullptr){
	        cvoltonManager = new CvoltonManager;
	        cvoltonManager->init();
	        cvoltonManager->retain();
	    }
	    return cvoltonManager;
	}
	void onHttpResponse(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
	void processHttpRequest();
	void doUpdateCheck();
	const char* getUserName(int id);
};
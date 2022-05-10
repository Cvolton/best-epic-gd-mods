#pragma once
#include <gd.h>
#include "../layers/CvoltonUpdateLayer.h"

enum CompleteMode {
	modeDefault,
	completed,
	completed21,
	completed211,
	allCoins,
	noCoins
};

class CvoltonManager : gd::GManager {
	inline static CvoltonManager* cvoltonManager = nullptr;
	CvoltonManager();


public:
	inline static const char* version = "v2.1.1\n";
	inline static const char* modName = "BetterInfo";
	std::string latestVer;
	std::string changelog;
	cocos2d::CCDictionary* nameDict;
	cocos2d::CCDictionary* settingsDict;
	bool hasDoneUpdateCheck = false;

	bool init();
	void encodeDataTo(DS_Dictionary* data) override;
    void dataLoaded(DS_Dictionary* data) override;
    void firstLoad() override;
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
	void forceUpdateCheck();
	void downloadChangelog(CvoltonUpdateLayer* updateLayer);
	bool isUpToDate();
	bool getOption(std::string option);
	void toggleOption(std::string option);
	int getOptionInt(std::string option);
	void setOptionInt(std::string option, int value);
	const char* getUserName(int id);
	void loadTextures();
	int randomNumber(int start, int end);
	int completedLevelsForStars(int stars);
	cocos2d::CCDictionary* CvoltonManager::responseToDict(std::string response);
};
#pragma once
#include <gd.h>
#include "../layers/CvoltonUpdateLayer.h"

class BetterInfoCache : public gd::GManager, public gd::OnlineListDelegate {
	inline static BetterInfoCache* m_instance = nullptr;
	BetterInfoCache();


public:
	cocos2d::CCDictionary* m_levelNameDict;
	cocos2d::CCDictionary* m_coinCountDict;

	bool init();
	void encodeDataTo(DS_Dictionary* data) override;
    void dataLoaded(DS_Dictionary* data) override;
    void firstLoad() override;
	static BetterInfoCache* sharedState(){
	    if(m_instance == nullptr){
	        m_instance = new BetterInfoCache;
	        m_instance->init();
	        m_instance->retain();
	    }
	    return m_instance;
	}

	void checkDailies();
	void cacheLevel(gd::GJGameLevel* level);
	void cacheLevels(std::set<int> toDownload);

	const char* getLevelName(int levelID);
	int getCoinCount(int levelID);

	void loadListFinished(cocos2d::CCArray*, const char*);
    void loadListFailed(const char*);
    void setupPageInfo(std::string, const char*);
};
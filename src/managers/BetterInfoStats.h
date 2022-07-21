#pragma once
#include <gd.h>
#include "../layers/CvoltonUpdateLayer.h"

class BetterInfoStats : public gd::GManager {
	inline static BetterInfoStats* m_instance = nullptr;
	BetterInfoStats();


public:
	cocos2d::CCDictionary* m_normalDict;
	cocos2d::CCDictionary* m_practiceDict;
	cocos2d::CCDictionary* m_firstPlayedDict;
	cocos2d::CCDictionary* m_lastPlayedDict;

	bool init();
	void encodeDataTo(DS_Dictionary* data) override;
    void dataLoaded(DS_Dictionary* data) override;
    void firstLoad() override;
	static BetterInfoStats* sharedState(){
	    if(m_instance == nullptr){
	        m_instance = new BetterInfoStats;
	        m_instance->init();
	        m_instance->retain();
	    }
	    return m_instance;
	}
	void logCompletion(gd::GJGameLevel* level, bool practice);
	void logCompletion(gd::GJGameLevel* level, bool practice, time_t timestamp);
	time_t getCompletion(gd::GJGameLevel* level, bool practice);
	void logPlay(gd::GJGameLevel* level);
	time_t getPlay(gd::GJGameLevel* level, bool last);
	std::string keyForLevel(gd::GJGameLevel* level);
};
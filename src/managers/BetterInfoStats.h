#pragma once
#include <gd.h>
#include "../layers/CvoltonUpdateLayer.h"

class BetterInfoStats : public gd::GManager {
	inline static BetterInfoStats* m_instance = nullptr;
	BetterInfoStats();


public:
	cocos2d::CCDictionary* m_normalDict;
	cocos2d::CCDictionary* m_practiceDict;

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
	void logCompletion(int levelID, bool practice);
	void logCompletion(int levelID, bool practice, time_t timestamp);
	time_t getCompletion(int levelID, bool practice);
};
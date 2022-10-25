#pragma once
#include <gd.h>
#include <queue>

class BetterInfoScheduler : public cocos2d::CCNode {
	inline static BetterInfoScheduler* m_instance = nullptr;
	BetterInfoScheduler();


public:
	std::queue<gd::GJGameLevel*> m_levelsToSubmit;

	bool init();
	static BetterInfoScheduler* sharedState(){
	    if(m_instance == nullptr){
	        m_instance = new BetterInfoScheduler;
	        m_instance->init();
	        m_instance->retain();
	    }
	    return m_instance;
	}

    void submitLevel(gd::GJGameLevel*);
    void onLevelSubmit(float dt);

};
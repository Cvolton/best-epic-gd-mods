#pragma once
#include <gd.h>
#include "../layers/CvoltonUpdateLayer.h"
#include "../delegates/BILeaderboardDelegate.h"

class BetterInfoOnline : public cocos2d::CCObject {
	inline static BetterInfoOnline* m_instance = nullptr;
	BetterInfoOnline();


public:
	std::map<int, cocos2d::CCArray*> m_scoreDict;
	BILeaderboardDelegate* m_scoreDelegate = nullptr;
	gd::ProfilePage* m_scoreProfilePage = nullptr;

	bool init();
	static BetterInfoOnline* sharedState(){
	    if(m_instance == nullptr){
	        m_instance = new BetterInfoOnline;
	        m_instance->init();
	        m_instance->retain();
	    }
	    return m_instance;
	}

    void loadScores(int accountID);
    void loadScores(int accountID, BILeaderboardDelegate* delegate);
    void loadScores(int accountID, gd::ProfilePage* profilePage);
    void onScoresFinished(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
    void generateScores(const std::string& response, int accountID);
};
#pragma once

#include <gd.h>

class BILeaderboardDelegate {
public:
	virtual void onLeaderboardFinished(cocos2d::CCArray* scores) = 0;
};
#pragma once

class IDRangeDelegate {
public:
	virtual void onIDRangeFinished(int min, int max) = 0;
};
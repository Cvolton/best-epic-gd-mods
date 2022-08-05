#pragma once

class BISearchObjectDelegate {
public:
	virtual void onSearchObjectFinished(const BISearchObject& searchObj) = 0;
};
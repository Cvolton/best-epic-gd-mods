#include "BetterInfoScheduler.h"
#include <gd.h>
#include "../utils.hpp"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;

bool BetterInfoScheduler::init(){
    return true;
}

BetterInfoScheduler::BetterInfoScheduler(){}


void BetterInfoScheduler::submitLevel(GJGameLevel* level){
    level->retain();
    m_levelsToSubmit.push(level);
    this->getScheduler()->scheduleSelector(schedule_selector(BetterInfoScheduler::onLevelSubmit), this, 1, 0, 1, false);
}

void BetterInfoScheduler::onLevelSubmit(CCObject* sender){
    auto level = m_levelsToSubmit.front();
    m_levelsToSubmit.pop();

    LevelLeaderboard::create(level, 0)->show();
    level->release();
}
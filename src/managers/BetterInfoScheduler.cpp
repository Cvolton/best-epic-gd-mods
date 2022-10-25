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

void BetterInfoScheduler::onLevelSubmit(float dt){
    while(!m_levelsToSubmit.empty()) {
        auto level = m_levelsToSubmit.front();
        m_levelsToSubmit.pop();

        GameLevelManager::sharedState()->getLevelLeaderboard(level, 0);
        level->release();
    }
}
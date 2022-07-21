#include "BetterInfoStats.h"
#include <gd.h>
#include "../utils.hpp"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;

bool BetterInfoStats::init(){
    bool init = CCNode::init();
    if(!init) return false;

    this->m_sFileName = "CCBetterInfoStats.dat";

    auto FU = CCFileUtils::sharedFileUtils();

    m_normalDict = CCDictionary::create();
    m_normalDict->retain();

    m_practiceDict = CCDictionary::create();
    m_practiceDict->retain();

    m_firstPlayedDict = CCDictionary::create();
    m_firstPlayedDict->retain();

    m_lastPlayedDict = CCDictionary::create();
    m_lastPlayedDict->retain();

    this->setup();

    return true;
}

BetterInfoStats::BetterInfoStats(){}

void BetterInfoStats::encodeDataTo(DS_Dictionary* data) {
    data->setDictForKey("normalCompletions", m_normalDict);
    data->setDictForKey("practiceCompletions", m_practiceDict);
    data->setDictForKey("firstPlayed", m_firstPlayedDict);
    data->setDictForKey("lastPlayed", m_lastPlayedDict);
}

void BetterInfoStats::dataLoaded(DS_Dictionary* data) {
    auto normalDict = static_cast<CCDictionary*>(data->getObjectForKey("normalCompletions"));
    if(normalDict) {
        m_normalDict->release();
        m_normalDict = normalDict;
        m_normalDict->retain();
    }

    auto practiceDict = static_cast<CCDictionary*>(data->getObjectForKey("practiceCompletions"));
    if(practiceDict) {
        m_practiceDict->release();
        m_practiceDict = practiceDict;
        m_practiceDict->retain();
    }

    auto firstPlayedDict = static_cast<CCDictionary*>(data->getObjectForKey("firstPlayed"));
    if(firstPlayedDict) {
        m_firstPlayedDict->release();
        m_firstPlayedDict = firstPlayedDict;
        m_firstPlayedDict->retain();
    }

    auto lastPlayedDict = static_cast<CCDictionary*>(data->getObjectForKey("lastPlayed"));
    if(lastPlayedDict) {
        m_lastPlayedDict->release();
        m_lastPlayedDict = lastPlayedDict;
        m_lastPlayedDict->retain();
    }

    this->save();
}

void BetterInfoStats::firstLoad() {
    std::cout << "firstLoad";

    this->save();
}

void BetterInfoStats::logCompletion(GJGameLevel* level, bool practice) {
    if(getCompletion(level, practice) != 0) return;

    logCompletion(level, practice, std::time(nullptr));
}

void BetterInfoStats::logCompletion(GJGameLevel* level, bool practice, time_t timestamp) {
    auto dict = practice ? m_practiceDict : m_normalDict;
    dict->setObject(CCString::create(std::to_string(timestamp).c_str()), keyForLevel(level));
    
    this->save();
}

time_t BetterInfoStats::getCompletion(GJGameLevel* level, bool practice) {
    auto dict = practice ? m_practiceDict : m_normalDict;

    auto ret = dict->valueForKey(keyForLevel(level));
    if(std::string_view(ret->getCString()).empty()) return 0;
    try {
        return std::strtol(ret->getCString(), nullptr, 10);
    } catch(...) {
        return 0;
    }
}

void BetterInfoStats::logPlay(GJGameLevel* level) {
    auto idString = keyForLevel(level);
    auto timeString = CCString::create(std::to_string(std::time(nullptr)).c_str());
    m_lastPlayedDict->setObject(timeString, idString);
    if(getPlay(level, false) == 0 && level->normalPercent <= 0) m_firstPlayedDict->setObject(timeString, idString);
    
    this->save();
}

time_t BetterInfoStats::getPlay(GJGameLevel* level, bool last) {
    auto dict = last ? m_lastPlayedDict : m_firstPlayedDict;

    auto ret = dict->valueForKey(keyForLevel(level));
    if(std::string_view(ret->getCString()).empty()) return 0;
    try {
        return std::strtol(ret->getCString(), nullptr, 10);
    } catch(...) {
        return 0;
    }
}

std::string BetterInfoStats::keyForLevel(GJGameLevel* level) {
    return std::format("{}_{}_{}_{}", level->levelID, static_cast<int>(level->levelType), level->dailyID, level->gauntletLevel);
}
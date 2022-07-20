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

    this->setup();

    return true;
}

BetterInfoStats::BetterInfoStats(){}

void BetterInfoStats::encodeDataTo(DS_Dictionary* data) {
    data->setDictForKey("normalCompletions", m_normalDict);
    data->setDictForKey("practiceCompletions", m_practiceDict);
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

    this->save();
}

void BetterInfoStats::firstLoad() {
    std::cout << "firstLoad";

    this->save();
}

void BetterInfoStats::logCompletion(int levelID, bool practice) {
    if(getCompletion(levelID, practice) != 0) return;

    logCompletion(levelID, practice, std::time(nullptr));
}

void BetterInfoStats::logCompletion(int levelID, bool practice, time_t timestamp) {
    auto dict = practice ? m_practiceDict : m_normalDict;
    auto idString = std::to_string(levelID);
    dict->setObject(CCString::create(std::to_string(timestamp).c_str()), idString);
    
    this->save();
}

time_t BetterInfoStats::getCompletion(int levelID, bool practice) {
    auto dict = practice ? m_practiceDict : m_normalDict;

    auto ret = dict->valueForKey(std::to_string(levelID));
    if(std::string_view(ret->getCString()).empty()) return 0;
    try {
        return std::strtol(ret->getCString(), nullptr, 10);
    } catch(...) {
        return 0;
    }
}
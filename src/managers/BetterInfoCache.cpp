#include "BetterInfoCache.h"
#include <gd.h>
#include "../utils.hpp"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;

bool BetterInfoCache::init(){
    bool init = CCNode::init();
    if(!init) return false;

    this->m_sFileName = "CCBetterInfoCache.dat";

    auto FU = CCFileUtils::sharedFileUtils();

    m_levelNameDict = CCDictionary::create();
    m_levelNameDict->retain();

    m_coinCountDict = CCDictionary::create();
    m_coinCountDict->retain();

    this->setup();

    std::cout << "initing cache" << std::endl;

    checkDailies();

    return true;
}

BetterInfoCache::BetterInfoCache(){}

void BetterInfoCache::encodeDataTo(DS_Dictionary* data) {
    data->setDictForKey("levelNames", m_levelNameDict);
    data->setDictForKey("coinCounts", m_coinCountDict);
}
void BetterInfoCache::dataLoaded(DS_Dictionary* data) {
    auto levelNameDict = static_cast<CCDictionary*>(data->getObjectForKey("levelNames"));
    if(levelNameDict) {
        m_levelNameDict->release();
        m_levelNameDict = levelNameDict;
        m_levelNameDict->retain();
    }


    auto coinCounts = static_cast<CCDictionary*>(data->getObjectForKey("coinCounts"));
    if(coinCounts) {
        m_coinCountDict->release();
        m_coinCountDict = coinCounts;
        m_coinCountDict->retain();
    }

    this->save();
}
void BetterInfoCache::firstLoad() {
    std::cout << "firstLoad";

    this->save();
}

void BetterInfoCache::checkDailies() {
    auto GLM = GameLevelManager::sharedState();

    auto dailyLevels = GLM->m_dailyLevels;
    CCDictElement* obj;
    CCDICT_FOREACH(dailyLevels, obj){
        auto currentLvl = static_cast<GJGameLevel*>(obj->getObject());
        if(currentLvl == nullptr) continue;

        auto idString = std::to_string(currentLvl->levelID);
        if(m_levelNameDict->objectForKey(idString) && m_coinCountDict->objectForKey(idString)) continue;

        auto levelFromSaved = static_cast<GJGameLevel*>(GLM->m_onlineLevels->objectForKey(std::to_string(currentLvl->levelID).c_str()));
        if(levelFromSaved != nullptr) {
            m_levelNameDict->setObject(CCString::create(levelFromSaved->levelName.c_str()), idString);
            m_coinCountDict->setObject(CCString::createWithFormat("%i", currentLvl->coins), idString);
        }
    }

    this->save();
}

const char* BetterInfoCache::getLevelName(int levelID) {
    auto ret = m_levelNameDict->valueForKey(std::to_string(levelID));
    if(std::string_view(ret->getCString()).empty()) return "Unknown";
    return ret->getCString();
}

int BetterInfoCache::getCoinCount(int levelID) {
    auto ret = m_coinCountDict->valueForKey(std::to_string(levelID));
    if(std::string_view(ret->getCString()).empty()) return 3;
    return ret->intValue();
}
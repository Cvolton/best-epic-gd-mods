#include "BetterInfoCache.h"
#include <gd.h>
#include "../utils.hpp"

#include <fstream>

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

    m_demonDifficultyDict = CCDictionary::create();
    m_demonDifficultyDict->retain();

    this->setup();

    checkDailies();

    return true;
}

BetterInfoCache::BetterInfoCache(){}

void BetterInfoCache::encodeDataTo(DS_Dictionary* data) {
    data->setDictForKey("levelNames", m_levelNameDict);
    data->setDictForKey("coinCounts", m_coinCountDict);
    data->setDictForKey("demonDifficulties", m_demonDifficultyDict);
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

    auto demonDifficulties = static_cast<CCDictionary*>(data->getObjectForKey("demonDifficulties"));
    if(demonDifficulties) {
        m_demonDifficultyDict->release();
        m_demonDifficultyDict = demonDifficulties;
        m_demonDifficultyDict->retain();
    }

    this->save();
}
void BetterInfoCache::firstLoad() {
    this->save();
}

void BetterInfoCache::checkDailies() {
    std::set<int> toDownload;

    auto GLM = GameLevelManager::sharedState();

    auto dailyLevels = GLM->m_dailyLevels;
    CCDictElement* obj;
    CCDICT_FOREACH(dailyLevels, obj){
        auto currentLvl = static_cast<GJGameLevel*>(obj->getObject());
        if(currentLvl == nullptr) continue;

        auto idString = std::to_string(currentLvl->levelID);
        if(m_levelNameDict->objectForKey(idString) && m_coinCountDict->objectForKey(idString) && getLevelName(currentLvl->levelID) != "") continue;

        auto levelFromSaved = static_cast<GJGameLevel*>(GLM->m_onlineLevels->objectForKey(std::to_string(currentLvl->levelID).c_str()));
        if(levelFromSaved != nullptr && levelFromSaved->levelName != "") cacheLevel(levelFromSaved);
        else toDownload.insert(currentLvl->levelID);
    }

    if(!toDownload.empty()) cacheLevels(toDownload);

    this->save();
}

void BetterInfoCache::cacheLevel(GJGameLevel* level) {
    auto idString = std::to_string(level->levelID);
    m_levelNameDict->setObject(CCString::create(level->levelName.c_str()), idString);
    m_coinCountDict->setObject(CCString::createWithFormat("%i", level->coins), idString);
    m_demonDifficultyDict->setObject(CCString::createWithFormat("%i", level->demonDifficulty), idString);
}

void BetterInfoCache::cacheLevels(std::set<int> toDownload) {
    //Search type 10 currently does not have a limit on level IDs, so we can do this all in one request
    bool first = true;
    std::stringstream levels;
    std::vector<std::string> levelSets;
    int i = 0;
    for(const auto& id : toDownload) {
        if(i != 0) levels << ",";
        levels << id;
        first = false;
        
        i = (i + 1) % 300;
        if(i == 0) {
            levelSets.push_back(levels.str());
            levels.str("");
        }
    }

    levelSets.push_back(levels.str());

    //Splitting up the request like this is required because GJSearchObject::create crashes if str is too long
    for(const auto& set : levelSets) {
        auto searchObj = GJSearchObject::create(kSearchTypeMapPackList, set);
        auto GLM = GameLevelManager::sharedState();
        GLM->m_pOnlineListDelegate = this;
        GLM->getOnlineLevels(searchObj);
    }

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

int BetterInfoCache::getDemonDifficulty(int levelID) {
    auto ret = m_demonDifficultyDict->valueForKey(std::to_string(levelID));
    if(std::string_view(ret->getCString()).empty()) return 3;
    return ret->intValue();
}

void BetterInfoCache::loadListFinished(cocos2d::CCArray* levels, const char*) {
    for(size_t i = 0; i < levels->count(); i++) {
        auto level = static_cast<GJGameLevel*>(levels->objectAtIndex(i));
        if(level == nullptr) continue;

        cacheLevel(level);
    }

    this->save();
}

void BetterInfoCache::loadListFailed(const char*) {}
void BetterInfoCache::setupPageInfo(std::string, const char*) {}

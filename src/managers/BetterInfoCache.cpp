#include "BetterInfoCache.h"
#include <gd.h>
#include <random>
#include <filesystem>
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
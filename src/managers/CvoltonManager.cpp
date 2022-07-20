#include "CvoltonManager.h"
#include "BetterInfoCache.h"
#include <gd.h>
#include <random>
#include <filesystem>
#include "../utils.hpp"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;

const char* sheets[1] = {"BI_GameSheet"};
const char* textures[3] = {"BI_mainButton_001", "BI_blankBtn_001", "BI_updateArrow_001"};

bool CvoltonManager::init(){
    bool init = CCNode::init();
    if(!init) return false;

    this->m_sFileName = "CCBetterInfo.dat";

    auto FU = CCFileUtils::sharedFileUtils();

    std::string plistPath(CCFileUtils::sharedFileUtils()->fullPathForFilename("BI_destroyedUsers.plist", false));
    if(FU->isFileExist(plistPath)) {
        nameDict = CCDictionary::createWithContentsOfFile("BI_destroyedUsers.plist");
        plistLoaded = true;
    }
    else {
        nameDict = CCDictionary::create();
    }
    nameDict->retain();

    settingsDict = CCDictionary::create();
    settingsDict->retain();

    this->setup();

    std::cout << "initing" << std::endl;

    BetterInfoCache::sharedState();

    return true;
}

void CvoltonManager::doUpdateHttpRequest(){
	CCHttpRequest* request = new CCHttpRequest;
    request->setUrl("https://cvolton.eu/gdmods/api/betterinfo/version/");
    request->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
    request->setResponseCallback(this, httpresponse_selector(CvoltonManager::onUpdateHttpResponse));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void CvoltonManager::onUpdateHttpResponse(CCHttpClient* client, CCHttpResponse* response){
    if(!(response->isSucceed())) return;

    std::vector<char>* responseData = response->getResponseData();
    std::string responseString(responseData->begin(), responseData->end());
    latestVer = responseString;

    if(isUpToDate()) return;

    /*std::ostringstream stream;
    stream << modNameColored << " " << responseString;*/


    //AchievementNotifier::sharedState()->notifyAchievement("Update available", stream.str().c_str(), "", false);
    //FLAlertLayer::create(nullptr, "User Info", "OK", nullptr, 300, stream.str().c_str())->show();
    //BetterInfo::showBIExclamationMark(reinterpret_cast<CCLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0)));

}

void CvoltonManager::onChangelogHttpResponse(CCHttpClient* client, CCHttpResponse* response){
    CvoltonUpdateLayer* updateLayer = static_cast<CvoltonUpdateLayer*>(response->getHttpRequest()->getUserData());

    if(!(response->isSucceed())){
        updateLayer->onLoadFailed();
        return;
    }

    std::vector<char>* responseData = response->getResponseData();
    std::string responseString(responseData->begin(), responseData->end());
    changelog = responseString;

    updateLayer->onLoadFinished();
    updateLayer->release();
    //FLAlertLayer::create(nullptr, "User Info", "OK", nullptr, 300, stream.str().c_str())->show();
}

//CvoltonManager* CvoltonManager::sharedState()

CvoltonManager::CvoltonManager(){}

/*void CvoltonManager::doUpdateCheck(){
    if(getOption("no_update_check")) return;

    forceUpdateCheck();
}*/

void CvoltonManager::forceUpdateCheck(){
    if(hasDoneUpdateCheck) return;

    doUpdateHttpRequest();
    hasDoneUpdateCheck = true;
}

const char* CvoltonManager::getUserName(int id){
    auto obj = dynamic_cast<CCString*>(nameDict->objectForKey(std::to_string(id)));
    if(obj == nullptr) return "";
    return obj->getCString();
}

void CvoltonManager::downloadChangelog(CvoltonUpdateLayer* updateLayer){
    updateLayer->retain();
    forceUpdateCheck();

    CCHttpRequest* request = new CCHttpRequest;
    request->setUrl("https://cvolton.eu/gdmods/api/betterinfo/changelog/latest/");
    request->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
    request->setUserData(updateLayer);
    request->setResponseCallback(this, httpresponse_selector(CvoltonManager::onChangelogHttpResponse));
    CCHttpClient::getInstance()->send(request);
    //request->release();

}

bool CvoltonManager::isUpToDate(){
    if(latestVer == "") return true;
    return version == latestVer;
}

void CvoltonManager::encodeDataTo(DS_Dictionary* data) {
    data->setDictForKey("settings", settingsDict);
    data->setStringForKey("versionString", version);

    std::cout << "encodeDataTo";
}
void CvoltonManager::dataLoaded(DS_Dictionary* data) {
    auto settingsDictLoaded = static_cast<CCDictionary*>(data->getObjectForKey("settings"));
    if(settingsDictLoaded) {
        settingsDict->release();
        settingsDict = settingsDictLoaded;
        settingsDict->retain();
    }

    saveVer = data->getStringForKey("versionString");

    this->save();
}
void CvoltonManager::firstLoad() {
    std::cout << "firstLoad";

    this->save();
}
bool CvoltonManager::getOption(std::string option){
    return settingsDict->valueForKey(option)->boolValue();
}
bool CvoltonManager::toggleOption(std::string option){
    bool toggled = !getOption(option);
    settingsDict->setObject(CCString::createWithFormat("%i", toggled), option);
    return toggled;
}
bool CvoltonManager::setOption(std::string option, bool value){
    settingsDict->setObject(CCString::createWithFormat("%i", value), option);
    return value;
}

int CvoltonManager::getOptionInt(std::string option){
    return settingsDict->valueForKey(option)->intValue();
}
void CvoltonManager::setOptionInt(std::string option, int value){
    settingsDict->setObject(CCString::createWithFormat("%i", value), option);
}

void CvoltonManager::loadTextures(){
    CCTextureCache::sharedTextureCache()->addImage("BI_GameSheet.png", 0);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("BI_GameSheet.plist");
}

int CvoltonManager::randomNumber(int start, int end){
    std::random_device os_seed;
    const unsigned int seed = os_seed();

    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribute(start, end);

    return distribute(generator);
}

int CvoltonManager::completedLevelsForStars(int stars){
    //TODO: do not iterate this 10 times for values 0-10
    int res = 0;
    auto GLM = GameLevelManager::sharedState();
    auto levels = GLM->m_onlineLevels;
    CCDictElement* obj;
    CCDICT_FOREACH(levels, obj){
        auto currentLvl = static_cast<GJGameLevel*>(obj->getObject());
        if(currentLvl != nullptr && currentLvl->stars == stars && currentLvl->normalPercent == 100) res++;
    }

    auto dailyLevels = GLM->m_dailyLevels;
    CCDICT_FOREACH(dailyLevels, obj){
        auto currentLvl = static_cast<GJGameLevel*>(obj->getObject());
        if(currentLvl != nullptr && currentLvl->stars == stars && currentLvl->normalPercent == 100) res++;
    }

    auto gauntletLevels = GLM->m_gauntletLevels;
    CCDICT_FOREACH(gauntletLevels, obj){
        auto currentLvl = static_cast<GJGameLevel*>(obj->getObject());
        if(currentLvl != nullptr && currentLvl->stars == stars && currentLvl->normalPercent == 100) res++;
    }

    return res;
}

/*
    This is a reimplementation of GameLevelManager::responseToDict
    because I couldn't get it to work. It's not 1:1 with the original
    so that's why it's here, even though it achieves the same purpose.
*/
CCDictionary* CvoltonManager::responseToDict(std::string response){
    auto dict = CCDictionary::create();

    std::stringstream responseStream(response);
    std::string currentKey;
    std::string keyID;

    unsigned int i = 0;
    while(getline(responseStream, currentKey, ':')){

        if(i % 2 == 0) keyID = currentKey;
        else dict->setObject(CCString::create(currentKey.c_str()),keyID);

        i++;
    }

    return dict;
}

FLAlertLayer* CvoltonManager::missingResourcesError() {
    if(hasDoneHealthCheck) return nullptr;

    auto FU = CCFileUtils::sharedFileUtils();
    std::ostringstream stream;
    if(!plistLoaded) stream << "\nBI_destroyedUsers.plist";
    for(auto sheet : sheets){
        const char* extensions[] = {"", "-hd", "-uhd"};
        for(auto extension : extensions){
            std::string plistPath(
                CCFileUtils::sharedFileUtils()->fullPathForFilename(
                    CCString::createWithFormat("%s%s.plist", sheet, extension)->getCString(), false
                )
            );
            std::string pngPath(
                CCFileUtils::sharedFileUtils()->fullPathForFilename(
                    CCString::createWithFormat("%s%s.png", sheet, extension)->getCString(), false
                )
            );
            if(!(FU->isFileExist(plistPath))) stream << "\n" << sheet << extension << ".plist";
            if(!(FU->isFileExist(pngPath))) stream << "\n" << sheet << extension << ".png";
        }
    }

    for(auto texture : textures){
        const char* extensions[] = {"", "-hd", "-uhd"};
        for(auto extension : extensions){
            std::string pngPath(
                CCFileUtils::sharedFileUtils()->fullPathForFilename(
                    CCString::createWithFormat("%s%s.png", texture, extension)->getCString(), false
                )
            );
            if(!(FU->isFileExist(pngPath))) stream << "\n" << texture << extension << ".png";
        }
    }

    std::string file_list(stream.str());
    if(file_list.empty()) return nullptr;

    auto layer = FLAlertLayer::create(nullptr, modName, "OK", nullptr, 400, true, 300, "<cr>Error: Missing resources!</c>\n\nUnable to find:<cl>"+file_list+"</c>\n\nPlease refer to the <cg>installation instructions</c>.");

    hasDoneHealthCheck = true;

    return layer;
}

FLAlertLayer* CvoltonManager::updateCompleteDialog(bool forced) {
    if((saveVer == version || shownUpdateDialog) && !forced) return nullptr;

    shownUpdateDialog = true;

    auto layer = FLAlertLayer::create(
        nullptr,
        modName,
        "OK",
        nullptr,
        400,
        true,
        300,
        "<cg>BetterInfo has updated!</c>\n"
        "\n"
        "<cy>Changelog:</c> <cg>(v2.3.1 - 2022-06-28)</c>\n"
        "- Added <cj>even more</c> <co>Saved Filters</c>\n"
        "- Added a <co>percentage</c> <cj>Completed Mode</c>\n"
        "- Bugfixes & improvements\n"
        "\n"
        "<cy>Changelog:</c> <cg>(v2.3.0 - 2022-06-26)</c>\n"
        "- Added <cj>Claimed Chest</c> <co>history</c>\n"
        "- Added <cj>more</c> <cj>Saved Filters</c>\n"
        "- Added <co>Live</c> <cj>Comment Chat</c> <co>refreshing</c>\n"
        "- Added <co>more stuff</c> to <cj>Extended Level Info</c>\n"
        "- Added a <cj>white level ID option</c>"
    );

    this->save();

    return layer;
}
#include "CvoltonManager.h"
#include "BetterInfoCache.h"
#include <gd.h>
#include <random>
#include <filesystem>
#include "../utils.hpp"
#include "../layers/DailyListView.h"
#include "../layers/PaginatedFLAlert.h"

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

    BetterInfoCache::sharedState();

    return true;
}

void CvoltonManager::doUpdateHttpRequest(){
	CCHttpRequest* request = new CCHttpRequest;
    request->setUrl("https://geometrydash.eu/mods/betterinfo/v2/stable/version.txt");
    request->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
    request->setResponseCallback(this, httpresponse_selector(CvoltonManager::onUpdateHttpResponse));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void CvoltonManager::onUpdateHttpResponse(CCHttpClient* client, CCHttpResponse* response){
    if(!(response->isSucceed())) {
        if(updateLayer) updateLayer->onLoadFailed();
        return;
    }

    std::vector<char>* responseData = response->getResponseData();
    std::string responseString(responseData->begin(), responseData->end());
    latestVer = responseString;

    if(updateLayer) updateLayer->onLoadFinished();

    //if(isUpToDate()) return;

    /*std::ostringstream stream;
    stream << modNameColored << " " << responseString;*/


    //AchievementNotifier::sharedState()->notifyAchievement("Update available", stream.str().c_str(), "", false);
    //FLAlertLayer::create(nullptr, "User Info", "OK", nullptr, 300, stream.str().c_str())->show();
    //BetterInfo::showBIExclamationMark(reinterpret_cast<CCLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0)));

}

/*void CvoltonManager::onChangelogHttpResponse(CCHttpClient* client, CCHttpResponse* response){
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
}*/

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

void CvoltonManager::downloadChangelog(){
    changelog = "Click the button in the lower left corner to see changelogs";
    forceUpdateCheck();

    /*CCHttpRequest* request = new CCHttpRequest;
    request->setUrl("https://cvolton.eu/gdmods/api/betterinfo/changelog/latest/");
    request->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
    request->setUserData(updateLayer);
    request->setResponseCallback(this, httpresponse_selector(CvoltonManager::onChangelogHttpResponse));
    CCHttpClient::getInstance()->send(request);*/
    //request->release();

}

bool CvoltonManager::isUpToDate(){
    if(latestVer == "") return true;
    return version == latestVer;
}

void CvoltonManager::encodeDataTo(DS_Dictionary* data) {
    data->setDictForKey("settings", settingsDict);
    data->setStringForKey("versionString", version);
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

    std::vector<std::string> versions;
    versions.push_back(
        "<cg>BetterInfo has updated!</c>\n"
        "\n"
        "<cy>Changelog:</c> <cg>(v2.4.10 - 2023-12-09)</c>\n"
        "- <cg>Added</c> <cj>demon difficulty</c> to <co>Weekly History</c>\n"
        "- <cg>Added</c> <cj>highlighting</c> to <co>View Leaderboard As</c>\n"
        "- <cg>Fixed</c> <cj>level names</c> in <co>Daily History</c>\n"
        "- <cg>Improved</c> <co>GDPS support</c>\n"
        "- Other bugfixes & improvements\n"
        "\n"
        "Remember how 2.2 was supposed to be in October...\n"
        "Sorry, I mean November..."
    );

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.4.9 - 2023-10-18)</c>\n"
        "- <cg>Fixed</c> <co>chest history</c> <cj>icons</c>\n"
        "- <cg>Fixed</c> <cj>submitting coins</c> to old <co>daily levels leaderboards</c>\n"
        "\n"
        "BetterInfo is now also available on Geode with a few more minor features added in.\n\n"
        "Thank you all for using BI, see you in 2.2 :) --Cvolton"
    );

    /*versions.push_back(
        "<cr>A few more words from me if you wanna read that:TM:</c>\n"
        "Really this was supposed to be there on the previous page but FLAlerts are terribly small, so I had to split it up. Welp, at least I can make it \"a bit\" longer lol\n\nReleasing this update is kind of a weird moment to me because I know this is the last update the standalone version of BI will ever get. This, of course, doesn't mean the end of the mod as a whole - none of the features here are getting taken away and I will keep updating the Geode version with new stuff. <co>(1/3)</c>"
    );

    versions.push_back(
        "<cr>A few more words from me if you wanna read that:TM:</c>\n"
        "The Geode version will also become the only version once 2.2 rolls around - there won't be a standalone dll for that version. The version you're using now is effectively abandoned though, unless I get a report of some important game crashing bug or something.\n\nThank you all for using this little mod that I effectively made for myself because I wanted to see these features in the game. I've counted over 25,000 users recently, isn't that crazy? (no, I do not know who exactly) <co>(2/3)</c>"
    );

    versions.push_back(
        "<cr>A few more words from me if you wanna read that:TM:</c>\n"
        "Big stuff is coming for BI (hopefully if I stay motivated), even despite 2.2 hopefully coming out soon :) See you in the update --Cvolton <co>(3/3)</c>\n\n"
        "The next pages are changelogs again."
    );*/

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.4.8 - 2023-08-19)</c>\n"
        "- Fixed a potential crash caused by filtered level search"
    );

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.4.7 - 2023-07-30)</c>\n"
        "- Removed \"Name Contains\", \"No ID Search\" and \"No Forced Star\" due to server-side changes\n"
        "- Removed \"GDW Most Liked\" due to server-side changes\n"
        "- Using a faster search method for some filtered searches"
    );

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.4.6 - 2023-06-11)</c>\n"
        "- Added \"no coins\" filter (thanks _tz and RandomB)\n"
        "- Bugfixes & improvements\n"
    );

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.4.5 - 2023-02-03)</c>\n"
        "- Songs not served from NG highlighted in orange/red/brown/pink/whatever that color is now (idk its a weird shade, i copied it from gdbrowser)\n"
        "- Fixed a crash when exiting user leaderboards\n"
        "- Fixed a crash when exiting filtered level search"
    );

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.4.4 - 2023-01-16)</c>\n"
        "- Added visible comment IDs (optional)\n"
        "- Fixed a crash when viewing some level descriptions\n"
        "- Bugfixes & improvements"
    );

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.4.3 - 2022-11-21)</c>\n"
        "- Increased delay between requests to reduce/prevent rate-limit IP bans\n"
        "- Bugfixes & improvements"
    );

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.4.2 - 2022-10-26)</c>\n"
        "- <co>RobTop Bug Fix:</c> Made <cr>unloadable</c> profiles <cg>loadable</c>\n(but still not searchable)\n"
        "- <cy>Delayed</c> <cj>auto submit to leaderboards</c> to <co>level exit</c> (+1s)\n"
        "- <cg>Added</c> a <co>time left indicator</c> to <cj>Finding Last Page</c>\n"
        "- Bugfixes & improvements"
    );

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.4.1 - 2022-08-24)</c>\n"
        "- <cg>Fixed</c> <cr>spacebar crash</c> in <cj>paginated alerts</c>\n"
        "- <cg>Added</c> the ability to <co>change pages</c> using <cy>left/right keys</c> in <cj>paginated alerts</c>\n"
        "- Click <cy>level IDs</c> in <co>created levels</c> to go to the <cj>uploaded version</c> of the level\n"
        "- Bugfixes & improvements"
    );

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.4.0 - 2022-08-17)</c>\n"
        "- <cg>Added</c> <co>custom</c> <cj>level search filtering</c>\n"
        "- <cg>Added</c> <co>played & completed</c> <cj>level search</c>\n"
        "- <cj>No ID search</c> <cg>can</c> now be <co>combined</c> with other BI filters\n"
        "- Bugfixes & improvements"
    );

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.3.3 - 2022-07-31)</c>\n"
        "- <cg>Added</c> separate <cj>normal & practice mode</c> <co>attempts</c>\n"
        "- <cg>Replaced</c> <cj>scrolling alerts</c> with <co>paginated</c> ones\n"
        "- <cg>Fixed</c> a <cr>startup crash</c> for people with\n<cl>more</c> than ~450 <co>daily levels</c>\n"
        "- <cg>Fixed</c> <cj>auto submit to leaderboards</c>\n<cr>not</c> submitting coins\n"
        "- <cg>:)</c>"
    );

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.3.2 - 2022-07-22)</c>\n"
        "- Added <cj>even more</c> <co>Saved Filters</c> (again)\n"
        "- Added <cj>first/last played, completed dates</c> to <co>Level Stats</c>\n"
        "- Added <cj>auto submit to level leaderboards</c> (<cr>disabled</c> by default, <cg>enable</c> in the BI settings)\n"
        "- Bugfixes & improvements"
    );

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.3.1 - 2022-06-28)</c>\n"
        "- Added <cj>even more</c> <co>Saved Filters</c>\n"
        "- Added a <co>percentage</c> <cj>Completed Mode</c>\n"
        "- Bugfixes & improvements\n"
    );

    versions.push_back(
        "<cy>Changelog:</c> <cg>(v2.3.0 - 2022-06-26)</c>\n"
        "- Added <cj>Claimed Chest</c> <co>history</c>\n"
        "- Added <cj>more</c> <cj>Saved Filters</c>\n"
        "- Added <co>Live</c> <cj>Comment Chat</c> <co>refreshing</c>\n"
        "- Added <co>more stuff</c> to <cj>Extended Level Info</c>\n"
        "- Added a <cj>white level ID option</c>"
    );

    auto layer = PaginatedFLAlert::create(modName, versions);
    
    this->save();

    return layer;
}

FLAlertLayer* CvoltonManager::incompatibleModDialog() {
    std::string mods;
    if(GetModuleHandle("more-button.dll") != nullptr) mods.append("- more-button.dll\n");

    if(mods.empty()) return nullptr;

    auto layer = FLAlertLayer::create(nullptr, modName, "OK", nullptr, 400, false, 300, std::format("<cr>Error: Incompatible mods detected!</c>\n{}\n\nBetterInfo will try to function as normal but keeping these enabled may result in degraded experience and crashes", mods));
    return layer;
}
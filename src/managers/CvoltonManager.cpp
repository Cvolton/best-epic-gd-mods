#include "CvoltonManager.h"
#include <gd.h>

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;

bool CvoltonManager::init(){
    bool init = CCNode::init();
    if(!init) return false;

    this->m_sFileName = "CCCvoltonManager.dat";

    nameDict = CCDictionary::createWithContentsOfFile("CV_destroyedUsers.plist");
    nameDict->retain();

    settingsDict = CCDictionary::create();
    settingsDict->retain();

    this->setup();

    std::cout << "initing" << std::endl;

    return true;
}

void CvoltonManager::doUpdateHttpRequest(){
	CCHttpRequest* request = new CCHttpRequest;
    request->setUrl("https://cvolton.eu/gdmods/api/more-button/version/");
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

    std::ostringstream stream;
    stream << modName << " " << responseString;


    AchievementNotifier::sharedState()->notifyAchievement("Update available", stream.str().c_str(), "", false);
    //FLAlertLayer::create(nullptr, "User Info", "OK", nullptr, 300, stream.str().c_str())->show();
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

void CvoltonManager::doUpdateCheck(){
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
    doUpdateCheck();

    CCHttpRequest* request = new CCHttpRequest;
    request->setUrl("https://cvolton.eu/gdmods/api/more-button/changelog/latest/");
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
    settingsDict->release();
    settingsDict = static_cast<CCDictionary*>(data->getObjectForKey("settings"));
    settingsDict->retain();

    std::cout << "dataLoaded - " << settingsDict->valueForKey("cv_test")->getCString();

    this->save();
}
void CvoltonManager::firstLoad() {
    settingsDict->setObject(CCString::createWithFormat("%i", true), "cv_test");

    std::cout << "firstLoad";

    this->save();
}
bool CvoltonManager::getOption(std::string option){
    return settingsDict->valueForKey(option)->boolValue();
}
void CvoltonManager::toggleOption(std::string option){
    settingsDict->setObject(CCString::createWithFormat("%i", !getOption(option)), option);

    this->save();
}
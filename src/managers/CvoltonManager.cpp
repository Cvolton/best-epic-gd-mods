#include "CvoltonManager.h"
#include <gd.h>

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;

const char* version = "v2.0.0-beta\n";

bool CvoltonManager::init(){
    bool init = CCNode::init();
    if(!init) return false;

    return true;
}

void CvoltonManager::processHttpRequest(){
	CCHttpRequest* request = new CCHttpRequest;
    request->setUrl("https://cvolton.eu/gdmods/api/more-button/version/");
    request->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
    request->setResponseCallback(this, httpresponse_selector(CvoltonManager::onHttpResponse));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void CvoltonManager::onHttpResponse(CCHttpClient* client, CCHttpResponse* response){
    if(!(response->isSucceed())) return;

    std::vector<char>* responseData = response->getResponseData();
    std::string responseString(responseData->begin(), responseData->end());

    if(responseString == version) return;

    std::ostringstream stream;
    stream << "Cvolton's Epic Mod " << responseString;

    AchievementNotifier::sharedState()->notifyAchievement("Update available", stream.str().c_str(), "", false);
    //FLAlertLayer::create(nullptr, "User Info", "OK", nullptr, 300, stream.str().c_str())->show();
}

CvoltonManager* CvoltonManager::sharedState(){
    if(manager == nullptr){
        manager = new CvoltonManager;
        manager->init();
    }
    return manager;
}

CvoltonManager::CvoltonManager(){}

void CvoltonManager::doUpdateCheck(){
    if(hasDoneUpdateCheck) return;

    processHttpRequest();
    hasDoneUpdateCheck = true;
}
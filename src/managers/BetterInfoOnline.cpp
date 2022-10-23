#include "BetterInfoOnline.h"
#include <gd.h>
#include "../utils.hpp"
#include "CvoltonManager.h"

#include <fstream>

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;

bool BetterInfoOnline::init(){
    return true;
}

BetterInfoOnline::BetterInfoOnline(){}


void BetterInfoOnline::loadScores(int accountID, BILeaderboardDelegate* delegate) {
    m_scoreDelegate = delegate;
    loadScores(accountID);
}

void BetterInfoOnline::loadScores(int accountID, ProfilePage* profilePage) {
    m_scoreProfilePage = profilePage;
    loadScores(accountID);
}

void BetterInfoOnline::loadScores(int accountID){
    CCHttpRequest* request = new CCHttpRequest;
    request->setUrl("http://www.boomlings.com/database/getGJScores20.php");
    request->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
    request->setResponseCallback(this, httpresponse_selector(BetterInfoOnline::onScoresFinished));
    //TODO: make this slightly more dynamic
    auto postData = CCString::createWithFormat("gameVersion=21&binaryVersion=35&gdw=0&accountID=%i&type=relative&secret=Wmfd2893gb7", accountID);
    request->setRequestData(
        postData->getCString(), strlen(postData->getCString())
    );
    request->setUserData((void*) accountID);
    CCHttpClient::getInstance()->send(request);
    //request->release();
}

void BetterInfoOnline::onScoresFinished(CCHttpClient* client, CCHttpResponse* response){
    if(!(response->isSucceed())) return;

    std::vector<char>* responseData = response->getResponseData();
    std::string responseString(responseData->begin(), responseData->end());

    int accountID = (int) (response->getHttpRequest()->getUserData());
    generateScores(responseString, accountID);

    if(m_scoreDelegate) {
        m_scoreDelegate->onLeaderboardFinished(m_scoreDict[accountID]);
        m_scoreDelegate = nullptr;
    }
}


void BetterInfoOnline::generateScores(const std::string& response, int accountID){

    if(m_scoreDict.contains(accountID)) { 
        m_scoreDict[accountID]->release();
        m_scoreDict[accountID] = nullptr;
    }

    CCArray* scores = CCArray::create();
    scores->retain();
    m_scoreDict[accountID] = scores;

    if(response == "-1") return;

    std::stringstream responseStream(response);
    std::string current;

    while(getline(responseStream, current, '|')){
        
        auto CM = CvoltonManager::sharedState();

        auto score = GJUserScore::create(
            CM->responseToDict(current)
        );

        scores->addObject(score);
    }
}
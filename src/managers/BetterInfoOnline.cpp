#include "BetterInfoOnline.h"
#include <gd.h>
#include "../utils.hpp"
#include "CvoltonManager.h"
#include "support/base64.h"

#include <fstream>

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;

bool BetterInfoOnline::init(){
    return true;
}

BetterInfoOnline::BetterInfoOnline(){}


void BetterInfoOnline::loadScores(int accountID, bool force, BILeaderboardDelegate* delegate) {
    m_scoreDelegate = delegate;
    loadScores(accountID, force);
}

void BetterInfoOnline::loadScores(int accountID, bool force, ProfilePage* profilePage) {
    m_scoreProfilePage = profilePage;
    loadScores(accountID, force);
}

void BetterInfoOnline::loadScores(int accountID, bool force){
    //cache optimization
    if(!force && m_scoreDict.contains(accountID)) {
        sendScores(m_scoreDict[accountID], accountID);
        return;
    }

    //only on forced reload or if we dont have cached
    CCHttpRequest* request = new CCHttpRequest;

    unsigned char* originalUrl = (unsigned char*)(base + 0x29BF30);
    unsigned char* getGJScores;
    int getGJScoresSize = base64Decode(originalUrl, strlen((const char*) originalUrl), &getGJScores);

    request->setUrl((const char*) getGJScores);
    request->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
    request->setResponseCallback(this, httpresponse_selector(BetterInfoOnline::onScoresFinished));
    //TODO: make this slightly more dynamic
    auto postData = CCString::createWithFormat("gameVersion=21&binaryVersion=35&gdw=0&accountID=%i&udid=asdadsas&type=relative&secret=Wmfd2893gb7", accountID);
    request->setRequestData(
        postData->getCString(), strlen(postData->getCString())
    );
    request->setUserData((void*) accountID);
    CCHttpClient::getInstance()->send(request);

    delete getGJScores;
    //request->release();
}

void BetterInfoOnline::onScoresFinished(CCHttpClient* client, CCHttpResponse* response){
    if(!(response->isSucceed())) return;

    std::vector<char>* responseData = response->getResponseData();
    std::string responseString(responseData->begin(), responseData->end());

    int accountID = (int) (response->getHttpRequest()->getUserData());
    generateScores(responseString, accountID);

    sendScores(m_scoreDict[accountID], accountID);
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
        if(score->userUDID_ != "") score->userUDID_ = GameManager::sharedState()->m_sPlayerUDID;

        scores->addObject(score);
    }
}

void BetterInfoOnline::sendScores(cocos2d::CCArray* scores, int accountID){
    if(m_scoreDelegate) {
        m_scoreDelegate->onLeaderboardFinished(scores);
        m_scoreDelegate = nullptr;
    }else if(m_scoreProfilePage) {
        sendScoreToProfilePage(scores, accountID);
    }
}

void BetterInfoOnline::sendScoreToProfilePage(cocos2d::CCArray* scores, int accountID){
    if(!m_scoreProfilePage) return;

    CCObject* obj;
    CCARRAY_FOREACH(scores, obj){
        auto score = static_cast<GJUserScore*>(obj);
        score->messageState_ = 2;
        score->friendStatus_ = 1;
        if(score->accountID_ == accountID) {
            score->globalRank_ = score->playerRank_;
            switch(score->iconType_) {
                case kIconTypeCube:
                    score->playerCube_ = score->iconID_;
                    break;
                case kIconTypeShip:
                    score->playerShip_ = score->iconID_;
                    break;
                case kIconTypeBall:
                    score->playerBall_ = score->iconID_;
                    break;
                case kIconTypeUfo:
                    score->playerUfo_ = score->iconID_;
                    break;
                case kIconTypeWave:
                    score->playerWave_ = score->iconID_;
                    break;
                case kIconTypeRobot:
                    score->playerRobot_ = score->iconID_;
                    break;
                case kIconTypeSpider:
                    score->playerSpider_ = score->iconID_;
                    break;
            }

            m_scoreProfilePage->getUserInfoChanged(score);
            m_scoreProfilePage = nullptr;
            break;
        }
    }
}
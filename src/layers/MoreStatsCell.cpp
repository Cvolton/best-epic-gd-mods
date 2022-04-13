#include "MoreStatsCell.h"
#include "ExtendedLevelInfo.h"
#include "../managers/CvoltonManager.h"

using namespace gd;
using namespace cocos2d;

void MoreStatsCell::loadFromKey(CCString* key) {
    auto CM = CvoltonManager::sharedState();
    auto GM = gd::GameManager::sharedState();
    auto AM = gd::GJAccountManager::sharedState();
    auto GLM = gd::GameLevelManager::sharedState();
    auto GSM = gd::GameStatsManager::sharedState();
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    std::string title, value;
    switch(key->intValue()){
        default:
            title = "Undefined";
            value = "-1";
            break;
        case 0:
            title = "Bootups";
            value = std::to_string(GM->m_nBootups);
            break;
        case 1:
            title = "Cod3breaker";
            value = std::to_string(GM->m_nSecretNumberRand1 - GM->m_nSecretNumberRand2);
            break;
        case 2:
            title = "Moderator Level";
            value = std::to_string(GM->m_nRatePower);
            break;
        case 3:
            title = "User ID";
            value = std::to_string(GM->m_nPlayerUserID);
            break;
        case 4:
            title = "Account ID";
            value = std::to_string(AM->accountID);
            break;
        case 5:
            title = "UDID";
            value = GM->m_sPlayerUDID;
            break;
        case 6:
            title = "Completed Unrated Levels";
            value = std::to_string(CM->completedLevelsForStars(0));
            break;
        case 7:
            title = "Completed Auto (1*) Levels";
            value = std::to_string(CM->completedLevelsForStars(1));
            break;
        case 8:
            title = "Completed Easy (2*) Levels";
            value = std::to_string(CM->completedLevelsForStars(2));
            break;
        case 9:
            title = "Completed Normal (3*) Levels";
            value = std::to_string(CM->completedLevelsForStars(3));
            break;
        case 10:
            title = "Completed Hard (4*) Levels";
            value = std::to_string(CM->completedLevelsForStars(4));
            break;
        case 11:
            title = "Completed Hard (5*) Levels";
            value = std::to_string(CM->completedLevelsForStars(5));
            break;
        case 12:
            title = "Completed Harder (6*) Levels";
            value = std::to_string(CM->completedLevelsForStars(6));
            break;
        case 13:
            title = "Completed Harder (7*) Levels";
            value = std::to_string(CM->completedLevelsForStars(7));
            break;
        case 14:
            title = "Completed Insane (8*) Levels";
            value = std::to_string(CM->completedLevelsForStars(8));
            break;
        case 15:
            title = "Completed Insane (9*) Levels";
            value = std::to_string(CM->completedLevelsForStars(9));
            break;
        case 16:
            title = "Completed Demon (10*) Levels";
            value = std::to_string(CM->completedLevelsForStars(10));
            break;
        case 17:
            title = "Played Online Levels";
            value = std::to_string(GLM->m_onlineLevels->count());
            break;
        case 18:
            title = "Reported Levels";
            value = std::to_string(GLM->m_reportedLevels->count());
            break;
    }

    auto titleTxt = CCLabelBMFont::create(title.c_str(), "goldFont.fnt");
    titleTxt->setAnchorPoint({ 0.0f, .5f });
    titleTxt->setPosition(20.f, 19.f);
    titleTxt->limitLabelWidth(200, .7f, .4f);
    this->m_pLayer->addChild(titleTxt);

    auto content = CCLabelBMFont::create(value.c_str(), "goldFont.fnt");
    content->setAnchorPoint({ 1.f, .5f });
    content->setPosition(356 - 25, 20);
    content->limitLabelWidth(200, .7f, .4f);
    this->m_pLayer->addChild(content);

    this->m_pLayer->setVisible(true);
    this->m_pBGLayer->setOpacity(255);
}

void MoreStatsCell::draw() {
    // just call StatsCell::draw, no one will notice
    // this is stolen from betteredit blame fod
    reinterpret_cast<void(__thiscall*)(MoreStatsCell*)>(
        base + 0x59d40
    )(this);
}

MoreStatsCell::MoreStatsCell(const char* name, CCSize size) :
    TableViewCell(name, 356.f, size.height) {} //356 should be size.width but its getting 220 for some reason and i cant be bothered to troubleshoot this rn //TODO: fix this properly

MoreStatsCell* MoreStatsCell::create(const char* key, CCSize size) {
    auto ret = new MoreStatsCell(key, size);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}
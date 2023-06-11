#include "FLAlertResultHandler.h"
#include "../managers/CvoltonManager.h"

#include <cocos2d.h>
#include <gd.h>

using namespace cocos2d;
using namespace gd;

FLAlertResultHandler* FLAlertResultHandler::create(){
    auto ret = new FLAlertResultHandler();
    if (ret && ret->init()) {
        //robert 1 :D
        ret->autorelease();
        ret->retain();
    } else {
        //robert -1
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void FLAlertResultHandler::FLAlert_Clicked(gd::FLAlertLayer*, bool success) {
    if(success) {
        auto CM = CvoltonManager::sharedState();
        CM->setOption("show_comment_ids", true);
        CM->save();
        FLAlertLayer::create(nullptr, CvoltonManager::modName, "OK", nullptr, "Refresh for the change to take effect.")->show();
    }
}

bool FLAlertResultHandler::init(){
    CvoltonManager::sharedState()->setOption("comment_id_alert_shown", true);
    CvoltonManager::sharedState()->save();

    return CCNode::init();
}
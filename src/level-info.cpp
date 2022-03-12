#define WIN32_LEAN_AND_MEAN
#include "mapped-hooks.hpp"
#include "Windows.h"
#include <fstream>
#include <iostream>
#include <cocos2d.h>
#include <gd.h>
#include <typeinfo>
#include <cstring>
#include "utils.hpp"

using namespace cocos2d;

uintptr_t base;

class InfoLayer : CCLayer {
public:
    PAD(472 - sizeof(CCLayer));
    gd::GJGameLevel* level;
};

std::string getGameVersionName(int version){
    if(version > 99) return std::string("Hacked");

    switch(version){
        case 6:
            return std::string("1.6 or earlier");
        case 10:
            return std::string("1.7");
        case 11:
            return std::string("1.7 or 1.8");
    }

    std::stringstream contentStream;

    if(version > 17){
        double newVersion = (double)version / 10.0;
        contentStream << std::fixed << std::setprecision(1) << newVersion;
    }else{
        contentStream << "1." << (version-1);
    }
    return contentStream.str();
}

void __fastcall InfoLayer_onLevelInfo(InfoLayer* self, void* a, CCObject* sender) {
    //MHook::getOriginal(CommentCell_loadFromComment)(self, a, b);

    auto level = self->level;

    int levelPassword = level->password_rand - level->password_seed;

    std::stringstream contentStream;
    contentStream << "Description: " << level->getUnpackedLevelDescription() << "\n";

    contentStream << "\nUploaded: " << level->uploadDate 
        << " ago\nUpdated: " << level->updateDate << " ago";
    if(level->starsRequested != 0) contentStream << "\nStars Requested: " << level->starsRequested;
    if(level->originalLevel != 0) contentStream << "\nOriginal: " << level->originalLevel;
    contentStream << "\nGame Version: " << getGameVersionName(level->gameVersion);
    if(level->originalLevel != 0) contentStream << "\nFeature Score: " << level->featured;
    if(levelPassword > 1) contentStream << "\nPassword: " << (levelPassword - 1000000);
    if(level->workingTime > 0) contentStream << "\nWorking time: " << level->workingTime;
    if(level->workingTime2 > 0) contentStream << "\nWorking time 2: " << level->workingTime2;
    if (level->objectCount > 0) contentStream << "\nObjects: " << (level->objectCount == 65535 ? "65535+" : std::to_string(level->objectCount));
    contentStream << "\nUser ID: " << level->userID;
        //<< "\nYour personal bests: " << level->personalBests;
        //<< "\n isCompletionLegitimate: " << level->isCompletionLegitimate;

    gd::FLAlertLayer::create(nullptr, "Level Info", "OK", nullptr, 500, contentStream.str())->show();
}

DWORD WINAPI my_thread(void* hModule) {
    MH_Initialize();

    base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    
    MHook::registerHook(base + 0x151850, InfoLayer_onLevelInfo);

    MH_EnableHook(MH_ALL_HOOKS);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, my_thread, hModule, 0, 0);
    return TRUE;
}
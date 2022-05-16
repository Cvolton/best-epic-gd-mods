#include "ExtendedLevelInfo.h"
#include "UnregisteredProfileLayer.h"

#include <cocos2d.h>
#include <gd.h>
#include <deque>

using namespace cocos2d;
using namespace gd;

ExtendedLevelInfo* ExtendedLevelInfo::create(gd::GJGameLevel* level){
    auto ret = new ExtendedLevelInfo();
    level->retain();
    ret->level = level;
    if (ret && ret->init()) {
        //robert 1 :D
        ret->autorelease();
    } else {
        //robert -1
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void ExtendedLevelInfo::onClose(cocos2d::CCObject* sender)
{
    level->release();
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

std::string ExtendedLevelInfo::getGameVersionName(int version){
    if(version < 1 || version > 99) return std::string("NA");

    switch(version){
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

std::string ExtendedLevelInfo::stringDate(std::string date){
    if(date == "") return "NA";
    std::ostringstream stream;
    stream << date << " ago";
    return stream.str();
}

const char* ExtendedLevelInfo::getDifficultyIcon(int stars){
    switch(stars){
        case 1: 
            return "difficulty_auto_btn_001.png";
        case 2:
            return "difficulty_01_btn_001.png";
        case 3:
            return "difficulty_02_btn_001.png";
        case 4:
        case 5:
            return "difficulty_03_btn_001.png";
        case 6:
        case 7:
            return "difficulty_04_btn_001.png";
        case 8:
        case 9:
            return "difficulty_05_btn_001.png";
        case 10:
            return "difficulty_06_btn_001.png";
        default:
            return "difficulty_00_btn_001.png";
    }
}

std::string ExtendedLevelInfo::passwordString(int password){
    if(password == 0) return "NA";
    if(password == 1) return "Free Copy";
    return std::to_string(password - 1000000);
}

std::string ExtendedLevelInfo::zeroIfNA(int value){
    if(value == 0) return "NA";
    return std::to_string(value);
}

std::string ExtendedLevelInfo::workingTime(int value){
    if(value <= 0) return "NA";

    int hours = value / 3600;
    int minutes = (value % 3600) / 60;
    int seconds = value % 60;

    std::ostringstream stream;
    if(hours > 0) stream << hours << "h ";
    if(minutes > 0) stream << minutes << "m ";
    stream << seconds << "s";

    return stream.str();
}

std::string ExtendedLevelInfo::printableProgress(std::string personalBests, int percentage){

    std::stringstream bestsStream(personalBests);
    std::string currentBest;
    std::deque<int> progresses;
    while(getline(bestsStream, currentBest, ',')){
        try {
            progresses.push_front(std::stoi(currentBest));
            //contentStream << percentage << "% ";
        }catch(...){}
    }
    std::string printable;
    //std::reverse(std::begin(progresses), std::end(progresses));
    for(auto i : progresses){
        //contentStream << percentage << "% ";
        //printable.insert(0, std::format("{}% ", percentage));
        printable = std::to_string(percentage) + "% " + printable;
        percentage -= i;
    }

    return printable;
}

bool ExtendedLevelInfo::init(){
    bool init = createBasics({440.0f, 290.0f}, menu_selector(ExtendedLevelInfo::onClose));
    if(!init) return false;

    auto levelName = CCLabelBMFont::create(level->levelName.c_str(), "bigFont.fnt");
    levelName->setPosition({0,125});
    m_pButtonMenu->addChild(levelName);

    std::ostringstream userNameText;
    userNameText << "By " << level->userName;
    auto userName = CCLabelBMFont::create(userNameText.str().c_str(), "goldFont.fnt");
    userName->setPosition({0,99});
    userName->setScale(0.8f);
    m_pButtonMenu->addChild(userName);

    cocos2d::extension::CCScale9Sprite* descBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    descBg->setContentSize({340,55});
    descBg->setColor({130,64,33});
    m_pButtonMenu->addChild(descBg, -1);
    descBg->setPosition({0,52});

    auto descText = level->getUnpackedLevelDescription();
    size_t descLength = descText.length();
    float descDelimiter = 1;
    if(descLength > 140) descLength = 140;
    if(descLength > 70) descDelimiter = ((((140 - descLength) / 140) * 0.3f) + 0.7f);
    auto description = gd::TextArea::create("chatFont.fnt", false, descText, 1, 295 / descDelimiter, 20, {0.5f,0.5f});
    description->setScale(descDelimiter);
    description->setPosition({0,52});
    m_pButtonMenu->addChild(description);

    cocos2d::extension::CCScale9Sprite* infoBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    infoBg->setContentSize({340,148});
    //infoBg->setColor({130,64,33});
    //infoBg->setColor({191,114,62});
    infoBg->setColor({123,60,31});
    m_pButtonMenu->addChild(infoBg, -1);
    infoBg->setPosition({0,-57});

    int levelPassword = level->password_rand - level->password_seed;
    std::ostringstream infoText;
    infoText << "\n<cj>Uploaded</c>: " << stringDate(level->uploadDate)
        << "\n<cj>Updated</c>: " << stringDate(level->updateDate)
        //<< "\n<cy>Stars Requested</c>: " << level->starsRequested
        << "\n<cg>Original</c>: " << zeroIfNA(level->originalLevel)
        //<< "\n<cg>Feature score</c>: " << zeroIfNA(level->featured)
        << "\n<cy>Game Version</c>: " << getGameVersionName(level->gameVersion)
        //<< "\nFeature Score</c>: " << level->featured
        << "\n<co>Password</c>: " << passwordString(levelPassword)
        << "\n<cr>In Editor</c>: " << workingTime(level->workingTime)
        << "\n<cr>Editor (C)</c>: " << workingTime(level->workingTime2);

    /*infoText << "<cg>Total Attempts</c>: " << level->attempts
        << "\n<cl>Total Jumps</c>: " << level->jumps
        << "\n<co>Clicks (best att.)</c>: " << level->clicks // the contents of this variable make no sense to me
        << "\n<co>Time (best att.)</c>: " << ExtendedLevelInfo::workingTime(level->attemptTime) // the contents of this variable make no sense to me
        //<< "\n<co>Is legit</c>: " << level->isCompletionLegitimate // the contents of this variable make no sense to me
        << "\n<cp>Normal</c>: " << level->normalPercent
        << "%\n<co>Practice</c>: " << level->practicePercent << "%";

    if(level->orbCompletion != level->newNormalPercent2) infoText << "\n<cj>2.1 Normal</c>: " << level->orbCompletion << "%";
    if(level->newNormalPercent2 != level->normalPercent) infoText << "\n<cr>2.11 Normal</c>: " << level->newNormalPercent2 << "%";*/

    auto info = gd::TextArea::create("chatFont.fnt", false, infoText.str(), 1, 170, 20, {0,1});
    info->setPosition({-160.5,26});
    //info->setPosition({-160.5,10});
    info->setAnchorPoint({0,1});
    info->setScale(0.925f);
    m_pButtonMenu->addChild(info);

    /*std::ostringstream progressText;
    if(level->personalBests != "") progressText << "\n\n<cy>Progresses</c>:\n" << printableProgress(level->personalBests, level->newNormalPercent2);
    auto progress = gd::TextArea::create("chatFont.fnt", false, progressText.str(), 0.8f, 130, 20, {0,1});
    progress->setPosition({12,50});
    progress->setAnchorPoint({0,1});
    m_pButtonMenu->addChild(progress);*/

    /*std::ostringstream uploadedText;
    uploadedText << "Uploaded: " << level->uploadDate << " ago";
    createTextLabel(uploadedText.str(), {0,0}, 0.5f, m_pButtonMenu);*/

    createTextLabel("Requested Rate:", {88,-1}, 0.5f, m_pButtonMenu);

    auto diffSprite = CCSprite::createWithSpriteFrameName(getDifficultyIcon(level->starsRequested));
    diffSprite->setPosition({88,-57});
    m_pButtonMenu->addChild(diffSprite, 1);

    if(level->starsRequested > 0){
        auto featureSprite = CCSprite::createWithSpriteFrameName("GJ_featuredCoin_001.png");
        featureSprite->setPosition({88,-57});
        m_pButtonMenu->addChild(featureSprite);
        //infoSprite->setScale(0.7f);

        auto starsLabel = createTextLabel(std::to_string(level->starsRequested), {88, -87}, 0.4f, m_pButtonMenu);
        starsLabel->setAnchorPoint({1,0.5});

        auto diffSprite = CCSprite::createWithSpriteFrameName("star_small01_001.png");
        diffSprite->setPosition({95,-87});
        m_pButtonMenu->addChild(diffSprite);
    }

    /*
        thanks to Alphalaneous for quick UI improvement concept lol
    */

    auto separator = CCSprite::createWithSpriteFrameName("floorLine_001.png");
    separator->setPosition({6,-57});
    separator->setScaleX(0.3f);
    separator->setScaleY(1);
    separator->setOpacity(100);
    separator->setRotation(90);
    m_pButtonMenu->addChild(separator);

    return true;
}

CCLabelBMFont* ExtendedLevelInfo::createTextLabel(const std::string text, const CCPoint& position, const float scale, CCNode* menu, const char* font){
    CCLabelBMFont* bmFont = CCLabelBMFont::create(text.c_str(), font);
    bmFont->setPosition(position);
    bmFont->setScale(scale);
    menu->addChild(bmFont);
    return bmFont;
}

void ExtendedLevelInfo::showProgressDialog(GJGameLevel* level){

    if(level == nullptr) return;

    if(level->levelID == -1){
        gd::FLAlertLayer::create(nullptr, "It's a secret...", "OK", nullptr, 360, "The <cr>darkness</c> lingers. Be careful who you trust...\nThere might be an <c_>impostor</c> among us.")->show();
        return;
    }

    std::ostringstream contentStream;
    contentStream << "<cg>Total Attempts</c>: " << level->attempts
        << "\n<cl>Total Jumps</c>: " << level->jumps
        << "\n<co>Clicks (best att.)</c>: " << level->clicks
        << "\n<co>Time (best att.)</c>: " << ExtendedLevelInfo::workingTime(level->attemptTime);

    if(level->levelType != GJLevelType::kGJLevelTypeEditor){ contentStream << "\n<cp>Normal</c>: " << level->normalPercent
        << "%\n<co>Practice</c>: " << level->practicePercent << "%";

        if(level->orbCompletion != level->normalPercent) contentStream << "\n<cj>Orbs</c>: " << level->orbCompletion << "%";
        if(level->newNormalPercent2 != level->orbCompletion) contentStream << "\n<cr>Leaderboard</c>: " << level->newNormalPercent2 << "%";
    }else{
        contentStream << "\n<cp>Objects</c>: " << level->objectCount
            << "\n<cr>In Editor</c>: " << ExtendedLevelInfo::workingTime(level->workingTime);
        if(level->workingTime2) contentStream << "\n<cr>Editor (C)</c>: " << ExtendedLevelInfo::workingTime(level->workingTime2);
        ;
    }

    std::string progresses;
    if(level->personalBests != ""){
        progresses = ExtendedLevelInfo::printableProgress(level->personalBests, level->newNormalPercent2);
        contentStream << "\n\n<cy>Progresses</c>: " << progresses;
    }

    float dialogWidth = 300;
    if(progresses.length() > 48) dialogWidth = 350;
    if(progresses.length() > 72) dialogWidth = 400;
    gd::FLAlertLayer::create(nullptr, "Level Stats", "OK", nullptr, dialogWidth, contentStream.str().length() > 300, 300, contentStream.str())->show();
}
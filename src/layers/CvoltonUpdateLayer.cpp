#include "CvoltonUpdateLayer.h"
#include "UnregisteredProfileLayer.h"
#include "../managers/CvoltonManager.h"
#include "../utils.hpp"

#include <cocos2d.h>
#include <gd.h>

using namespace cocos2d;
using namespace gd;

CvoltonUpdateLayer* CvoltonUpdateLayer::create(){
    auto ret = new CvoltonUpdateLayer();
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

void CvoltonUpdateLayer::onClose(cocos2d::CCObject* sender)
{
    setKeypadEnabled(false);
    removeFromParentAndCleanup(true);
}

bool CvoltonUpdateLayer::init(){
    bool init = createBasics({440.0f, 290.0f}, menu_selector(CvoltonUpdateLayer::onClose));
    if(!init) return false;

    title = CCLabelBMFont::create("Checking for updates!", "bigFont.fnt");
    title->setPosition({0,120});
    title->limitLabelWidth(340, 1, 0);
    m_pButtonMenu->addChild(title);

    /*auto userName = CCLabelBMFont::create("Epic GMD Mod", "goldFont.fnt");
    userName->setPosition({0,99});
    userName->setScale(0.8f);
    m_pButtonMenu->addChild(userName);*/

    cocos2d::extension::CCScale9Sprite* descBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    descBg->setContentSize({340,55});
    descBg->setColor({130,64,33});
    m_pButtonMenu->addChild(descBg, -1);
    descBg->setPosition({0,68});

    /*auto descText = level->getUnpackedLevelDescription();
    size_t descLength = descText.length();
    float descDelimiter = 1;
    if(descLength > 140) descLength = 140;
    if(descLength > 70) descDelimiter = ((((140 - descLength) / 140) * 0.3f) + 0.7f);*/

    description = TextArea::create("chatFont.fnt", false, "<cy>Installed: </c>Placeholder\n<cg>Latest: </c> Placeholder\n", 1, 295, 20, {0,0});
    //description->setScale(descDelimiter);
    description->setAnchorPoint({0,0});
    description->setPosition({-161, 50});
    m_pButtonMenu->addChild(description);

    cocos2d::extension::CCScale9Sprite* infoBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    infoBg->setContentSize({340,125});
    infoBg->setAnchorPoint({0.5f,1});
    //infoBg->setColor({130,64,33});
    //infoBg->setColor({191,114,62});
    infoBg->setColor({123,60,31});
    m_pButtonMenu->addChild(infoBg, -1);
    infoBg->setPosition({0,4});

    changelog = TextArea::create("chatFont.fnt", false, "Changelog Placeholder Text", 1, 340, 20, {0,1});
    changelog->setPosition({-162,0.f});
    changelog->setScale(0.8f);
    changelog->setAnchorPoint({0,1});
    m_pButtonMenu->addChild(changelog);
    changelog->setVisible(false);

    changelogFailText = CCLabelBMFont::create("Load failed", "goldFont.fnt");
    changelogFailText->setPosition({0,-50});
    m_pButtonMenu->addChild(changelogFailText);
    changelogFailText->setVisible(false);

    circle = LoadingCircle::create();
    m_pButtonMenu->addChild(circle);
    circle->setPosition({-286,-217});
    circle->show();

    auto websiteSprite = ButtonSprite::create("Visit Website", 150, true, "bigFont.fnt", "GJ_button_01.png", 25, 0.5f);
    auto webBtn = CCMenuItemSpriteExtra::create(
        websiteSprite,
        this,
        menu_selector(CvoltonUpdateLayer::onVisit)
    );
    webBtn->setSizeMult(1.2f);
    webBtn->setPosition({-88,22});
    m_pButtonMenu->addChild(webBtn);

    auto updateBtnSprite = ButtonSprite::create("Download Update", 150, true, "bigFont.fnt", "GJ_button_01.png", 25, 0.5f);
    auto updateBtn = CCMenuItemSpriteExtra::create(
        updateBtnSprite,
        this,
        menu_selector(CvoltonUpdateLayer::onUpdate)
    );
    updateBtn->setSizeMult(1.2f);
    updateBtn->setPosition({88,22});
    m_pButtonMenu->addChild(updateBtn);

    auto historySprite = BetterInfo::createWithBISpriteFrameName("BI_historyBtn_001.png");
    auto historyBtn = CCMenuItemSpriteExtra::create(
        historySprite,
        this,
        menu_selector(CvoltonUpdateLayer::onChangelog)
    );
    historyBtn->setPosition({-210,-135});
    m_pButtonMenu->addChild(historyBtn);

    downloadChangelog();
    showVersion();

    return true;
}

void CvoltonUpdateLayer::downloadChangelog(){
    auto CM = CvoltonManager::sharedState();

    if(CM->changelog == "" || CM->latestVer == "") CM->downloadChangelog(this);
    else onLoadFinished();
}

void CvoltonUpdateLayer::showVersion(){
    auto CM = CvoltonManager::sharedState();

    std::ostringstream verStream;
    verStream << "<cy>Installed: </c>" << CM->version << "<cg>Latest: </c>" << CM->latestVer;

    if(CM->isUpToDate()) title->setString("Up to date!");
    else title->setString("Update available!");

    title->limitLabelWidth(340, 1, 0);

    description->setString(verStream.str());
}

void CvoltonUpdateLayer::onUpdate(cocos2d::CCObject* sender)
{
    CCApplication::sharedApplication()->openURL("https://cvolton.eu/gdmods/betterinfo/download/");
}

void CvoltonUpdateLayer::onVisit(cocos2d::CCObject* sender)
{
    CCApplication::sharedApplication()->openURL("https://geometrydash.eu/mods");
}

void CvoltonUpdateLayer::onChangelog(cocos2d::CCObject* sender)
{
    auto layer = CvoltonManager::sharedState()->updateCompleteDialog(true);
    layer->m_pParent = this;
    if(layer) layer->show();
}

void CvoltonUpdateLayer::onLoadFailed()
{
    changelogFailText->setVisible(true);
    circle->fadeAndRemove();
}

void CvoltonUpdateLayer::onLoadFinished()
{
    auto CM = CvoltonManager::sharedState();

    if(CM->changelog == "" || CM->changelog.length() > 500){
        onLoadFailed();
        return;
    }

    changelog->setString(CM->changelog);
    changelog->setVisible(true);

    showVersion();

    circle->fadeAndRemove();
}

CCLabelBMFont* CvoltonUpdateLayer::createTextLabel(const std::string text, const CCPoint& position, const float scale, CCNode* menu, const char* font){
    CCLabelBMFont* bmFont = CCLabelBMFont::create(text.c_str(), font);
    bmFont->setPosition(position);
    bmFont->setScale(scale);
    menu->addChild(bmFont);
    return bmFont;
}
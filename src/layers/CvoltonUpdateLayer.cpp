#include "CvoltonUpdateLayer.h"
#include "UnregisteredProfileLayer.h"
#include "../managers/CvoltonManager.h"

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

    auto title = CCLabelBMFont::create("Update available!", "bigFont.fnt");
    title->setPosition({0,120});
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
    auto CM = CvoltonManager::sharedState();

    std::ostringstream verStream;
    verStream << "<cy>Installed: </c>" << CM->version << "<cg>Latest: </c>" << CM->latestVer;

    auto description = gd::TextArea::create("chatFont.fnt", false, verStream.str().c_str(), 1, 295, 20, {0,0});
    //description->setScale(descDelimiter);
    description->setPosition({429, 61});
    m_pButtonMenu->addChild(description);

    cocos2d::extension::CCScale9Sprite* infoBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    infoBg->setContentSize({340,125});
    infoBg->setAnchorPoint({0.5f,1});
    //infoBg->setColor({130,64,33});
    //infoBg->setColor({191,114,62});
    infoBg->setColor({123,60,31});
    m_pButtonMenu->addChild(infoBg, -1);
    infoBg->setPosition({0,4});

    std::ostringstream infoText;
    infoText << "\n<cj>Changelog</c>: \n" << "- Added amongus\n- Added sus\n- Added susamogus\n- Removed vents\n- Removed vents\n- Removed Herobrine";

    auto info = gd::TextArea::create("chatFont.fnt", false, infoText.str(), 1, 170, 20, {0,1});
    info->setPosition({-162,13.5f});
    info->setScale(0.8f);
    info->setAnchorPoint({0,1});
    m_pButtonMenu->addChild(info);

    /*std::ostringstream uploadedText;
    uploadedText << "Uploaded: " << level->uploadDate << " ago";
    createTextLabel(uploadedText.str(), {0,0}, 0.5f, m_pButtonMenu);*/

    /*createTextLabel("Requested Rate:", {88,-1}, 0.5f, m_pButtonMenu);

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

    /*auto separator = CCSprite::createWithSpriteFrameName("floorLine_001.png");
    separator->setPosition({6,-57});
    separator->setScaleX(0.3f);
    separator->setScaleY(1);
    separator->setOpacity(100);
    separator->setRotation(90);
    m_pButtonMenu->addChild(separator);*/

    auto websiteSprite = gd::ButtonSprite::create("Visit Website", 150, true, "bigFont.fnt", "GJ_button_01.png", 25, 0.5f);
    auto webBtn = gd::CCMenuItemSpriteExtra::create(
        websiteSprite,
        this,
        menu_selector(CvoltonUpdateLayer::onVisit)
    );
    webBtn->setSizeMult(1.2f);
    webBtn->setPosition({-88,22});
    m_pButtonMenu->addChild(webBtn);

    auto updateBtnSprite = gd::ButtonSprite::create("Download Update", 150, true, "bigFont.fnt", "GJ_button_01.png", 25, 0.5f);
    auto updateBtn = gd::CCMenuItemSpriteExtra::create(
        updateBtnSprite,
        this,
        menu_selector(CvoltonUpdateLayer::onUpdate)
    );
    updateBtn->setSizeMult(1.2f);
    updateBtn->setPosition({88,22});
    m_pButtonMenu->addChild(updateBtn);

    return true;
}

void CvoltonUpdateLayer::onUpdate(cocos2d::CCObject* sender)
{
    CCApplication::sharedApplication()->openURL("https://www.innersloth.com/games/among-us/");
}

void CvoltonUpdateLayer::onVisit(cocos2d::CCObject* sender)
{
    CCApplication::sharedApplication()->openURL("https://geometrydash.eu/mods");
}

CCLabelBMFont* CvoltonUpdateLayer::createTextLabel(const std::string text, const CCPoint& position, const float scale, CCNode* menu, const char* font){
    CCLabelBMFont* bmFont = CCLabelBMFont::create(text.c_str(), font);
    bmFont->setPosition(position);
    bmFont->setScale(scale);
    menu->addChild(bmFont);
    return bmFont;
}
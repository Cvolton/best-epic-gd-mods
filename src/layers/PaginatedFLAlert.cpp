#include "PaginatedFLAlert.h"
#include "CvoltonUpdateLayer.h"
#include "CvoltonOptionsLayer.h"
#include "LevelIDLayer.h"
#include "RewardViewLayer.h"
#include "MoreStatsListView.h"
#include "../managers/CvoltonManager.h"
#include "../utils.hpp"

using namespace gd;
using namespace cocos2d;

PaginatedFLAlert* PaginatedFLAlert::create(const std::string& title, const std::vector<std::string>& content, size_t page) {
    auto ret = new PaginatedFLAlert();
    if (ret && ret->init(title, content, page)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool PaginatedFLAlert::init(const std::string& title, const std::vector<std::string>& content, size_t page) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_title = title;
    m_content = content;
    m_page = page;
    /*
        flalert
    */
    if(m_content.size() == 0) m_content.push_back("PaginatedFLAlert content is empty, this is very likely a coding mistake and should be reported as a bug to the mod author(s).");

    FLAlertLayer::init(nullptr, title.c_str(), "OK", nullptr, 400, false, 300, content[page % m_content.size()]);

    /*
        next/prev page btn
    */
    auto y = -m_pButtonMenu->getPosition().y + (winSize.height / 2);

    auto prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    prevSprite->setScale(.8f);
    auto prevBtn = gd::CCMenuItemSpriteExtra::create(
        prevSprite,
        this,
        menu_selector(PaginatedFLAlert::onPrev)
    );
    prevBtn->setPosition({-220, y});
    m_pButtonMenu->addChild(prevBtn);
    prevBtn->setVisible(m_page > 0);

    auto nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextSprite->setFlipX(true);
    nextSprite->setScale(.8f);
    auto nextBtn = gd::CCMenuItemSpriteExtra::create(
        nextSprite,
        this,
        menu_selector(PaginatedFLAlert::onNext)
    );
    nextBtn->setPosition({220, y});
    nextBtn->setVisible(m_page < (m_content.size() - 1));
    m_pButtonMenu->addChild(nextBtn);

    return true;
}

void PaginatedFLAlert::keyBackClicked() {
    onClose(this);
}

void PaginatedFLAlert::onNext(cocos2d::CCObject* sender) {
    loadPage(m_page+1);
}

void PaginatedFLAlert::onPrev(cocos2d::CCObject* sender) {
    loadPage(m_page-1);
}

void PaginatedFLAlert::loadPage(size_t page) {
    //this needs adjustments to support the parent member in flalert shall it ever be needed
    auto newAlert = PaginatedFLAlert::create(m_title, m_content, page);
    CCDirector::sharedDirector()->getRunningScene()->addChild(newAlert);
    onClose(this);
}
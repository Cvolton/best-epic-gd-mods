#include "DailyListView.h"
#include "RewardListView.h"
#include "RewardCell.h"
#include "../managers/CvoltonManager.h"

using namespace gd;
using namespace cocos2d;

RewardListView* RewardListView::create(cocos2d::CCArray* rewards, float width, float height) {
    auto ret = new RewardListView();
    if (ret && ret->init(rewards, 0x420, width, height)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void RewardListView::setupList() {
    this->m_fItemSeparation = 45.0f;

    this->m_pTableView->reloadData();

    auto coverage = DailyListView::calculateNodeCoverage(m_pTableView->m_pContentLayer->getChildren());
    if (this->m_pEntries->count() > 4)
        m_pTableView->m_pContentLayer->setContentSize({-coverage.origin.x + coverage.size.width, -coverage.origin.y + coverage.size.height});

    this->m_pTableView->moveToTop();

    if (this->m_pEntries->count() == 1)
        this->m_pTableView->moveToTopWithOffset(this->m_fItemSeparation);
}

void RewardListView::loadCell(TableViewCell* cell, unsigned int index) {
    static_cast<RewardCell*>(cell)->loadFromReward(
        static_cast<GJRewardItem*>(this->m_pEntries->objectAtIndex(index))
    );
    static_cast<StatsCell*>(cell)->updateBGColor(index);
}

TableViewCell* RewardListView::getListCell(const char* key) {
    return RewardCell::create(key, { this->m_fWidth, this->m_fItemSeparation });
}
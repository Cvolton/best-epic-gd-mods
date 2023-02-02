#include "LeaderboardListView.h"
#include "DailyListView.h"
#include "../managers/CvoltonManager.h"

using namespace gd;
using namespace cocos2d;

LeaderboardListView* LeaderboardListView::create(cocos2d::CCArray* scores, float width, float height) {
    auto ret = new LeaderboardListView();
    if (ret && ret->init(scores, 0x420, width, height)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void LeaderboardListView::setupList() {
    this->m_fItemSeparation = 60.0f;

    this->m_pTableView->retain();
    this->m_pTableView->reloadData();

    auto coverage = DailyListView::calculateNodeCoverage(m_pTableView->m_pContentLayer->getChildren());
    if (this->m_pEntries->count() > 4)
        m_pTableView->m_pContentLayer->setContentSize({-coverage.origin.x + coverage.size.width, -coverage.origin.y + coverage.size.height});

    this->m_pTableView->moveToTop();

    if (this->m_pEntries->count() == 1)
        this->m_pTableView->moveToTopWithOffset(this->m_fItemSeparation);
}

void LeaderboardListView::loadCell(TableViewCell* cell, unsigned int index) {
    static_cast<GJScoreCell*>(cell)->loadFromScore(
        static_cast<GJUserScore*>(this->m_pEntries->objectAtIndex(index))
    );
    static_cast<GJScoreCell*>(cell)->updateBGColor(index);
}

TableViewCell* LeaderboardListView::getListCell(const char* key) {
    return GJScoreCell::create(key, this->m_fWidth, this->m_fItemSeparation);
}
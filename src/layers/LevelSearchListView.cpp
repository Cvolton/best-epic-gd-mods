#include "LevelSearchListView.h"
#include "DailyListView.h"
#include "../managers/CvoltonManager.h"

using namespace gd;
using namespace cocos2d;

LevelSearchListView* LevelSearchListView::create(cocos2d::CCArray* levels, float width, float height) {
    auto ret = new LevelSearchListView();
    if (ret && ret->init(levels, 0x420, width, height)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void LevelSearchListView::setupList() {
    this->m_fItemSeparation = 90.0f;

    this->m_pTableView->reloadData();

    auto coverage = DailyListView::calculateNodeCoverage(m_pTableView->m_pContentLayer->getChildren());
    if (this->m_pEntries->count() > 4)
        m_pTableView->m_pContentLayer->setContentSize({-coverage.origin.x + coverage.size.width, -coverage.origin.y + coverage.size.height});

    this->m_pTableView->moveToTop();

    if (this->m_pEntries->count() == 1)
        this->m_pTableView->moveToTopWithOffset(this->m_fItemSeparation);
}

void LevelSearchListView::loadCell(TableViewCell* cell, unsigned int index) {
    static_cast<LevelCell*>(cell)->loadFromLevel(
        static_cast<GJGameLevel*>(this->m_pEntries->objectAtIndex(index))
    );
    static_cast<LevelCell*>(cell)->updateBGColor(index);
}

TableViewCell* LevelSearchListView::getListCell(const char* key) {
    return LevelCell::create(key, this->m_fWidth, this->m_fItemSeparation);
}
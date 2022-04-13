#include "MoreStatsListView.h"
#include "MoreStatsCell.h"
#include "../managers/CvoltonManager.h"

using namespace gd;
using namespace cocos2d;

MoreStatsListView* MoreStatsListView::create(cocos2d::CCArray* levels, float width, float height) {
    auto ret = new MoreStatsListView();
    if (ret && ret->init(levels, 0x420, width, height)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void MoreStatsListView::setupList() {
    this->m_fItemSeparation = 40.0f;

    this->m_pTableView->reloadData();

    this->m_pTableView->moveToTop();

    if (this->m_pEntries->count() == 1)
        this->m_pTableView->moveToTopWithOffset(this->m_fItemSeparation);
}

void MoreStatsListView::loadCell(TableViewCell* cell, unsigned int index) {
    static_cast<MoreStatsCell*>(cell)->loadFromKey(
        static_cast<CCString*>(this->m_pEntries->objectAtIndex(index))
    );
    static_cast<StatsCell*>(cell)->updateBGColor(index);
}

TableViewCell* MoreStatsListView::getListCell(const char* key) {
    return MoreStatsCell::create(key, { this->m_fWidth, this->m_fItemSeparation });
}
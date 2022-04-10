#include "DailyListView.h"
#include "DailyCell.h"
#include "../managers/CvoltonManager.h"

using namespace gd;
using namespace cocos2d;

DailyListView* DailyListView::create(cocos2d::CCArray* levels, float width, float height) {
    auto ret = new DailyListView();
    if (ret && ret->init(levels, 0x420, width, height)) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

void DailyListView::setupList() {
    this->m_fItemSeparation = 55.0f;

    this->m_pTableView->reloadData();

    if (this->m_pEntries->count() == 1)
        this->m_pTableView->moveToTopWithOffset(this->m_fItemSeparation);
    
    this->m_pTableView->moveToTop();
}

void DailyListView::loadCell(TableViewCell* cell, unsigned int index) {
    static_cast<DailyCell*>(cell)->loadFromLevel(
        static_cast<GJGameLevel*>(this->m_pEntries->objectAtIndex(index))
    );
    static_cast<StatsCell*>(cell)->updateBGColor(index);
}

TableViewCell* DailyListView::getListCell(const char* key) {
    return DailyCell::create(key, { this->m_fWidth, 55.0f });
}
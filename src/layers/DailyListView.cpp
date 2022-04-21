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

    auto coverage = calculateNodeCoverage(m_pTableView->m_pContentLayer->getChildren());
    m_pTableView->m_pContentLayer->setContentSize({-coverage.origin.x + coverage.size.width, -coverage.origin.y + coverage.size.height});

    this->m_pTableView->moveToTop();

    if (this->m_pEntries->count() == 1)
        this->m_pTableView->moveToTopWithOffset(this->m_fItemSeparation);
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

/*
    https://github.com/geode-sdk/loader/blob/77c47653778db8c77d86e483996bc84b7dad26f7/src/utils/node.cpp#L6
    will switch to geode implementation in geode version assuming that ever happens
    (scuffed rushed re-implementation below)
*/
CCRect DailyListView::calculateNodeCoverage(CCArray* nodes){
    CCRect coverage;
    CCObject* childObj;
    CCARRAY_FOREACH(nodes, childObj) {
        CCNode* child = dynamic_cast<CCNode*>(childObj);
        if(child == nullptr) continue;

        auto contentSize = child->getScaledContentSize();
        contentSize.width *= child->getAnchorPoint().x;
        contentSize.height *= child->getAnchorPoint().y;
        auto pos = child->getPosition() - contentSize;

        contentSize = child->getScaledContentSize();
        auto contentPoint = CCPoint { 1.f, 1.f } - child->getAnchorPoint();
        contentSize.width *= contentPoint.x;
        contentSize.height *= contentPoint.y;

        auto csize = child->getPosition() + contentSize;
        if (pos.x < coverage.origin.x) {
            coverage.origin.x = pos.x;
        }
        if (pos.y < coverage.origin.y) {
            coverage.origin.y = pos.y;
        }
        if (csize.x > coverage.size.width) {
            coverage.size.width = csize.x;
        }
        if (csize.y > coverage.size.height) {
            coverage.size.height = csize.y;
        }
    }
    return coverage;
}
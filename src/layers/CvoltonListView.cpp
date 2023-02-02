#include "CvoltonListView.h"

using namespace gd;
using namespace cocos2d;

bool CvoltonListView::init(cocos2d::CCArray* entries, int btype, float width, float height) {
    if(!CustomListView::init(entries, btype, width, height)) return false;

    m_pTableView->retain();
    return true;
}

CvoltonListView::~CvoltonListView() {
    m_pTableView->release();
}
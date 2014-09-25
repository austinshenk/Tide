#include <QTabBar>
#include <QFileInfo>

#include "tabwidget.h"
#include "textedit.h"

TabWidget::TabWidget()
{
    setMovable(true);
    setTabsClosable(true);
    setTabShape(QTabWidget::Rounded);
    setTabPosition(QTabWidget::North);
    tabBar()->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
    setDocumentMode(true);

    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(requestTabtoClose(int)));
}

void TabWidget::requestTabtoClose(int index) {
    removeTab(index);
}

void TabWidget::markTab() {
    TextEdit *editor = (TextEdit*) currentWidget();
    if(editor != NULL && !editor->isMarked()) {
        editor->setMarked(true);
        setTabText(currentIndex(), tr("*%1").arg(tabText(currentIndex())));
    }
}

void TabWidget::unMarkTab(int index) {
    TextEdit *editor = (TextEdit*) widget(index);
    if(editor->isMarked()) {
        editor->setMarked(false);
        QFileInfo info(editor->getFileName());
        setTabText(index, tr("%1.%2").arg(info.baseName()).arg(info.completeSuffix()));
    }
}

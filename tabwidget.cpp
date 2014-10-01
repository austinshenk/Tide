#include <QTabBar>
#include <QFileInfo>
#include <QMessageBox>

#include "tabwidget.h"
#include "textedit.h"
#include "tide.h"
#include "filecontroller.h"

TabWidget::TabWidget(QWidget *parent)
{
    setParent(parent);
    setMovable(true);
    setTabsClosable(true);
    setTabShape(QTabWidget::Rounded);
    setTabPosition(QTabWidget::North);
    tabBar()->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
    setDocumentMode(true);

    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(requestTabtoClose(int)));
}

void TabWidget::giveTide(Tide *tide) {
    this->tide = tide;
}

void TabWidget::requestTabtoClose(int index) {
    TextEdit *editor = (TextEdit*)widget(index);
    if(editor->isMarked()) {
        QMessageBox::StandardButton reply = QMessageBox::warning(this, "Tide", "This document has been modified.\nDo you want to save your changes?",
                             QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
        if(reply == QMessageBox::Save) {
            tide->fileController->saveFile(editor->getFileName(), index);
        } else if (reply == QMessageBox::Discard) {
            removeTab(index);
        } else {
            return;
        }
    } else {
        removeTab(index);
    }
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
    if(editor != NULL && editor->isMarked()) {
        editor->setMarked(false);
        QFileInfo info(editor->getFileName());
        setTabText(index, tr("%1.%2").arg(info.baseName()).arg(info.completeSuffix()));
    }
}

void TabWidget::closeAllTabs() {
    int c = count();
    for(int i=0; i<c; i++) {
        removeTab(0);
    }
}

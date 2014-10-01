#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

#include "tide.h"

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget *parent = 0);
    void giveTide(Tide *tide);
    void unMarkTab(int index);
    void closeAllTabs();

private:
    Tide *tide;

public slots:
    void markTab();

private slots:
    void requestTabtoClose(int index);
};

#endif // TABWIDGET_H

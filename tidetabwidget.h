#ifndef TIDETABWIDGET_H
#define TIDETABWIDGET_H

#include <QTabWidget>

#include "tide.h"

class TideTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    TideTabWidget(Tide *tide);
    void newFile();
    void loadFile();
    void saveFile();
    void saveAsNewFile();

private:
    Tide *tide;
    QWidget *getCurrentWidget();
    void unMarkTab(int index);

public slots:

private slots:
    void requestTabtoClose(int index);
    void markTab();
    void readFile(const QString &fileName);
    void writeFile(const QString &fileName);
};

#endif // TIDETABWIDGET_H

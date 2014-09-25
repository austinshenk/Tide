#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>

#include "tide.h"
#include "tabwidget.h"

class Tide;
class TabWidget;
class QFileDialog;

class FileController : public QObject
{
    Q_OBJECT
public:
    FileController();
    void giveTabWidget(TabWidget *tabs);
    void giveTide(Tide *tide);
    void newFile();
    void loadFile();
    void saveFile();
    void saveAsNewFile();

private:
    QFileDialog *fileDialog;
    TabWidget *tabs;
    Tide *tide;

private slots:
    void readFile(const QString &fileName);
    void writeFile(const QString &fileName);
};

#endif // FILECONTROLLER_H

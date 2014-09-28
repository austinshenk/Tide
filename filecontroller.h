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
    void loadFile(const QString &name, int pos);
    void saveFile();
    void saveAsNewFile();

private:
    QFileDialog *fileDialog;
    TabWidget *tabs;
    Tide *tide;
    void readFile(const QString &name, int insertAt = -1);

private slots:
    void handleLoading(const QString &name);
    void writeFile(const QString &name);
};

#endif // FILECONTROLLER_H

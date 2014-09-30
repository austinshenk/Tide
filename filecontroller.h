#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>

#include "tide.h"
#include "tabwidget.h"

class Tide;
class QFileDialog;

class FileController : public QObject
{
    Q_OBJECT
public:
    FileController();
    void giveTide(Tide *tide);
    void setDirectory(const QString &dir);
    QString getDirectory();
    void newFile();
    void loadFile();
    void loadFile(const QString &name, int pos);
    void saveFile();
    void saveFile(const QString &name, int pos);
    void saveAsNewFile();
    static QString getShortName(const QString &name, int length);

private:
    QFileDialog *fileDialog;
    Tide *tide;
    QString dir;
    void readFile(const QString &name, int insertAt = -1);

private slots:
    void handleLoading(const QString &name);
    void writeFile(const QString &name);
};

#endif // FILECONTROLLER_H

#ifndef PROJECTCONTROLLER_H
#define PROJECTCONTROLLER_H

#include <QObject>
#include "tide.h"

class Tide;
class QModelIndex;
class QFileDialog;

class ProjectController : public QObject
{
    Q_OBJECT
public:
    ProjectController();
    void giveTide(Tide *tide);
    void newProject();
    void openProject();
    void setDirectory(const QString &dir);
    const QString &getDirectory();

private:
    Tide *tide;
    QFileDialog *fileDialog;
    QString dir;

public slots:
    void openFile(const QModelIndex &index);

private slots:
    void createProject(const QString &name);
    void loadProject(const QString &loc);
};

#endif // PROJECTCONTROLLER_H

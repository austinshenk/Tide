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
    void setDirectory(const QString &dir);
    const QString &getDirectory();

private:
    Tide *tide;
    QFileDialog *fileDialog;
    QString dir;

private slots:
    void createProject(const QString &name);
    void openFile(QModelIndex *index);
};

#endif // PROJECTCONTROLLER_H

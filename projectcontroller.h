#ifndef PROJECTCONTROLLER_H
#define PROJECTCONTROLLER_H

#include <QObject>
#include "tide.h"

class Tide;
class FileController;
class QTreeView;
class QFileDialog;

class ProjectController : public QObject
{
    Q_OBJECT
public:
    ProjectController();
    void giveTide(Tide *tide);
    void giveProjectViewer(QTreeView *viewer);
    void giveFileController(FileController *controller);
    void newProject();
    void setDirectory(const QString &dir);
    const QString &getDirectory();

private:
    Tide *tide;
    QTreeView *viewer;
    QFileDialog *fileDialog;
    FileController *fileController;
    QString dir;

private slots:
    void createProject(const QString &name);
};

#endif // PROJECTCONTROLLER_H

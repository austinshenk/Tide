#ifndef PROJECTCONTROLLER_H
#define PROJECTCONTROLLER_H

#include <QObject>

class QTreeView;
class QFileDialog;

class ProjectController : public QObject
{
    Q_OBJECT
public:
    ProjectController();
    void giveProjectViewer(QTreeView *viewer);
    void newProject();

private:
    QTreeView *viewer;
    QFileDialog *fileDialog;

private slots:
    void createProject(const QString &name);
};

#endif // PROJECTCONTROLLER_H

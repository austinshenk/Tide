#ifndef PROJECTCONTROLLER_H
#define PROJECTCONTROLLER_H

#include <QObject>

class QTreeView;

class ProjectController : public QObject
{
    Q_OBJECT
public:
    ProjectController();
    void giveProjectViewer(QTreeView *viewer);
    void newProject();

private:
    QTreeView *viewer;
};

#endif // PROJECTCONTROLLER_H

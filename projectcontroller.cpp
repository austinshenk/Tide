#include <QFileDialog>

#include "projectcontroller.h"

ProjectController::ProjectController()
{
    fileDialog = new QFileDialog();
    fileDialog->setOption(QFileDialog::ShowDirsOnly, true);
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setWindowModality(Qt::ApplicationModal);
}

void ProjectController::giveProjectViewer(QTreeView *viewer) {
    this->viewer = viewer;
}

void ProjectController::newProject() {
    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(createProject(QString)));
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setVisible(true);
}

void ProjectController::createProject(const QString &name) {

}

#include <QFileDialog>

#include "projectcontroller.h"

ProjectController::ProjectController()
{
    fileDialog = new QFileDialog();
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setOption(QFileDialog::ShowDirsOnly, true);
    fileDialog->setOption(QFileDialog::DontUseNativeDialog, false);
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

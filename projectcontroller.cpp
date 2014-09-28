#include <QFileDialog>
#include <QMessageBox>
#include <QTreeView>
#include <QFileSystemModel>
#include <QDebug>

#include "projectcontroller.h"
#include "filecontroller.h"

ProjectController::ProjectController()
{
    fileDialog = new QFileDialog();
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setOption(QFileDialog::ShowDirsOnly, true);
    fileDialog->setOption(QFileDialog::DontUseNativeDialog, false);
    fileDialog->setWindowModality(Qt::ApplicationModal);
    fileDialog->setNameFilter(tr("Directory"));
}

void ProjectController::giveTide(Tide *tide) {
    this->tide = tide;
}

void ProjectController::giveProjectViewer(QTreeView *viewer) {
    this->viewer = viewer;
    this->viewer->setRootIsDecorated(false);
    this->viewer->setUniformRowHeights(true);
    this->viewer->setHeaderHidden(true);
}

void ProjectController::giveFileController(FileController *controller) {
    this->fileController = controller;
}

void ProjectController::setDirectory(const QString &dir) {
    this->dir = dir;
    fileDialog->setDirectory(dir);
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(dir);
    viewer->setModel(model);
    viewer->setRootIndex(model->index(dir));
    viewer->show();
}

const QString &ProjectController::getDirectory() {
    return dir;
}

void ProjectController::newProject() {
    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(createProject(QString)));
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setVisible(true);
}

void ProjectController::createProject(const QString &name) {
    if(name.isEmpty()) {
        QMessageBox::warning(tide, tr("WARNING"),
                             tr("Please enter a project name"));
        return;
    }
    QDir dir(name);
    if(dir.mkpath(dir.absolutePath())) {
        tide->showMessage(tr("Created project %1").arg(dir.dirName()));

        //Set viewer to new Directory
        setDirectory(dir.absolutePath());

        //Add default files
        QString dependsPath = tr("%1/%2").arg(dir.absolutePath()).arg("depends.txt");
        QFile depends(dependsPath);
        if (!depends.open(QFile::ReadWrite | QFile::Text)) {
            QMessageBox::warning(tide, tr("WARNING"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg("depends.txt")
                                 .arg(depends.errorString()));
            return;
        }
        QString initPath = tr("%1/%2").arg(dir.absolutePath()).arg("init.lua");
        QFile init(initPath);
        if (!init.open(QFile::ReadWrite | QFile::Text)) {
            QMessageBox::warning(tide, tr("WARNING"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg("init.lua")
                                 .arg(init.errorString()));
            return;
        }
    } else {
        QMessageBox::warning(tide, tr("WARNING"),
                             tr("Cannot create project %1.")
                             .arg(name));
        return;
    }
}

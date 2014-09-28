#include <QtWidgets>

#include "tide.h"
#include "ui_tide.h"
#include "tabwidget.h"
#include "filecontroller.h"
#include "projectcontroller.h"

Tide::Tide()
{
    ui = new Ui::Tide();
    ui->setupUi(this);

    QWidget *centralWidget = findChild<QWidget*>("centralWidget");

    viewer = new QTreeView(this);
    viewer->hide();
    centralWidget->layout()->addWidget(viewer);
    tabs = new TabWidget();
    centralWidget->layout()->addWidget(tabs);    

    fileController = new FileController();
    fileController->giveTabWidget(tabs);
    fileController->giveTide(this);

    projectController = new ProjectController();
    projectController->giveProjectViewer(viewer);

    //constructStatusBar();

    connectToolBarActions();
}

void Tide::connectToolBarActions() {
    QAction *menuFileNewMod = findChild<QAction*>("menuFileNewMod");
    connect(menuFileNewMod, SIGNAL(triggered()), this, SLOT(requestNewProject()));

    QAction *menuFileNew = findChild<QAction*>("menuFileNew");
    connect(menuFileNew, SIGNAL(triggered()), this, SLOT(requestNewFile()));

    QAction *menuFileOpen = findChild<QAction*>("menuFileOpen");
    connect(menuFileOpen, SIGNAL(triggered()), this, SLOT(requestOpenFile()));

    QAction *menuFileSave = findChild<QAction*>("menuFileSave");
    connect(menuFileSave, SIGNAL(triggered()), this, SLOT(requestSaveFile()));

    QAction *menuFileSaveas = findChild<QAction*>("menuFileSaveas");
    connect(menuFileSaveas, SIGNAL(triggered()), this, SLOT(requestSaveFileas()));
}

void Tide::showMessage(const QString &text) {
    statusBar()->clearMessage();
    statusBar()->showMessage(text, 2000);
}

void Tide::constructStatusBar() {
    constructSyntaxMenu();
    syntaxChooser = new QPushButton(statusBar());
    syntaxChooser->setText(tr("Text"));
    syntaxChooser->setFlat(true);
    syntaxChooser->setMenu(syntaxMenu);
    statusBar()->addPermanentWidget(syntaxChooser);
}

void Tide::constructSyntaxMenu() {
    syntaxMenu = new QMenu(this);

    QAction *text = new QAction(syntaxMenu);
    text->setText(tr("Plain Text"));
    connect(text, SIGNAL(triggered()), this, SLOT(setSyntaxtoPlainText()));
    syntaxMenu->addAction(text);
}

void Tide::setSyntaxtoPlainText() {

}

void Tide::requestNewProject() {
    projectController->newProject();
}

void Tide::requestNewFile() {
    fileController->newFile();
}

void Tide::requestOpenFile() {
    fileController->loadFile();
}

void Tide::requestSaveFile() {
    fileController->saveFile();
}

void Tide::requestSaveFileas() {
    fileController->saveAsNewFile();
}

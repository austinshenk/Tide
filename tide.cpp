#include <QtWidgets>

#include "tide.h"
#include "ui_tide.h"
#include "tabwidget.h"
#include "textedit.h"
#include "filecontroller.h"
#include "projectcontroller.h"
#include "settingscontroller.h"

Tide::Tide()
{
    ui = new Ui::Tide();
    ui->setupUi(this);

    QWidget *centralWidget = findChild<QWidget*>("centralWidget");

    viewer = new QTreeView(this);
    viewer->hide();
    centralWidget->layout()->addWidget(viewer);
    tabs = new TabWidget(this);
    centralWidget->layout()->addWidget(tabs);    

    fileController = new FileController();
    fileController->giveTide(this);

    projectController = new ProjectController();
    projectController->giveTide(this);

    //constructStatusBar();

    connectToolBarActions();

    readSettings();
}

Tide::~Tide() {
    delete fileController;
    delete projectController;
}

void Tide::connectToolBarActions() {
    QAction *menuFileNewMod = findChild<QAction*>("menuFileNewMod");
    connect(menuFileNewMod, SIGNAL(triggered()), this, SLOT(requestNewProject()));

    QAction *menuFileNew = findChild<QAction*>("menuFileNew");
    connect(menuFileNew, SIGNAL(triggered()), this, SLOT(requestNewFile()));

    QAction *menuFileOpenMod = findChild<QAction*>("menuFileOpenMod");
    connect(menuFileOpenMod, SIGNAL(triggered()), this, SLOT(requestOpenProject()));

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

void Tide::readSettings() {
    QCoreApplication::setOrganizationName("Shenk");
    QCoreApplication::setApplicationName("Tide");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QSettings settings;
    move(settings.value(Settings::MainPos).toPoint());
    resize(settings.value(Settings::MainSize, QSize(400, 400)).toSize());
    int size = settings.beginReadArray(Settings::Files);
    for(int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        if(!settings.value(Settings::FilesName).toString().isEmpty()) {
            fileController->loadFile(settings.value(Settings::FilesName).toString(), i);
        }
    }
    settings.endArray();
    projectController->setDirectory(settings.value(Settings::Project).toString());
    fileController->setDirectory(settings.value(Settings::Project).toString());
}

void Tide::writeSettings() {
    QSettings settings;
    settings.setValue(Settings::MainPos, pos());
    settings.setValue(Settings::MainSize, size());
    settings.beginWriteArray(Settings::Files);
    for(int i = 0; i < tabs->count(); ++i) {
        settings.setArrayIndex(i);
        TextEdit *editor = (TextEdit*)tabs->widget(i);
        if(!editor->getFileName().isEmpty()) {
            settings.setValue(Settings::FilesName, editor->getFileName());
        }
    }
    settings.endArray();
    settings.setValue(Settings::Project, projectController->getDirectory());
}

void Tide::closeEvent(QCloseEvent *event) {
    if(!fileController->checkMarks()) {
        event->ignore();
        return;
    }
    writeSettings();
    event->accept();
}

void Tide::requestNewProject() {
    projectController->newProject();
}

void Tide::requestNewFile() {
    fileController->newFile();
}

void Tide::requestOpenProject() {
    projectController->openProject();
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

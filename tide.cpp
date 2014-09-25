#include <QtWidgets>

#include "tide.h"
#include "ui_tide.h"
#include "tidetabwidget.h"

Tide::Tide()
{
    ui->setupUi(this);

    tabs = new TideTabWidget(this);
    setCentralWidget(tabs);

    constructStatusBar();

    connectToolBarActions();

    fileDialog = new QFileDialog(this);
    fileDialog->setNameFilter(tr("*.txt *.lua"));
    fileDialog->setOption(QFileDialog::DontUseNativeDialog, true);
}

void Tide::connectToolBarActions() {
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

void Tide::requestNewFile() {
    tabs->newFile();
}

void Tide::requestOpenFile() {
    tabs->loadFile();
}

void Tide::requestSaveFile() {
    tabs->saveFile();
}

void Tide::requestSaveFileas() {
    tabs->saveAsNewFile();
}

#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

#include "filecontroller.h"
#include "textedit.h"

FileController::FileController()
{
    fileDialog = new QFileDialog;
    fileDialog->setNameFilter(tr("Modding files (*.txt *.lua)"));
    fileDialog->setOption(QFileDialog::DontUseNativeDialog, false);
    fileDialog->setWindowModality(Qt::ApplicationModal);
    fileDialog->setDefaultSuffix(tr("lua"));
}

void FileController::giveTabWidget(TabWidget *tabs) {
    this->tabs = tabs;
}

void FileController::giveTide(Tide *tide) {
    this->tide = tide;
}

void FileController::newFile() {
    TextEdit *editor = new TextEdit;
    editor->setFileName(tr(""));
    tabs->insertTab((tabs->currentIndex() == -1) ? 0 : tabs->currentIndex(), editor, tr(""));
    connect(editor, SIGNAL(textChanged()), tabs, SLOT(markTab()));
}

void FileController::loadFile() {
    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(handleLoading(QString)));
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setVisible(true);
}

void FileController::loadFile(const QString &name, int pos) {
    readFile(name, pos);
}

void FileController::handleLoading(const QString &name) {
    QFileInfo info(name);
    if(info.isDir()) {
        //Load directory
    } else {
        readFile(name);
    }
}

void FileController::readFile(const QString &name, int insertAt) {
    QFile file(name);
    if (!file.open(QFile::ReadWrite | QFile::Text)) {
        QMessageBox::warning(tide, tr("WARNING"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(name)
                             .arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
    TextEdit *editor = new TextEdit;
    editor->setFileName(name);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    editor->setText(in.readAll());
    QApplication::restoreOverrideCursor();
    QFileInfo info(name);
    if(insertAt == -1) {
        tabs->insertTab((tabs->currentIndex() == -1) ? 0 : tabs->currentIndex(), editor, tr("%1.%2").arg(info.baseName()).arg(info.completeSuffix()));
    } else {
        tabs->insertTab(insertAt, editor, tr("%1.%2").arg(info.baseName()).arg(info.completeSuffix()));
    }
    connect(editor, SIGNAL(textChanged()), tabs, SLOT(markTab()));
    disconnect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(handleLoading(QString)));
}

void FileController::saveFile() {
    if(tabs->currentIndex() != -1) {
        TextEdit *editor = (TextEdit*) tabs->currentWidget();
        if(tabs->tabText(tabs->currentIndex()).isEmpty() || tabs->tabText(tabs->currentIndex()) == "*") {
            saveAsNewFile();
            return;
        }
        if(editor->isMarked()) {
            writeFile(editor->getFileName());
        }
    }
}

void FileController::saveAsNewFile() {
    if(tabs->currentIndex() != -1) {
        connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(writeFile(QString)));
        fileDialog->setFileMode(QFileDialog::AnyFile);
        fileDialog->setAcceptMode(QFileDialog::AcceptSave);
        fileDialog->setVisible(true);
    }
}

void FileController::writeFile(const QString &name) {
    QFile file(name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(tide, tr("WARNING"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(name)
                             .arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    TextEdit *editor = (TextEdit*) tabs->currentWidget();
    editor->setFileName(name);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(editor != NULL) {
        out << editor->toPlainText();
    }
    QApplication::restoreOverrideCursor();
    tide->showMessage(tr("%1 saved").arg(name));
    tabs->unMarkTab(tabs->currentIndex());
    QFileInfo info(name);
    tabs->setTabText(tabs->currentIndex(), tr("%1.%2").arg(info.baseName()).arg(info.completeSuffix()));
    disconnect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(writeFile(QString)));
    return;
}

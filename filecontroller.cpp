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

void FileController::giveTide(Tide *tide) {
    this->tide = tide;
}

void FileController::setDirectory(const QString &dir) {
    this->dir = dir;
    fileDialog->setDirectory(dir);
}

QString FileController::getDirectory() {
    return dir;
}

QString FileController::getShortName(const QString &name, int length) {
    QString shortName = name.right(length);
    return tr("%1%2").arg((shortName.length() == name.length())? "" : "...").arg(shortName);
}

void FileController::newFile() {
    TextEdit *editor = new TextEdit;
    editor->setFileName(tr(""));
    tide->tabs->insertTab((tide->tabs->currentIndex() == -1) ? 0 : tide->tabs->currentIndex(), editor, tr(""));
    connect(editor, SIGNAL(textChanged()), tide->tabs, SLOT(markTab()));
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
        tide->tabs->insertTab((tide->tabs->currentIndex() == -1) ? 0 : tide->tabs->currentIndex(), editor, tr("%1.%2").arg(info.baseName()).arg(info.completeSuffix()));
    } else {
        tide->tabs->insertTab(insertAt, editor, tr("%1.%2").arg(info.baseName()).arg(info.completeSuffix()));
    }
    connect(editor, SIGNAL(textChanged()), tide->tabs, SLOT(markTab()));
    disconnect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(handleLoading(QString)));
}

void FileController::saveFile() {
    if(tide->tabs->currentIndex() != -1) {
        TextEdit *editor = (TextEdit*) tide->tabs->currentWidget();
        if(tide->tabs->tabText(tide->tabs->currentIndex()).isEmpty() || tide->tabs->tabText(tide->tabs->currentIndex()) == "*") {
            saveAsNewFile();
            return;
        }
        if(editor->isMarked()) {
            writeFile(editor->getFileName());
        }
    }
}

void FileController::saveFile(const QString &name, int pos) {
    QFile file(name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(tide, tr("WARNING"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(name)
                             .arg(file.errorString()));
        return;
    }
    QTextStream out(&file);
    TextEdit *editor = (TextEdit*) tide->tabs->widget(pos);
    editor->setFileName(name);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(editor != NULL) {
        out << editor->toPlainText();
    }
    QApplication::restoreOverrideCursor();
    tide->showMessage(tr("Saved %1").arg(getShortName(name, 20)));
}

void FileController::saveAsNewFile() {
    if(tide->tabs->currentIndex() != -1) {
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
    TextEdit *editor = (TextEdit*) tide->tabs->currentWidget();
    editor->setFileName(name);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(editor != NULL) {
        out << editor->toPlainText();
    }
    QApplication::restoreOverrideCursor();
    tide->showMessage(tr("Saved %1").arg(getShortName(name, 20)));
    tide->tabs->unMarkTab(tide->tabs->currentIndex());
    QFileInfo info(name);
    tide->tabs->setTabText(tide->tabs->currentIndex(), tr("%1.%2").arg(info.baseName()).arg(info.completeSuffix()));
    disconnect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(writeFile(QString)));
    return;
}

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

int FileController::fileTabExists(const QString &name) {
    for(int i=0; i<tide->tabs->count(); i++) {
        TextEdit *editor = (TextEdit *)tide->tabs->widget(i);
        if(editor->getFileName() == name) {
            return i;
        }
    }
    return -1;
}

void FileController::loadFile() {
    disconnect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(handleLoading(QString)));
    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(handleLoading(QString)));
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setVisible(true);
}

void FileController::loadFile(const QString &name, int pos) {
    int exists = fileTabExists(name);
    if(exists == -1) {
        readFile(name, pos);
    } else {
        tide->tabs->setCurrentIndex(exists);
    }
}

void FileController::handleLoading(const QString &name) {
    int exists = fileTabExists(name);
    if(exists != -1) {
        tide->tabs->setCurrentIndex(exists);
        return;
    }
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
    editor->setMarked(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(editor != NULL) {
        out << editor->toPlainText();
    }
    QApplication::restoreOverrideCursor();
    tide->tabs->unMarkTab(pos);
    tide->showMessage(tr("Saved %1").arg(getShortName(name, 20)));
}

void FileController::saveAsNewFile() {
    if(tide->tabs->currentIndex() != -1) {
        disconnect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(writeFile(QString)));
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
    return;
}

bool FileController::checkMarks() {
    bool hasUnsavedNewDocument = false;
    for(int i=0; i<tide->tabs->count(); i++) {
        TextEdit *editor = (TextEdit *)tide->tabs->widget(i);
        if(editor->isMarked() && editor->getFileName().isEmpty()) {
            hasUnsavedNewDocument = true;
            break;
        }
    }
    if(hasUnsavedNewDocument) {
        QMessageBox::StandardButton reply = QMessageBox::warning(tide, tr("Tide"), tr("A new document has unsaved changes.\nPlease save the changes, otherwise discard."),
                             QMessageBox::Ok|QMessageBox::Discard|QMessageBox::Cancel);
        if(reply == QMessageBox::Ok || reply == QMessageBox::Cancel) {
            return false;
        } else if(reply == QMessageBox::Discard) {
            //Do Nothing
        }
    }
    bool hasUnsavedChanges = false;
    for(int i=0; i<tide->tabs->count(); i++) {
        TextEdit *editor = (TextEdit *)tide->tabs->widget(i);
        if(editor->isMarked() && !editor->getFileName().isEmpty()) {
            hasUnsavedChanges = true;
            break;
        }
    }
    if(hasUnsavedChanges) {
        QMessageBox::StandardButton reply = QMessageBox::warning(tide, tr("Tide"), tr("Multiple documents have unsaved changes.\nWould you like to save those changes?"),
                             QMessageBox::SaveAll|QMessageBox::Discard|QMessageBox::Cancel);
        if(reply == QMessageBox::SaveAll) {
            for(int i=0; i<tide->tabs->count(); i++) {
                TextEdit *editor = (TextEdit *)tide->tabs->widget(i);
                if(editor->isMarked() && !editor->getFileName().isEmpty()) {
                    saveFile(editor->getFileName(), i);
                }
            }
        } else if(reply == QMessageBox::Discard) {
            //Do Nothing
        } else if(reply == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

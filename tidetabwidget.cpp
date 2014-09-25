#include <QTabBar>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>

#include "tidetabwidget.h"
#include "tide.h"
#include "tidetextedit.h"

TideTabWidget::TideTabWidget(Tide *tide)
{
    this->tide = tide;
    setMovable(true);
    setTabsClosable(true);
    setTabShape(QTabWidget::Rounded);
    setTabPosition(QTabWidget::North);
    tabBar()->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
    setDocumentMode(true);

    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(requestTabtoClose(int)));
}

void TideTabWidget::requestTabtoClose(int index) {
    removeTab(index);
}

void TideTabWidget::markTab() {
    TideTextEdit *editor = (TideTextEdit*) getCurrentWidget();
    if(editor != NULL && !editor->isMarked()) {
        editor->setMarked(true);
        setTabText(currentIndex(), tr("*%1").arg(tabText(currentIndex())));
    }
}

void TideTabWidget::unMarkTab(int index) {
    TideTextEdit *editor = (TideTextEdit*) widget(index);
    if(editor->isMarked()) {
        editor->setMarked(false);
        QFileInfo info(editor->getFileName());
        setTabText(index, tr("%1.%2").arg(info.baseName()).arg(info.completeSuffix()));
    }
}

void TideTabWidget::newFile() {
    TideTextEdit *editor = new TideTextEdit;
    editor->setFileName(tr(""));
    insertTab((currentIndex() == -1) ? 0 : currentIndex(), editor, tr(""));
    connect(editor, SIGNAL(textChanged()), this, SLOT(markTab()));
}

void TideTabWidget::loadFile() {
    connect(tide->fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(readFile(QString)));
    tide->fileDialog->setFileMode(QFileDialog::ExistingFile);
    tide->fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    tide->fileDialog->setVisible(true);
}

void TideTabWidget::readFile(const QString &name) {
    QFile file(name);
    if (!file.open(QFile::ReadWrite | QFile::Text)) {
        QMessageBox::warning(this, tr("WARNING"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(name)
                             .arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
    TideTextEdit *editor = new TideTextEdit;
    editor->setFileName(name);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    editor->setText(in.readAll());
    QApplication::restoreOverrideCursor();
    QFileInfo info(name);
    insertTab((currentIndex() == -1) ? 0 : currentIndex(), editor, tr("%1.%2").arg(info.baseName()).arg(info.completeSuffix()));
    connect(editor, SIGNAL(textChanged()), this, SLOT(markTab()));
    disconnect(tide->fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(readFile(QString)));
}

void TideTabWidget::saveFile() {
    if(currentIndex() != -1) {
        TideTextEdit *editor = (TideTextEdit*) currentWidget();
        if(tabText(currentIndex()).isEmpty() || tabText(currentIndex()) == "*") {
            saveAsNewFile();
            return;
        }
        if(editor->document()->isModified()) {
            writeFile(editor->getFileName());
        }
    }
}

void TideTabWidget::saveAsNewFile() {
    if(currentIndex() != -1) {
        connect(tide->fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(writeFile(QString)));
        tide->fileDialog->setFileMode(QFileDialog::AnyFile);
        tide->fileDialog->setAcceptMode(QFileDialog::AcceptSave);
        tide->fileDialog->setVisible(true);
    }
}

QWidget *TideTabWidget::getCurrentWidget() {
    if(currentIndex() != -1) {
        return currentWidget();
    } else {
        return NULL;
    }
}

void TideTabWidget::writeFile(const QString &name) {
    QFile file(name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("WARNING"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(name)
                             .arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    TideTextEdit *editor = (TideTextEdit*) getCurrentWidget();
    editor->setFileName(name);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(editor != NULL) {
        out << editor->toPlainText();
    }
    QApplication::restoreOverrideCursor();
    tide->showMessage(tr("%1 saved").arg(name));
    unMarkTab(currentIndex());
    disconnect(tide->fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(writeFile(QString)));
    return;
}

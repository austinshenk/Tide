#ifndef TIDE_H
#define TIDE_H

#include <QMainWindow>

#include "filecontroller.h"

namespace Ui {
class Tide;
}

class QAction;
class TabWidget;
class FileController;
class QPushButton;

class Tide : public QMainWindow
{
    Q_OBJECT

public:
    Tide();
    void showMessage(const QString &text);

private slots:
    void requestNewFile();
    void requestOpenFile();
    void requestSaveFile();
    void requestSaveFileas();
    void setSyntaxtoPlainText();

private:
    void connectToolBarActions();
    void constructStatusBar();
    void constructSyntaxMenu();
    Ui::Tide *ui;
    TabWidget *tabs;
    FileController *fileController;
    QPushButton *syntaxChooser;
    QMenu *syntaxMenu;
};

#endif // TIDE_H

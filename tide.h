#ifndef TIDE_H
#define TIDE_H

#include <QMainWindow>

namespace Ui {
class Tide;
}

class QHBoxLayout;
class QAction;
class QTreeView;
class TabWidget;
class FileController;
class ProjectController;
class QPushButton;

class Tide : public QMainWindow
{
    Q_OBJECT

public:
    Tide();
    void showMessage(const QString &text);

private slots:
    void requestNewProject();
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
    QTreeView *viewer;
    TabWidget *tabs;
    FileController *fileController;
    ProjectController *projectController;
    QPushButton *syntaxChooser;
    QMenu *syntaxMenu;
};

#endif // TIDE_H

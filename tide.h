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
    ~Tide();
    void showMessage(const QString &text);
    QTreeView *viewer;
    TabWidget *tabs;
    FileController *fileController;
    ProjectController *projectController;

private slots:
    void requestNewProject();
    void requestNewFile();
    void requestOpenProject();
    void requestOpenFile();
    void requestSaveFile();
    void requestSaveFileas();
    void setSyntaxtoPlainText();

private:
    void connectToolBarActions();
    void constructStatusBar();
    void constructSyntaxMenu();
    void readSettings();
    void writeSettings();
    void closeEvent(QCloseEvent *event);
    Ui::Tide *ui;
    QPushButton *syntaxChooser;
    QMenu *syntaxMenu;
};

#endif // TIDE_H

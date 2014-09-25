#ifndef TIDE_H
#define TIDE_H

#include <QMainWindow>

namespace Ui {
class Tide;
}

class QAction;
class TideTabWidget;
class QPushButton;
class QFileDialog;

class Tide : public QMainWindow
{
    Q_OBJECT

public:
    Tide();
    QFileDialog *fileDialog;
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
    TideTabWidget *tabs;
    QPushButton *syntaxChooser;
    QMenu *syntaxMenu;
};

#endif // TIDE_H

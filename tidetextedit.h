#ifndef TIDETEXTEDIT_H
#define TIDETEXTEDIT_H

#include <QTextEdit>

class TideTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    TideTextEdit();
    const QString &getFileName();
    void setFileName(const QString &name);
    bool isMarked();
    void setMarked(bool marked);

private:
    QString fileName;
    bool marked;
};

#endif // TIDETEXTEDIT_H

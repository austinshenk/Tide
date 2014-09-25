#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    TextEdit();
    const QString &getFileName();
    void setFileName(const QString &name);
    bool isMarked();
    void setMarked(bool marked);

private:
    QString fileName;
    bool marked;
};

#endif // TEXTEDIT_H

#include "tidetextedit.h"

TideTextEdit::TideTextEdit()
{
    setTabChangesFocus(false);
    setUndoRedoEnabled(true);
    marked = false;
}

const QString &TideTextEdit::getFileName() {
    return fileName;
}

void TideTextEdit::setFileName(const QString &name) {
    fileName = name;
}

bool TideTextEdit::isMarked() {
    return marked;
}

void TideTextEdit::setMarked(bool marked) {
    this->marked = marked;
}

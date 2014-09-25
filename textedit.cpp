#include "TextEdit.h"

TextEdit::TextEdit()
{
    setTabChangesFocus(false);
    setUndoRedoEnabled(true);
    marked = false;
}

const QString &TextEdit::getFileName() {
    return fileName;
}

void TextEdit::setFileName(const QString &name) {
    fileName = name;
}

bool TextEdit::isMarked() {
    return marked;
}

void TextEdit::setMarked(bool marked) {
    this->marked = marked;
}

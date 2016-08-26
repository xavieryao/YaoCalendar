#include "dndtextedit.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDebug>

DnDTextEdit::DnDTextEdit(QWidget* parent) : QTextEdit(parent)
{
    setAcceptDrops(true);
}

void DnDTextEdit::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls() && e->mimeData()->urls().size() == 1) {
        e->acceptProposedAction();
    }
}

void DnDTextEdit::dropEvent(QDropEvent *e)
{
    emit fileDropped(e->mimeData()->urls().first());
}

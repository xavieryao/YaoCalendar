#ifndef DNDTEXTEDIT_H
#define DNDTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <QMimeData>
#include <QUrl>

class DnDTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    DnDTextEdit(QWidget* parent = 0);

signals:
    void fileDropped(const QUrl& url);

protected:
    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent(QDropEvent *e);
};

#endif // DNDTEXTEDIT_H

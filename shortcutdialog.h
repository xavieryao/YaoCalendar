#ifndef SHORTCUTDIALOG_H
#define SHORTCUTDIALOG_H

#include <QDialog>
#include "types.h"

class ShortcutDialog : public QDialog
{
    Q_OBJECT
public:
    ShortcutDialog(QWidget* parent = 0);

private:
    QString descriptionForAction(ShortcutAction action);
};

#endif // SHORTCUTDIALOG_H

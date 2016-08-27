#ifndef SHORTCUTDIALOG_H
#define SHORTCUTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "types.h"

class ShortcutDialog : public QDialog
{
    Q_OBJECT
public:
    ShortcutDialog(QWidget* parent = 0);
    static QString descriptionForAction(ShortcutAction action);


private:
    void setUpTable(QTableWidget* table);
};

#endif // SHORTCUTDIALOG_H

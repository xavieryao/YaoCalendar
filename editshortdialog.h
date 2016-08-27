#ifndef EDITSHORTDIALOG_H
#define EDITSHORTDIALOG_H
#include <QDialog>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include "types.h"
#include "shortcutdialog.h"
#include "types.h"

class EditShortDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EditShortDialog(QWidget* parent = 0, QString shortCut = QString(), ShortcutAction act = ShortcutAction::NEW) : QDialog(parent)
    {
        setWindowTitle(tr("Edit Shortcut"));
        QVBoxLayout* root = new QVBoxLayout(this);
        QWidget* layWid = new QWidget(this);
        QHBoxLayout* lay = new QHBoxLayout(this);
        QLineEdit* le = new QLineEdit(this);
        le->setPlaceholderText(tr("e.g. Ctrl+C"));

        QComboBox* cb = new QComboBox(this);
        for (int i = 0; i < 6; i++) {
            ShortcutAction act = ShortcutAction(i);
            cb->addItem(ShortcutDialog::descriptionForAction(act), QVariant::fromValue(act));
        }

        if (shortCut != QString()) {
            le->setText(shortCut);
            cb->setCurrentIndex(act);
        }

        lay->addWidget(le);
        lay->addWidget(cb);
        layWid->setLayout(lay);

        QWidget* btnWid = new QWidget(this);
        QHBoxLayout* btnLay = new QHBoxLayout(this);
        QPushButton* btnCancel = new QPushButton(tr("Cancel"), this);
        QPushButton* btnOk = new QPushButton(tr("Ok"), this);

        connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
        connect(btnOk, &QPushButton::clicked, [=]{
            if (le->text().isEmpty()) {
                reject();
            } else {
                emit modified(le->text(), cb->currentData().value<ShortcutAction>());
                accept();
            }
        });

        btnWid->setLayout(btnLay);
        btnLay->addStretch();
        btnLay->addWidget(btnCancel);
        btnLay->addWidget(btnOk);

        root->addWidget(layWid);
        root->addWidget(btnWid);
        setLayout(root);
    }

signals:
    void modified(QString key, ShortcutAction act);
};
#endif // EDITSHORTDIALOG_H

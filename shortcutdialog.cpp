#include "shortcutdialog.h"
#include <QSettings>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QKeySequence>
#include <QFont>
#include <QLineEdit>
#include <QComboBox>

class EditShortDialog : public QDialog
{
public:
    explicit EditShortDialog(QWidget* parent = 0) : QDialog(parent)
    {
        QHBoxLayout* lay = new QHBoxLayout(this);
        QLineEdit* le = new QLineEdit(this);
        le->setPlaceholderText(tr("Insert Shortcut Here"));
        QComboBox* cb = new QComboBox(this);
        for (int i = 0; i < 6; i++) {

        }
    }
};

ShortcutDialog::ShortcutDialog(QWidget* parent) : QDialog(parent)
{
    QSettings settings("./config.plist", QSettings::IniFormat);

    QTableWidget* table = new QTableWidget(this);
    QMap<QString, QVariant> map = settings.value("shortcuts").toMap();
    table->setColumnCount(2);
    QList<QString> keys = map.keys();
    table->setRowCount(keys.size());
    for (int i = 0; i < keys.size(); i++) {
        QString des = descriptionForAction(map.value(keys.at(i)).value<ShortcutAction>());
        QString shortCut = QKeySequence(keys.at(i), QKeySequence::PortableText).toString(QKeySequence::NativeText);
        table->setItem(i, 0, new QTableWidgetItem(shortCut));
        table->setItem(i, 1, new QTableWidgetItem(des));
    }
    QVBoxLayout* root = new QVBoxLayout(this);
    setLayout(root);
    root->addWidget(table);

    QWidget* ctrlWid = new QWidget(this);
    QHBoxLayout* ctrlLay = new QHBoxLayout(this);
    ctrlWid->setLayout(ctrlLay);

    QFont font;
    font.setFamily("FontAwesome");

    QPushButton* btnAdd = new QPushButton(this);
    QPushButton* btnRemove = new QPushButton(this);
    QPushButton* btnEdit = new QPushButton(this);

    btnAdd->setText(QChar(0xf067));
    btnRemove->setText(QChar(0xf068));
    btnEdit->setText(QChar(0xf013));
    btnAdd->setFont(font);
    btnRemove->setFont(font);
    btnEdit->setFont(font);

    ctrlLay->addStretch();
    ctrlLay->addWidget(btnAdd);
    ctrlLay->addWidget(btnRemove);
    ctrlLay->addWidget(btnEdit);

    root->addWidget(ctrlWid);
}

QString ShortcutDialog::descriptionForAction(ShortcutAction action)
{
    switch (action) {
    case ShortcutAction::NEW:
        return QObject::tr("New event");
    case ShortcutAction::DELETE:
        return QObject::tr("Delete event");
    case ShortcutAction::NEXT_MONTH:
        return tr("Show next month");
    case ShortcutAction::PREV_MONTH:
        return tr("Show previous month");
    case ShortcutAction::TODAY:
        return tr("Show today");
    case ShortcutAction::PIN:
        return tr("Pin calendar");
    default:
        break;
    }
    return "Undefined";
}

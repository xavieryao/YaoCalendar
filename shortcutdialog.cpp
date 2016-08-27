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
#include "editshortdialog.h"

ShortcutDialog::ShortcutDialog(QWidget* parent) : QDialog(parent)
{
    QSettings settings("./config.plist", QSettings::IniFormat);

    QTableWidget* table = new QTableWidget(this);
    setUpTable(table);

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

    connect(btnAdd, &QPushButton::clicked, [=]{
       EditShortDialog* dialog = new EditShortDialog(this);
       connect(dialog, &EditShortDialog::modified, [=](QString key, ShortcutAction act){
           QMap<QString, QVariant> map = settings.value("shortcuts").toMap();
           map.insert(key, QVariant::fromValue(act));
           settings.setValue("shortcuts", map);
           setUpTable(table);
       });
       dialog->exec();
    });

    connect(btnRemove, &QPushButton::clicked, [=]{
        if (table->selectedItems().size() == 0) {
            return;
        }
        QMap<QString, QVariant> map = settings.value("shortcuts").toMap();
//        map.remove(table->selectedItems().first())
        // TODO!
        setUpTable(table);
    });

    ctrlLay->addStretch();
    ctrlLay->addWidget(btnAdd);
    ctrlLay->addWidget(btnRemove);
    ctrlLay->addWidget(btnEdit);

    root->addWidget(ctrlWid);
}

void ShortcutDialog::setUpTable(QTableWidget* table)
{
    QSettings settings("./config.plist", QSettings::IniFormat);
    table->setSelectionBehavior(QTableWidget::SelectRows);
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

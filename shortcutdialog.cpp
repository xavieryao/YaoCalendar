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
#include <QDebug>
#include "editshortdialog.h"

ShortcutDialog::ShortcutDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("Shortcuts"));
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
           QSettings settings("./config.ini", QSettings::IniFormat);
           QMap<QString, QVariant> map = settings.value("shortcuts").toMap();
           map.insert(key, static_cast<int>(act));
           settings.setValue("shortcuts", map);
           setUpTable(table);
       });
       dialog->exec();
    });

    connect(btnRemove, &QPushButton::clicked, [=]{
        if (table->selectedItems().size() == 0) {
            return;
        }
        QSettings settings("./config.ini", QSettings::IniFormat);
        QMap<QString, QVariant> map = settings.value("shortcuts").toMap();
        qDebug() << "remove" << table->selectedItems().first()->data(Qt::UserRole).toString();
        map.remove(table->selectedItems().first()->data(Qt::UserRole).toString());
        settings.setValue("shortcuts", map);
        setUpTable(table);
    });

    connect(btnEdit, &QPushButton::clicked, [=]{
        if (table->selectedItems().size() == 0) {
            return;
        }
        EditShortDialog* dialog =
                new EditShortDialog(this, table->selectedItems().first()->data(Qt::UserRole).toString(),
                                    ShortcutAction(table->selectedItems()[1]->data(Qt::UserRole).toInt()));
        connect(dialog, &EditShortDialog::modified, [=](QString key, ShortcutAction act){
            QSettings settings("./config.ini", QSettings::IniFormat);
            QMap<QString, QVariant> map = settings.value("shortcuts").toMap();
            map.remove(table->selectedItems().first()->data(Qt::UserRole).toString());
            map.insert(key, static_cast<int>(act));
            settings.setValue("shortcuts", map);
            setUpTable(table);
        });
        dialog->exec();
    });

    ctrlLay->addStretch();
    ctrlLay->addWidget(btnAdd);
    ctrlLay->addWidget(btnRemove);
    ctrlLay->addWidget(btnEdit);

    root->addWidget(ctrlWid);
}

void ShortcutDialog::setUpTable(QTableWidget* table)
{
    QSettings settings("./config.ini", QSettings::IniFormat);
    table->setSelectionBehavior(QTableWidget::SelectRows);
    QMap<QString, QVariant> map = settings.value("shortcuts").toMap();
    table->setColumnCount(2);
    QList<QString> keys = map.keys();
    table->setRowCount(keys.size());
    for (int i = 0; i < keys.size(); i++) {
        ShortcutAction act = map.value(keys.at(i)).value<ShortcutAction>();
        QString des = descriptionForAction(act);
        QString shortCut = QKeySequence(keys.at(i), QKeySequence::PortableText).toString(QKeySequence::NativeText);
        QTableWidgetItem* item1 = new QTableWidgetItem(shortCut);
        QTableWidgetItem* item2 = new QTableWidgetItem(des);
        item1->setData(Qt::UserRole, keys.at(i));
        item2->setData(Qt::UserRole, static_cast<int>(act));

        table->setItem(i, 0, item1);
        table->setItem(i, 1, item2);
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

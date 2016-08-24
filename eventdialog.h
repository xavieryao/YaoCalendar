#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QColor>
#include "calendarevent.h"

class ColorButton : public QPushButton {
    Q_OBJECT
public:
    explicit ColorButton(QWidget* parent = 0) : QPushButton(parent) {
        QFont font;
        font.setFamily("FontAwesome");
        this->setFont(font);
        this->setText(QChar(0xf0c8));
    }

    void setColor(const QColor color) {
        this->mColor = color;
        this->setStyleSheet(QString("QPushButton{color: rgb(%1, %2, %3);}")
            .arg(color.red()).arg(color.green()).arg(color.blue()));
    }
    QColor color() const {
        return mColor;
    }
private:
    QColor mColor;
};

class EventDialog : public QWidget
{
    Q_OBJECT
public:
    explicit EventDialog(QWidget *parent = 0);
    void setEvent(const CalendarEvent event);
signals:

public slots:
    void chooseColor();
private:
    CalendarEvent mEvent;

    QVBoxLayout* mRootLayout;
    QWidget* mTitleWidget;
    QHBoxLayout* mTitleLayout;
    ColorButton* mColorBtn;
    QLineEdit* mTitleEdit;
    QLineEdit* mLocationEdit;
    QTextEdit* mDescriptionEdit;
};



#endif // EVENTDIALOG_H

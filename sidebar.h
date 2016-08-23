#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QListWidget>

class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = 0);

signals:

public slots:

private:
    QTextBrowser* mTextBrowser;
    QListWidget* mList;
};

#endif // SIDEBAR_H

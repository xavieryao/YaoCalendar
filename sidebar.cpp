#include "sidebar.h"

SideBar::SideBar(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* rootLayout = new QVBoxLayout(parent);

    mTextBrowser = new QTextBrowser(this);
    mList = new QListWidget(this);
    rootLayout->addWidget(mTextBrowser);
    rootLayout->addWidget(mList);

    this->setLayout(rootLayout);
}

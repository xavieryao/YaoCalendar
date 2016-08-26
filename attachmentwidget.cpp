#include "attachmentwidget.h"
#include <QVBoxLayout>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QIcon>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QDebug>
#include <QUrl>

AttachmentWidget::AttachmentWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->setLayout(layout);
    mIcon = new QLabel(this);
    mAttachment = new QLabel(this);
    layout->addWidget(mIcon);
    layout->addWidget(mAttachment);
}

void AttachmentWidget::setFile(const QString filename) {
    mFile = filename;
    if (filename.isEmpty()) {
        mIcon->setVisible(false);
        mAttachment->setVisible(false);
    } else {
        QFileInfo info(filename);
        QFileIconProvider ip;
        QIcon icon = ip.icon(info);
        mIconPixmap = icon.pixmap(30, 30);
        mIcon->setPixmap(mIconPixmap);
        mAttachment->setText(info.fileName());
        mIcon->setVisible(true);
        mAttachment->setVisible(true);
    }
}

void AttachmentWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        qDebug() << "drag!";
        QDrag *drag = new QDrag(this);
        QMimeData* mimeData = new QMimeData;
        QList<QUrl> urls;
        QUrl url  = QUrl::fromLocalFile(mFile);
        urls.append(url);
        mimeData->setUrls(urls);
        mimeData->setText(mAttachment->text());

        drag->setMimeData(mimeData);
        drag->setPixmap(mIconPixmap);

        drag->exec();
    }
}

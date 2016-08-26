#ifndef ATTACHMENTWIDGET_H
#define ATTACHMENTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class AttachmentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AttachmentWidget(QWidget *parent = 0);
    void setFile(const QString filename);

protected:
    void mousePressEvent(QMouseEvent* event);

signals:

public slots:

private:
    QLabel* mIcon;
    QLabel* mAttachment;
    QString mFile;
    QPixmap mIconPixmap;
};

#endif // ATTACHMENTWIDGET_H

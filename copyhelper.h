#ifndef COPYHELPER_H
#define COPYHELPER_H

#include <QString>
#include "wavewidget.h"

class CopyHelper : public QObject
{
    Q_OBJECT
public:
    CopyHelper(QObject* parent = 0);
    QString store(const QString& source, const QString& fileName);
    void get(const QString& dest, const QString& from);

private slots:

    void progressed(long, qint64);
    void done();

private:
    QString randStr(int digits = 6);
    qint64 full;
    WaveWidget* wave;
};

#endif // COPYHELPER_H

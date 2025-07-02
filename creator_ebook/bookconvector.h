#ifndef BOOKCONVECTOR_H
#define BOOKCONVECTOR_H

#include <QObject>

class BookConvector : public QObject
{
    Q_OBJECT
public:
    explicit BookConvector(QObject *parent = nullptr);


public slots:
    void conver(QString filePath);

signals:
    void alert(const QString & title, const QString & text, QString type);

// signals:
};

#endif // BOOKCONVECTOR_H

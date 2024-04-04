#ifndef PDFHANDLE_H
#define PDFHANDLE_H

#include <QThread>
#include <QFile>
#include <QImage>
#include <poppler/qt5/poppler-qt5.h>


class Filehandle :public QObject
{
    Q_OBJECT
public:
    explicit Filehandle();

signals:
    void sendpdf(QImage image,QString text);
    void sendText(QString line);
    void showreadwidget();

public slots:
    void handlefile(const QString& filepath,const QString& suffix);

private:
    QFile* filehandler = nullptr;
    Poppler::Document* pdfhandler = nullptr;
};

#endif // PDFHANDLE_H

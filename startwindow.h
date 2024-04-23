#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include "camerawidget.h"
#include "readwidget.h"
#include "bookwidget.h"
#include "filehandle.h"
#include "http.h"
#include <QSet>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QBuffer>
#include <QTextCodec>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>



QT_BEGIN_NAMESPACE
namespace Ui { class StartWindow; }
QT_END_NAMESPACE

struct BookInfo
{
    QString book_name;
    QString book_path;
    QString suffix;     //hou zhui ming
};

enum Modle{
    NORMAL_MODLE,
    BOOK_MODLE
};

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();
    QString getaccesstoken(const QString&,const QString&);
    void Speech_result(const QList<QString>&);
    QByteArray imageBaseTo64ToUrlEncode(QString imagePth);
    void getaccess_token();

signals:
    void sendinfo(const QString& path,const QString& suffix);
    void sendimages(QList<QImage>);
    void sendtexts(QList<QString>);

public slots:

    void on_CaptureButton_clicked();
    void on_AddButton_clicked();
    void on_exitbutton_clicked();
    void closecamerawidget();
    void changetoreadmode();
    void chengetobookmode();
    void getpdf(QImage image,QString text);
    void getText(QString line);
    void showreadwindow();
    void exitreadwidget();
    void exitbookwidget();
    void OCR_request();
    void Speech_request();
    void on_oepnButton_clicked();

private slots:
    void on_playerButton_clicked();

private:
    Ui::StartWindow *ui;
    ReadWidget* normalreadmode = nullptr;
    Camerawidget* camera = nullptr;
    BookWidget* bookreadmode = nullptr;
    Filehandle* filehandle = nullptr;
    QString current_suffix;
    QList<BookInfo> bookinfolist;
    QSet<QString> fileKeys;
    QList<QImage> pdfimages;
    QList<QString> pdftexts;
    QList<QString> texts;
    QString OCR_access_token;
    QString Speech_access_token;
    QMediaPlayer* player = nullptr;
    QString current_file;
    Modle modle;
};
#endif // STARTWINDOW_H

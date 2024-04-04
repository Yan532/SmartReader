#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include "camerawidget.h"
#include "readwidget.h"
#include "bookwidget.h"
#include "filehandle.h"
#include <QSet>




QT_BEGIN_NAMESPACE
namespace Ui { class StartWindow; }
QT_END_NAMESPACE

struct BookInfo
{
    QString book_name;
    QString book_path;
    QString suffix;     //hou zhui ming
};

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

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
};
#endif // STARTWINDOW_H

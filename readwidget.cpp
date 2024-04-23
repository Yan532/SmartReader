#include "readwidget.h"
#include "ui_readwidget.h"
#include <QLabel>
#include <QTextBrowser>
#include <QTextDocument>
#include <QDebug>

ReadWidget::ReadWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReadWidget)
{
    ui->setupUi(this);
}

ReadWidget::~ReadWidget()
{
    delete ui;
}

void ReadWidget::on_changeButton_clicked()
{
    emit changebookmode();
}

void ReadWidget::on_backButton_clicked()
{
    emit exitread();
}

void ReadWidget::createpdfpages(QList<QImage> images)
{
    int count = 0;
    for (const QImage& image : images) {
        QWidget* page = new QWidget(ui->scrollAreaWidgetContents);

        page->setStyleSheet("background-color: white;");
        page->setGeometry(135,20+count*(650+20),500,650);

        QLabel* imageview = new QLabel(page);
        imageview->setStyleSheet("background-color: white;");
        imageview->setGeometry(0,0,page->width(),page->height());
        imageview->setAlignment(Qt::AlignCenter);

        QPixmap pixmap = QPixmap::fromImage(image);
        QPixmap scaledPixmap = pixmap.scaled(imageview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        count++;
        imageview->setPixmap(scaledPixmap);
        if(20+count*(650+20) >= ui->scrollAreaWidgetContents->height()){
            ui->scrollAreaWidgetContents->setMinimumHeight(20+count*(650+20));
        }
    }
}

void ReadWidget::createfilepages(QList<QString> lines)
{
    int count = 0;
    int Current_height = 0;
    QFrame *frame = new QFrame(ui->scrollAreaWidgetContents);
    QTextBrowser *textBrowser = new QTextBrowser(frame);
    frame->setStyleSheet("background-color: white;");
    frame->setGeometry(135,20+count*(650+20),500,650);
    textBrowser->setGeometry(25,25,frame->width()-50,frame->height()-50);
    QFont font("Arial", 12, QFont::Normal);
    textBrowser->setFont(font);
    int MaxHeight = 390;
    count++;

    for (const QString &line : lines) {
        textBrowser->append(line);
        Current_height += textBrowser->document()->size().height();
        qDebug() << textBrowser->document()->size().height();
        if (MaxHeight <= Current_height){

            frame = new QFrame(ui->scrollAreaWidgetContents);
            textBrowser = new QTextBrowser(frame);
            frame->setStyleSheet("background-color: white;");
            frame->setGeometry(135,20+count*(650+20),500,650);
            textBrowser->setGeometry(25,25,frame->width()-50,frame->height()-50);
            QFont font("Arial", 12, QFont::Normal);
            textBrowser->setFont(font);
            count++;
            Current_height = 0;
            if(20+count*(650+20) >= ui->scrollAreaWidgetContents->height()){
                ui->scrollAreaWidgetContents->setMinimumHeight(20+count*(650+20));
            }
        }
    }

}

void ReadWidget::on_listenButton_clicked()
{
    emit send_speech();
}

#include "bookwidget.h"
#include "ui_bookwidget.h"
#include <QMessageBox>
#include <QGraphicsScene>
#include <QTextBrowser>
#include <QTextDocument>
#include <QDebug>
#include <QLabel>

int pagecount = 0;

BookWidget::BookWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookWidget)
{
    ui->setupUi(this);
    ui->frame->hide();

    ui->stackedWidget->removeWidget(ui->page);
    ui->stackedWidget->removeWidget(ui->page_2);
//    QWidget *page = new QWidget;
//    QString color = QColor(Qt::red).name(); // 生成不同颜色
//    page->setStyleSheet("background-color: " + color + ";");
//    ui->stackedWidget->addWidget(page);
//    page = new QWidget;
//    color = QColor(Qt::green).name(); // 生成不同颜色
//    page->setStyleSheet("background-color: " + color + ";");
//    ui->stackedWidget->addWidget(page);
//    page = new QWidget;
//    color = QColor(Qt::white).name(); // 生成不同颜色
//    page->setStyleSheet("background-color: " + color + ";");
//    ui->stackedWidget->addWidget(page);
}

BookWidget::~BookWidget()
{
    delete ui;
}

void BookWidget::on_normalButton_clicked()
{
    emit changereadmode();
}

void BookWidget::on_menuButton_clicked()
{
    if(!HideOrShow){
      ui->frame->show();
      HideOrShow = true;
    }
    else{
      ui->frame->hide();
      HideOrShow = false;
    }

}

void BookWidget::on_BackButton_clicked()
{
    emit exitbook();
}

void BookWidget::on_nextButton_clicked()
{
    int currentIndex = ui->stackedWidget->currentIndex();
    if(currentIndex < ui->stackedWidget->count()){
        if(currentIndex + 1 == ui->stackedWidget->count()){
            QMessageBox msg(QMessageBox::Warning,"warning","this is last page!!!");
            msg.exec();
        }else{
            ui->stackedWidget->setCurrentIndex(currentIndex + 1);
        }
    }
}

void BookWidget::on_preButton_clicked()
{
    int currentIndex = ui->stackedWidget->currentIndex();
    if(currentIndex >= 0){
        if(currentIndex == 0){
            QMessageBox msg(QMessageBox::Warning,"warning","this is first page!!!");
            msg.exec();
        }else{
            ui->stackedWidget->setCurrentIndex(currentIndex - 1);
        }
    }
}

void BookWidget::createpdfpages(QList<QImage> images){
    for (const QImage& image : images) {
        QWidget* page = new QWidget;
        page->setStyleSheet("background-color: black;");
        page->setGeometry(0,0,ui->stackedWidget->width(),ui->stackedWidget->height());
        ui->stackedWidget->addWidget(page);

        QLabel* imageview = new QLabel(page);
        imageview->setStyleSheet("background-color: white;");
        imageview->setGeometry(page->width()/4,0,page->width()/2,page->height());
        imageview->setAlignment(Qt::AlignCenter);

        QPixmap pixmap = QPixmap::fromImage(image);
        QPixmap scaledPixmap = pixmap.scaled(imageview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        imageview->setPixmap(scaledPixmap);
    }
}

void BookWidget::createfilepages(QList<QString> lines){
    ui->menuButton->resize(ui->menuButton->width()/3,ui->menuButton->height());
    int Current_height = 0;
    QString texts;
    QWidget *page = new QWidget;
    ui->stackedWidget->addWidget(page);
    page->setStyleSheet("background-color: black;");
    page->setGeometry(0,0,ui->stackedWidget->width(),ui->stackedWidget->height());

    QFrame *frame = new QFrame(page);
    QTextBrowser *textBrowser = new QTextBrowser(frame);
    frame->setStyleSheet("background-color: white;");
    frame->setGeometry(page->width()/4,0,page->width()/2,page->height());
    textBrowser->setGeometry(25,25,frame->width()-50,frame->height()-50);
    QFont font("Arial", 12, QFont::Normal);
    textBrowser->setFont(font);
    int MaxHeight = 260;

    for (const QString &line : lines) {
        textBrowser->append(line);
        Current_height += textBrowser->document()->size().height();
        qDebug() << textBrowser->document()->size().height();
        if (MaxHeight <= Current_height){
            page = new QWidget;
            ui->stackedWidget->addWidget(page);
            page->setStyleSheet("background-color: black;");
            page->setGeometry(0,0,ui->stackedWidget->width(),ui->stackedWidget->height());

            frame = new QFrame(page);
            textBrowser = new QTextBrowser(frame);
            frame->setStyleSheet("background-color: white;");
            frame->setGeometry(page->width()/4,0,page->width()/2,page->height());
            textBrowser->setGeometry(25,25,frame->width()-50,frame->height()-50);
            QFont font("Arial", 12, QFont::Normal);
            textBrowser->setFont(font);
            Current_height = 0;
        }
    }
}

void BookWidget::playerstate(quint8 state)
{
    if(state == QMediaPlayer::PlayingState){
        ui->playerButton->setText("pause");
    }else{
        ui->playerButton->setText("play");
    }
}

void BookWidget::on_listenButton_clicked()
{
    emit book_listen();
}

void BookWidget::on_playerButton_clicked()
{
    if(ui->playerButton->text() == "play"){
        ui->playerButton->setText("pause");
    }else{
        ui->playerButton->setText("play");
    }
    emit change();
}



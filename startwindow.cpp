#include "startwindow.h"
#include "ui_startwindow.h"
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>

int Column = 0;
int Row = 0;

StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StartWindow)
{
    ui->setupUi(this);

    camera = new Camerawidget();

    filehandle = new Filehandle;
    connect(this,&StartWindow::sendinfo,filehandle,&Filehandle::handlefile);
    connect(filehandle,&Filehandle::sendpdf,this,&StartWindow::getpdf);
    connect(filehandle,&Filehandle::sendText,this,&StartWindow::getText);
    connect(filehandle,&Filehandle::showreadwidget,this,&StartWindow::showreadwindow);

}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::on_CaptureButton_clicked()
{

    connect(camera,&Camerawidget::exitcamera,this,&StartWindow::closecamerawidget);
    camera->showFullScreen();

}

void StartWindow::on_AddButton_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,tr("add new file"),
                                                    "/opt",
                                                    tr("Open File (*.pdf *.txt *.cpp)"));

    if(!filepath.isEmpty()){
        QFileInfo fileinfo(filepath);
        QString filekey = fileinfo.canonicalFilePath();
        if(fileKeys.contains(filekey)){
            QMessageBox msgBox(QMessageBox::Warning,"file reapeat","Please choose other file!!!");
            msgBox.exec();
        }
        else{
            BookInfo bookinfo;
            bookinfo.book_name = fileinfo.fileName();
            bookinfo.book_path = fileinfo.filePath();
            bookinfo.suffix = fileinfo.suffix().toLower();
            fileKeys.insert(filekey);
            bookinfolist.append(bookinfo);
            QPushButton *fileButton = new QPushButton(bookinfo.book_name,ui->scrollAreaWidgetContents);
            if(Column == 0){
                fileButton->setGeometry(20+Column*(150+50),20+Row*(180+30),150,180);
                Column++;
            }
            else{
                if(ui->scrollAreaWidgetContents->width() >= 20+Column*(150+50)){
                    fileButton->setGeometry(20+Column*(150+50),20+Row*(180+30),150,180);
                    Column++;
                }
                else{
                    Row++;
                    Column = 0;
                    if(ui->scrollAreaWidgetContents->height() > 20+Row*(180+30)+180){
                        fileButton->setGeometry(20+Column*(150+50),20+Row*(180+30),150,180);
                        Column++;
                    }
                    else{
                        ui->scrollAreaWidgetContents->setMinimumHeight(20+Row*(180+30)+180);
                        fileButton->setGeometry(20+Column*(150+50),20+Row*(180+30),150,180);
                        Column++;
                    }
                }
            }
            fileButton->show();
            connect(fileButton, &QPushButton::clicked, this, [this, bookinfo]() {
                current_suffix = bookinfo.suffix;
                pdfimages.clear();
                pdftexts.clear();
                texts.clear();
                delete bookreadmode;
                delete normalreadmode;
                emit sendinfo(bookinfo.book_path,bookinfo.suffix);
            });
        }

    }
}

void StartWindow::on_exitbutton_clicked()
{
    this->close();
    QApplication::quit();
}

void StartWindow::closecamerawidget(){
    disconnect(camera,&Camerawidget::exitcamera,this,&StartWindow::closecamerawidget);
    camera->hide();
}

void StartWindow::changetoreadmode()
{
    bookreadmode->close();
    disconnect(bookreadmode,&BookWidget::changereadmode,this,&StartWindow::changetoreadmode);
    disconnect(bookreadmode,&BookWidget::exitbook,this,&StartWindow::exitbookwidget);
    if(current_suffix == "pdf"){
        disconnect(this,&StartWindow::sendimages,bookreadmode,&BookWidget::createpdfpages);
    }else{
        disconnect(this,&StartWindow::sendtexts,bookreadmode,&BookWidget::createfilepages);
    }
    delete bookreadmode;
    bookreadmode=nullptr;

    normalreadmode = new ReadWidget;
    connect(normalreadmode,&ReadWidget::changebookmode,this,&StartWindow::chengetobookmode);
    connect(normalreadmode,&ReadWidget::exitread,this,&StartWindow::exitreadwidget);
    if(current_suffix == "pdf"){
        connect(this,&StartWindow::sendimages,normalreadmode,&ReadWidget::createpdfpages);
        emit sendimages(this->pdfimages);
    }else{
        connect(this,&StartWindow::sendtexts,normalreadmode,&ReadWidget::createfilepages);
        emit sendtexts(this->texts);
    }
    normalreadmode->showFullScreen();
}

void StartWindow::chengetobookmode()
{
    normalreadmode->close();
    disconnect(normalreadmode,&ReadWidget::changebookmode,this,&StartWindow::chengetobookmode);
    disconnect(normalreadmode,&ReadWidget::exitread,this,&StartWindow::exitreadwidget);
    if(current_suffix == "pdf"){
        disconnect(this,&StartWindow::sendimages,normalreadmode,&ReadWidget::createpdfpages);
    }else{
        disconnect(this,&StartWindow::sendtexts,normalreadmode,&ReadWidget::createfilepages);
    }
//    QMessageBox msg(QMessageBox::Warning,"test","test change");
//    msg.exec();
    delete normalreadmode;
    normalreadmode=nullptr;

    bookreadmode = new BookWidget;
    connect(bookreadmode,&BookWidget::changereadmode,this,&StartWindow::changetoreadmode);
    connect(bookreadmode,&BookWidget::exitbook,this,&StartWindow::exitbookwidget);
    if(current_suffix == "pdf"){
        connect(this,&StartWindow::sendimages,bookreadmode,&BookWidget::createpdfpages);
        emit sendimages(this->pdfimages);
    }
    else{
        connect(this,&StartWindow::sendtexts,bookreadmode,&BookWidget::createfilepages);
        emit sendtexts(this->texts);
    }
    bookreadmode->showFullScreen();
}

void StartWindow::getText(QString line){
    texts.append(line);
}

void StartWindow::getpdf(QImage image,QString text){
    pdfimages.append(image);
    pdftexts.append(text);
}

void StartWindow::showreadwindow(){
    normalreadmode = new ReadWidget;
    connect(normalreadmode,&ReadWidget::changebookmode,this,&StartWindow::chengetobookmode);
    connect(normalreadmode,&ReadWidget::exitread,this,&StartWindow::exitreadwidget);
    if(current_suffix == "pdf"){
        connect(this,&StartWindow::sendimages,normalreadmode,&ReadWidget::createpdfpages);
        emit sendimages(this->pdfimages);
    }else{
        connect(this,&StartWindow::sendtexts,normalreadmode,&ReadWidget::createfilepages);
        emit sendtexts(this->texts);
    }
    normalreadmode->showFullScreen();
}

void StartWindow::exitbookwidget(){
    bookreadmode->close();
    disconnect(bookreadmode,&BookWidget::changereadmode,this,&StartWindow::changetoreadmode);
    disconnect(bookreadmode,&BookWidget::exitbook,this,&StartWindow::exitbookwidget);
    if(current_suffix == "pdf"){
        disconnect(this,&StartWindow::sendimages,bookreadmode,&BookWidget::createpdfpages);
    }else{
        disconnect(this,&StartWindow::sendtexts,bookreadmode,&BookWidget::createfilepages);
    }
    delete bookreadmode;
    bookreadmode = nullptr;
}

void StartWindow::exitreadwidget(){
    normalreadmode->close();
    disconnect(normalreadmode,&ReadWidget::changebookmode,this,&StartWindow::chengetobookmode);
    disconnect(normalreadmode,&ReadWidget::exitread,this,&StartWindow::exitreadwidget);
    if(current_suffix == "pdf"){
        disconnect(this,&StartWindow::sendimages,normalreadmode,&ReadWidget::createpdfpages);
    }else{
        disconnect(this,&StartWindow::sendtexts,normalreadmode,&ReadWidget::createfilepages);
    }
    delete normalreadmode;
    normalreadmode = nullptr;
}

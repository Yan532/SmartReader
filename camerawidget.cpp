#include "camerawidget.h"
#include "ui_camerawidget.h"
#include <QMessageBox>
#include <QDebug>
#include <QApplication>

Camerawidget::Camerawidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Camerawidget)
{
    ui->setupUi(this);
    camera = new V4l2Api;
    ui->captureButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->sendButton->setEnabled(false);

}

Camerawidget::~Camerawidget()
{
    delete camera;
    camera = nullptr;
    delete ui;
}

void Camerawidget::recvImage(QImage image)
{
    Saveimage = image;
    QPixmap mmp = QPixmap::fromImage(image);
    ui->screenlabel->setPixmap(mmp);
}

void Camerawidget::on_captureButton_clicked()
{
    if(cameraTorF == false)
    {
         QMessageBox::warning(this, "警告", "请先打开摄像头");
         return ;
    }
    picTorF = true;
    ui->sendButton->setEnabled(true);
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->captureButton->setEnabled(false);
    disconnect(camera, &V4l2Api::sendImage, this, &Camerawidget::recvImage);
    cameraTorF = false;

    Saveimage.save("./pic.jpg","JPG",100);
    ui->screenlabel->setPixmap(QPixmap::fromImage(Saveimage));
}

void Camerawidget::on_startButton_clicked()
{
    cameraTorF = true;
    ui->stopButton->setEnabled(true);
    ui->captureButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    connect(camera, &V4l2Api::sendImage, this, &Camerawidget::recvImage);
    camera->start();
}

void Camerawidget::on_sendButton_clicked()
{
    emit OCR();
}

void Camerawidget::on_stopButton_clicked()
{
    cameraTorF = false;
    ui->stopButton->setEnabled(false);
    ui->captureButton->setEnabled(false);
    ui->startButton->setEnabled(true);
    disconnect(camera, &V4l2Api::sendImage, this, &Camerawidget::recvImage);
    camera->quit();
    ui->screenlabel->clear();
}

void Camerawidget::on_exitButton_clicked()
{
    disconnect(camera, &V4l2Api::sendImage, this, &Camerawidget::recvImage);
    camera->quit();
    ui->screenlabel->clear();
    emit exitcamera();
}

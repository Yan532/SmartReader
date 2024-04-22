#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include "v4l2api.h"
#include <QImage>

namespace Ui {
class Camerawidget;
}

class Camerawidget : public QWidget
{
    Q_OBJECT

public:
    explicit Camerawidget(QWidget *parent = nullptr);
    ~Camerawidget();

signals:
    void exitcamera();
    void OCR();

public slots:
    void on_captureButton_clicked();
    void on_startButton_clicked();
    void on_sendButton_clicked();
    void on_stopButton_clicked();
    void on_exitButton_clicked();
    void recvImage(QImage image);

private:
    Ui::Camerawidget *ui;
    V4l2Api* camera = nullptr;
    QImage Saveimage;
    bool cameraTorF = false;
    bool picTorF = false;
    bool Keypadding = false;
};

#endif // CAMERAWIDGET_H

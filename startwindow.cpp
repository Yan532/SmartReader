#include "startwindow.h"
#include "ui_startwindow.h"
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>
#include <QHostInfo>
#include <QProcess>

/* OCR Url */
const QString OCR_Url = "https://aip.baidubce.com/rest/2.0/ocr/v1/general_basic?access_token=%1";
const QString OCRAPI_key = "wSweAjPU2LiAlHqr9rhF8qt8";
const QString OCRAPI_Secret = "NYePOhZOts5nNxqxKtRQ86ZT5A5U0DwO";

/* Speech Url */
const QString speech_Url = "https://tsn.baidu.com/text2audio";
const QString speechAPI_key = "NwV0ZDXZ2GHlj0tQXYNKYKv9";
const QString speech_Secret = "yrnJncB6m2NJFCBQrFl7RoXJRKyZjbM4";

/* Access_token Url */
const QString access_tokenUrl = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2";

int Column = 0;
int Row = 0;

StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    ui->playerButton->setEnabled(false);
    ui->oepnButton->setEnabled(false);
    player = new QMediaPlayer;

    getaccess_token();
    QProcess *m_process = new QProcess;
    QStringList arguments;
    arguments << "-c" << "0" << "cset" << "numid=2" << "on";
    m_process->start("amixer",arguments);
    m_process->waitForFinished();
    arguments.clear();
    m_process->close();
    arguments << "-c" << "0" << "cset" << "numid=1" << "100%";
    m_process->start("amixer",arguments);
    m_process->waitForFinished();
    arguments.clear();
    m_process->close();
    arguments << "-c" << "0" << "cset" << "numid=3" << "100%";
    m_process->start("amixer",arguments);
    m_process->waitForFinished();
    arguments.clear();
    m_process->close();
    arguments << "-c" << "0" << "cset" << "numid=7" << "off";
    m_process->start("amixer",arguments);
    m_process->waitForFinished();
    arguments.clear();
    m_process->close();
    arguments << "-c" << "0" << "cset" << "numid=15" << "1";
    m_process->start("amixer",arguments);
    m_process->waitForFinished();
    arguments.clear();
    m_process->close();
    arguments << "-c" << "0" << "cset" << "numid=16" << "2";
    m_process->start("amixer",arguments);
    m_process->waitForFinished();
    arguments.clear();
    m_process->close();
    arguments << "-c" << "0" << "cset" << "numid=17" << "2";
    m_process->start("amixer",arguments);
    m_process->waitForFinished();
    arguments.clear();
    m_process->close();

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
    connect(camera,&Camerawidget::OCR,this,&StartWindow::OCR_request);
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
            QPushButton *fileButton = new QPushButton(ui->scrollAreaWidgetContents);
            fileButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            fileButton->setText(bookinfo.book_name);
            fileButton->setStyleSheet("QPushButton{text-overflow: ellipsis;}");
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
                if(current_file == bookinfo.book_name){
                    QMessageBox msgBox(QMessageBox::Warning,"open the same file","Please choose other file to open or click open window button!!!");
                    msgBox.exec();
                }else{
                    current_file = bookinfo.book_name;
                    current_suffix = bookinfo.suffix;
                    isOCR_result = false;
                    if(!player){
                        player->stop();
                        ui->playerButton->setText("播放");
                        disconnect(player,&QMediaPlayer::stateChanged,this,&StartWindow::over_state);
                        delete player;
                    }
                    pdfimages.clear();
                    texts.clear();
                    ui->playerButton->setEnabled(false);
                    ui->oepnButton->setEnabled(true);
                    delete bookreadmode;
                    delete normalreadmode;
                    emit sendinfo(bookinfo.book_path,bookinfo.suffix);
                }

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
    disconnect(bookreadmode,&BookWidget::book_listen,this,&StartWindow::Speech_request);
    disconnect(bookreadmode,&BookWidget::change,this,&StartWindow::statechange);
    disconnect(this,&StartWindow::send_state,bookreadmode,&BookWidget::playerstate);
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
    connect(normalreadmode,&ReadWidget::send_speech,this,&StartWindow::Speech_request);
    connect(normalreadmode,&ReadWidget::change,this,&StartWindow::statechange);
    connect(this,&StartWindow::send_state,normalreadmode,&ReadWidget::playerstate);
    if(current_suffix == "pdf"){
        connect(this,&StartWindow::sendimages,normalreadmode,&ReadWidget::createpdfpages);
        emit sendimages(this->pdfimages);
    }else{
        connect(this,&StartWindow::sendtexts,normalreadmode,&ReadWidget::createfilepages);
        emit sendtexts(this->texts);
    }
    modle = NORMAL_MODLE;
    emit send_state(media_state);
    normalreadmode->showFullScreen();
}

void StartWindow::chengetobookmode()
{
    normalreadmode->close();
    disconnect(normalreadmode,&ReadWidget::changebookmode,this,&StartWindow::chengetobookmode);
    disconnect(normalreadmode,&ReadWidget::exitread,this,&StartWindow::exitreadwidget);
    disconnect(normalreadmode,&ReadWidget::send_speech,this,&StartWindow::Speech_request);
    disconnect(normalreadmode,&ReadWidget::change,this,&StartWindow::statechange);
    disconnect(this,&StartWindow::send_state,normalreadmode,&ReadWidget::playerstate);
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
    connect(bookreadmode,&BookWidget::book_listen,this,&StartWindow::Speech_request);
    connect(bookreadmode,&BookWidget::change,this,&StartWindow::statechange);
    connect(this,&StartWindow::send_state,bookreadmode,&BookWidget::playerstate);
    if(current_suffix == "pdf"){
        connect(this,&StartWindow::sendimages,bookreadmode,&BookWidget::createpdfpages);
        emit sendimages(this->pdfimages);
    }
    else{
        connect(this,&StartWindow::sendtexts,bookreadmode,&BookWidget::createfilepages);
        emit sendtexts(this->texts);
    }
    modle = BOOK_MODLE;
    emit send_state(media_state);
    bookreadmode->showFullScreen();
}

void StartWindow::getText(QString line){
    texts.append(line);
}

void StartWindow::getpdf(QImage image,QString text){
    pdfimages.append(image);
    texts.append(text);
}

void StartWindow::showreadwindow(){
    normalreadmode = new ReadWidget;
    connect(normalreadmode,&ReadWidget::changebookmode,this,&StartWindow::chengetobookmode);
    connect(normalreadmode,&ReadWidget::exitread,this,&StartWindow::exitreadwidget);
    connect(normalreadmode,&ReadWidget::send_speech,this,&StartWindow::Speech_request);
    connect(normalreadmode,&ReadWidget::change,this,&StartWindow::statechange);
    connect(this,&StartWindow::send_state,normalreadmode,&ReadWidget::playerstate);
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
    bookreadmode->hide();
    modle = BOOK_MODLE;
}

void StartWindow::exitreadwidget(){
    normalreadmode->hide();
    modle = NORMAL_MODLE;
}

void StartWindow::Speech_request(){
    Speech_result(texts);
}

void StartWindow::OCR_request(){
    current_file = "";
    pdfimages.clear();
    texts.clear();
    ui->playerButton->setEnabled(false);
    ui->oepnButton->setEnabled(true);
    delete bookreadmode;
    delete normalreadmode;
    QString imgUrl = QString(OCR_Url).arg(OCR_access_token);
    qDebug() << imgUrl;
    QByteArray img = imageBaseTo64ToUrlEncode("/root/test.jpg");    //image=xxxxxxx
//    qDebug() << img;
    QByteArray imgData = "image=" + img;               //body
    QByteArray replyData;                 //保存回复信息
    QMap<QString, QString> header;        //封装头部信息
    header.insert(QString("Content-Type"), QString("application/x-www-form-urlencoded"));
    bool result = Http::post_sync(imgUrl, header, imgData, replyData);
//    qDebug() << replyData;
    if(result){
    qDebug() << "Request sent successfully!";
    ui->oepnButton->setEnabled(true);
    // 解析JSON响应
    QJsonDocument jsonResponse = QJsonDocument::fromJson(replyData);
    QJsonObject jsonObject = jsonResponse.object();
    if (jsonObject.contains("words_result")) {
       QJsonArray wordsArray = jsonObject.value("words_result").toArray();
       foreach (const QJsonValue &value, wordsArray) {
           texts.append(value.toObject().value("words").toString());
           qDebug() << "识别结果:" << value.toObject().value("words").toString();
       }
       isOCR_result = true;
       current_suffix = "txt";
       modle = NORMAL_MODLE;
       isfirst = true;
    }
  }
}

void StartWindow::Speech_result(const QList<QString>& texts){
        QString Texts;
        if(texts.length() > 5){
            for (int i = 0; i < 5; ++i) {
                Texts.append(texts[i]);
            }
        }else{
            for (int i = 0; i < texts.length(); ++i) {
                Texts.append(texts[i]);
            }
        }
        QString encodedText = QUrl::toPercentEncoding(QUrl::toPercentEncoding(Texts));
        QByteArray postData;
        postData.append("tex="+encodedText.toUtf8());
        postData.append("&tok="+Speech_access_token.toUtf8());
        postData.append("&cuid="+QHostInfo::localHostName());
        postData.append("&ctp=1");
        postData.append("&lan=zh");
        postData.append("&vol=9");
        postData.append("&per=3");
        postData.append("&spd=5");
        postData.append("&aue=3");
        QByteArray audioData;                 //保存回复信息

        QMap<QString, QString> header;        //封装头部信息
        header.insert(QString("Content-Type"), QString("application/x-www-form-urlencoded"));
        bool result = Http::post_sync(speech_Url, header,postData,audioData);
//        qDebug() << audioData;
        if(result){
            qDebug() << "Request sent successfully!";
            // 保存音频文件
            QFile file("./output.mp3");
            if (file.open(QIODevice::WriteOnly)) {
                file.write(audioData);
                file.close();
                player = new QMediaPlayer;
                ui->playerButton->setEnabled(true);
                ui->playerButton->setText("暂停");
                player->setMedia(QUrl::fromLocalFile("/opt/SmartReader/bin/output.mp3"));
                player->play();
                media_state = QMediaPlayer::PlayingState;
                emit send_state(media_state);
                connect(player,&QMediaPlayer::stateChanged,this,&StartWindow::over_state);
                qDebug() << "Audio file saved as output.mp3";
            } else {
                qDebug() << "Failed to save audio file";
            }
        }
}


QByteArray StartWindow::imageBaseTo64ToUrlEncode(QString imagePth)
{
       QImage image(imagePth);
       QByteArray byte;
       //用QByteArray构造QBuffer
       QBuffer buf(&byte);
       buf.open(QIODevice::WriteOnly);
       image.save(&buf,"JPG");
       //对图片做base64编码(不包含编码头)
       QByteArray byteBase64 = byte.toBase64();
       QTextCodec *codec = QTextCodec::codecForName("UTF-8");
       QByteArray imgData = codec->fromUnicode(byteBase64).toPercentEncoding();
       return imgData;
}

void StartWindow::getaccess_token()
{
        QString OCR_tokenUrl = QString(access_tokenUrl).arg(OCRAPI_key).arg(OCRAPI_Secret);
        QString Speech_tokenUrl = QString(access_tokenUrl).arg(speechAPI_key).arg(speech_Secret);
        QMap<QString,QString> header;
        header.insert(QString("Content-Type"),QString("application/x-www-form-urlencoded"));
        QByteArray requestData;//请求内容
        QByteArray replyData;//url返回内容
        bool result = Http::post_sync(OCR_tokenUrl,header,requestData,replyData);
        if(result){
            QJsonDocument jsonResponse = QJsonDocument::fromJson(replyData);
            QJsonObject jsonObject = jsonResponse.object();
            if (jsonObject.contains("access_token")) {
                OCR_access_token = jsonObject.value("access_token").toString();
                qDebug() << "Access Token:" << OCR_access_token;
            } else {
                qDebug() << "Error: Access Token not found in response";
            }
        }
        replyData.clear();
        result = Http::post_sync(Speech_tokenUrl,header,requestData,replyData);
        if(result){
            QJsonDocument jsonResponse = QJsonDocument::fromJson(replyData);
            QJsonObject jsonObject = jsonResponse.object();
            if (jsonObject.contains("access_token")) {
                Speech_access_token = jsonObject.value("access_token").toString();
                qDebug() << "Access Token:" << Speech_access_token;
            } else {
                qDebug() << "Error: Access Token not found in response";
            }
        }
}

void StartWindow::on_oepnButton_clicked()
{
    switch (modle) {
    case BOOK_MODLE:
        bookreadmode->show();
        emit send_state(media_state);
        break;
    case NORMAL_MODLE:
        if(isOCR_result){
          if(isfirst){
            showreadwindow();
            isfirst = false;
          }else{
            normalreadmode->show();
            emit send_state(media_state);
          }
        }else{
          normalreadmode->show();
          emit send_state(media_state);
        }
        break;
    default:
        break;
    }
}

void StartWindow::statechange()
{
    if(player->state() == QMediaPlayer::PlayingState){
        player->pause();
        media_state = player->state();
        ui->playerButton->setText("播放");
    }else{
        player->play();
        media_state = player->state();
        ui->playerButton->setText("暂停");
    }
}

void StartWindow::over_state(QMediaPlayer::State newstate)
{
    if(newstate == QMediaPlayer::StoppedState){
        ui->playerButton->setText("播放");
        media_state = QMediaPlayer::PausedState;
        emit send_state(media_state);
    }
}

void StartWindow::on_playerButton_clicked()
{
    if(player->state() == QMediaPlayer::PlayingState){
        player->pause();
        media_state = player->state();
        emit send_state(media_state);
        ui->playerButton->setText("播放");
    }else{
        player->play();
        media_state = player->state();
        emit send_state(media_state);
        ui->playerButton->setText("暂停");
    }
}

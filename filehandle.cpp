#include "filehandle.h"
#include <QMessageBox>
#include <QTextStream>

Filehandle::Filehandle()
{

}

void Filehandle::handlefile(const QString &filepath, const QString &suffix)
{
    if(suffix == "pdf"){
        pdfhandler = Poppler::Document::load(filepath);
        if (!pdfhandler || pdfhandler->isLocked()) {
           QMessageBox msg(QMessageBox::Warning, "Error", "Failed to open or invalid PDF file");
           delete pdfhandler;
           pdfhandler = nullptr;
           msg.exec();
           return;
        }
//        QMessageBox msg(QMessageBox::Warning, "Open", "Open PDF");
//        msg.exec();
        for (int i = 0; i < pdfhandler->numPages(); ++i) {  
            QString text;
            Poppler::Page *page = pdfhandler->page(i);
            QList<Poppler::TextBox *>texts = page->textList();
            if(page == 0){
                QMessageBox msg(QMessageBox::Warning, "Error", "page Error");
                msg.exec();
                delete page;
                break;
            }
            if(texts.count() == 0){
                QMessageBox msg(QMessageBox::Warning, "Error", "text error");
                msg.exec();
            }
            for (int i = 0; i < texts.count(); ++i) {
                text.append(texts.at(i)->text());
            }
            if(text.isEmpty()){
                QMessageBox msg(QMessageBox::Warning, "Error", "text empty");
                msg.exec();
            }
//            QMessageBox msg(QMessageBox::Warning, "Error", text);
//            msg.exec();
            QImage image = page->renderToImage();
            if(image.isNull()){
                QMessageBox msg(QMessageBox::Warning, "Error", "iamge Error");
                msg.exec();
                delete page;
                break;
            }
            emit sendpdf(image,text);
            delete page;
      }
        delete pdfhandler;
        pdfhandler = nullptr;
    }
    else if(suffix == "txt" || suffix == "cpp"){
        filehandler = new QFile(filepath);
        if(!filehandler->open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox msg(QMessageBox::Warning, "Canot Open file", "Canot Open file");
            msg.exec();
            return ;
        }

        QTextStream in(filehandler);

        while (!in.atEnd()) {
          QString line = in.readLine().trimmed();
          if(!line.isEmpty()){
              emit sendText(line);
          }
        }

//        // 输出文件内容
//        qDebug() << "文件内容：" << fileContent;

        // 关闭文件
        filehandler->close();
        delete filehandler;
        filehandler = nullptr;
    }
    emit showreadwidget();
}

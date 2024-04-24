#ifndef READWIDGET_H
#define READWIDGET_H

#include <QWidget>
#include <QMediaPlayer>

namespace Ui {
class ReadWidget;
}

class ReadWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReadWidget(QWidget *parent = nullptr);
    ~ReadWidget();

signals:
    void changebookmode();
    void exitread();
    void send_speech();
    void change();

public slots:
    void on_changeButton_clicked();

public slots:
    void on_backButton_clicked();
    void createpdfpages(QList<QImage>);
    void createfilepages(QList<QString>);
    void playerstate(quint8);

private slots:
    void on_listenButton_clicked();

    void on_playerButton_clicked();

private:
    Ui::ReadWidget *ui;
};

#endif // READWIDGET_H

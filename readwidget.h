#ifndef READWIDGET_H
#define READWIDGET_H

#include <QWidget>

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

public slots:
    void on_changeButton_clicked();

public slots:
    void on_backButton_clicked();
    void createpdfpages(QList<QImage>);
    void createfilepages(QList<QString>);

private:
    Ui::ReadWidget *ui;
};

#endif // READWIDGET_H

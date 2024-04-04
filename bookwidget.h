#ifndef BOOKWIDGET_H
#define BOOKWIDGET_H

#include <QWidget>

namespace Ui {
class BookWidget;
}

class BookWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BookWidget(QWidget *parent = nullptr);
    ~BookWidget();

signals:
    void changereadmode();
    void exitbook();

public slots:
    void on_normalButton_clicked();

public slots:
    void on_menuButton_clicked();
    void on_BackButton_clicked();
    void on_nextButton_clicked();
    void on_preButton_clicked();
    void createpdfpages(QList<QImage>);
    void createfilepages(QList<QString>);

private:
    Ui::BookWidget *ui;
    bool HideOrShow = false;
};

#endif // BOOKWIDGET_H

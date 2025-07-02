#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bookconvector.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void conver(QString pathFile);

private slots:
    void on_action_ebook_triggered();
    void returnDialogMessage(const QString & title, const QString & text, QString type);

private:
    Ui::MainWindow *    ui;
    BookConvector *     _bc;
};

#endif // MAINWINDOW_H

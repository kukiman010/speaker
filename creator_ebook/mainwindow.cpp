#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _bc = new BookConvector();

    connect(_bc, &BookConvector::alert, this, &MainWindow::returnDialogMessage);
    connect( this, &MainWindow::conver , _bc, &BookConvector::conver);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_ebook_triggered()
{
    QFileDialog dialog(this,"", QDir::homePath(),"*.pdf *.epub (*.pdf *.epub)");
    dialog.setFileMode(QFileDialog::ExistingFile);


    QString pathFile;
    if (dialog.exec())
    {
        pathFile = dialog.selectedFiles().first();
        emit conver(pathFile);
    }

}

void MainWindow::returnDialogMessage(const QString & title, const QString & text, QString type)
{
    if(type == "warning")
        QMessageBox::warning(this, title, text);
    if(type == "error")
        QMessageBox::critical(this, title, text);
    else
        QMessageBox::information(this, title, text);
}

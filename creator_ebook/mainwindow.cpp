#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_ebook_triggered()
{
    QFileDialog dialog(this,"", QDir::homePath(),"*.pdf *.epub");
    dialog.setFileMode(QFileDialog::ExistingFile);

    dialog.show();

//    dialog.

    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();

}

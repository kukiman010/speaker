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

    settings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete _bc;
    delete _pageMain;
    delete _settings;
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
        openFile(pathFile);
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

void MainWindow::openFile(QString pathFile)
{
    lastUseFile(pathFile);
    _pageMain->loadLastFiles(lastUseFiles);
    emit conver(pathFile);
}

bool MainWindow::settings()
{
    _bc = new BookConvector();
    _pageMain = new Page_main();

    connect(_bc, &BookConvector::alert, this, &MainWindow::returnDialogMessage);
    connect( this, &MainWindow::conver , _bc, &BookConvector::conver);
    connect( _pageMain, &Page_main::createBook, this, &MainWindow::on_action_ebook_triggered);
    connect( _pageMain, &Page_main::createLatFile, this, &MainWindow::openFile);
    connect( _pageMain, &Page_main::alert, this, &MainWindow::returnDialogMessage);

    ui->stackedWidget->addWidget(_pageMain);


    QDir dir2(qApp->applicationDirPath());
    // dir2.cd("..");
    baseWay = dir2.filePath("") + "/";
    // qDebug() << baseWay;


    QDir dir(baseWay + "configure");
    if (!dir.exists())
        dir.mkdir(baseWay + "configure");

    _settings = new QSettings(baseWay +"configure/conf.ini", QSettings::IniFormat);


    if(fileExists(baseWay + "configure/conf.ini"))
        loadSettings();
    else
    {
        loadSettings();
        saveSettings();
    }

    _pageMain->loadLastFiles(lastUseFiles);

    return true;
}

void MainWindow::loadSettings()
{
    lastUseFiles = _settings->value("LastFiles", "").toStringList();
    settingLang = _settings->value("Language", "auto").toString();
    restoreGeometry(_settings->value("Geometry").toByteArray());
}

void MainWindow::saveSettings()
{
    _settings->setValue("LastFiles", lastUseFiles);
    _settings->setValue("Language", settingLang);
    _settings->setValue("Geometry", saveGeometry());
    _settings->sync();
}

bool MainWindow::fileExists(const QString &path)
{
    QFileInfo check_file(path);
    return check_file.exists() && check_file.isFile();
}

void MainWindow::lastUseFile(QString file)
{
    if(file.isEmpty())
    {
        returnDialogMessage("BookConvector::lastUseFile", tr("пустое имя файла"), "error");
        return;
    }

    int findItem = lastUseFiles.indexOf(file);
    if( findItem != -1)
        lastUseFiles.removeAt(findItem);

    int uu = (lastUseFiles.size() +1) - 10;
    for(int i=0; i<uu ;++i)
        lastUseFiles.pop_back();

    lastUseFiles.push_front(file);
}







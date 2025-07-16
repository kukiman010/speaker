#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "bookconvector.h"
#include "page_main.h"


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
    void                conver(QString pathFile);

private slots:
    void                on_action_ebook_triggered();
    void                returnDialogMessage(const QString & title, const QString & text, QString type);
    void                openFile(QString pathFile);

private:
    Ui::MainWindow *    ui = nullptr;
    BookConvector *     _bc = nullptr;
    Page_main *         _pageMain = nullptr;
    QSettings *         _settings = nullptr;
    QString             baseWay;
    QStringList         lastUseFiles;
    QString             settingLang;


    bool                settings();
    void                loadSettings();
    void                saveSettings();
    bool                fileExists(const QString &path);
    void                lastUseFile(QString file);
};

#endif // MAINWINDOW_H

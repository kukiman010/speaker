#ifndef PAGE_MAIN_H
#define PAGE_MAIN_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class Page_main;
}

class Page_main : public QWidget
{
    Q_OBJECT

public:
    explicit Page_main(QWidget *parent = nullptr);
    ~Page_main();
    void                    loadLastFiles(QStringList files);

signals:
    void                    createBook();
    void                    createLatFile(QString pathFile);
    void                    alert(const QString & title, const QString & text, QString type);


private slots:
    // void on_listWidget_clicked(const QModelIndex &index);
    void                    onItemClicked(QListWidgetItem *item);
    void                    on_pushButton_clicked();


private:
    Ui::Page_main *ui;
};

#endif // PAGE_MAIN_H

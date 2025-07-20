#include "page_main.h"
#include "ui_page_main.h"

#include <QDebug>

Page_main::Page_main(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Page_main)
{
    ui->setupUi(this);

    ui->label->setText(tr("Добро пожаловать в редактор книг!"));
    ui->label_2->setText(tr("Список ранее открытых файлов:"));

    connect(ui->listWidget, &QListWidget::itemClicked, this, &Page_main::onItemClicked);
}

Page_main::~Page_main()
{
    delete ui;
}

void Page_main::onItemClicked(QListWidgetItem *item)
{
    QString text = item->text();
    QVariant userData = item->data(Qt::UserRole);
    // qDebug() << "Клик по элементу:" << text << "данные:" << userData;
    emit createLatFile(text);
}


void Page_main::on_pushButton_clicked()
{
    emit createBook();
}

void Page_main::loadLastFiles(QStringList files)
{
    if(files.isEmpty())
        // emit alert()
        return;

    ui->listWidget->clear();

    int counter=1;
    for(QString & file: files)
    {
        if(file.isEmpty())
            continue;
        QListWidgetItem *item = new QListWidgetItem(file);
        item->setData(Qt::UserRole, QVariant::fromValue(counter));
        ui->listWidget->addItem(item);
        ++counter;
    }

}


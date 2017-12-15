#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "secondwindow.h"

ChatWindow::ChatWindow(QListWidgetItem *item, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setText(item->text());
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

// Кнопка назад, ведущая к поисковой странице
void ChatWindow::on_pushButton_clicked()
{
    SecondWindow * second_window = new SecondWindow(this);
    this->close();
    second_window->show();
}

#include "settings.h"
#include "ui_settings.h"
#include "iugram.h"

Settings::Settings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
}

Settings::~Settings()
{
    delete ui;
}

// Кнопка назад, которая ведет на страницу входа
void Settings::on_pushButton_clicked()
{
    IUGram * first_window = new IUGram(this);
    this->close();
    first_window->show();
}

// Кнопка сохранить настройки для хоста и порта
void Settings::on_pushButton_2_clicked()
{
    ip_address = ui->lineEdit->text();
    port = ui->lineEdit_2->text();
}

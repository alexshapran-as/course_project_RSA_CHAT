#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "chatwindow.h"
#include "settingswindow.h"
#include "secondwindow.h"
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>

int port;

SettingsWindow::SettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

// Кнопка назад, которая ведет к странице поиска пользователей
void SettingsWindow::on_pushButton_clicked()
{
    SecondWindow * second_window = new SecondWindow(this);
    this->close();
    second_window->showFullScreen();
}

// Кнопка ОК
void SettingsWindow::on_lineEdit_returnPressed()
{
    on_pushButton_2_clicked();
}

// Кнопка сохранить
void SettingsWindow::on_pushButton_2_clicked()
{
    if (ui->lineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, "Ошибка!", "Поле открытый порт должно быть заполнено.");
        return;
    }
    port = ui->lineEdit->text().toInt();
    // Менеджер сети для определения подключения к WIFI
    QNetworkConfigurationManager manager;
    // Проверка наличия соединения
    if (manager.isOnline())
    {
        ChatWindow * chat_window = new ChatWindow(this);
        this->close();
        chat_window->showFullScreen();
    }
    else
    {
        QMessageBox::critical(this, "Ошибка подключения!", "Для перехода к чату необходимо подключиться к сети WIFI.");
    }
}

#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "chatwindow.h"
#include "settingswindow.h"

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
    SettingsWindow * settings_window = new SettingsWindow(this);
    this->close();
    settings_window->showFullScreen();
}

// Кнопка ОК
void SettingsWindow::on_lineEdit_returnPressed()
{
    on_pushButton_2_clicked();
}

// Кнопка сохранить
void SettingsWindow::on_pushButton_2_clicked()
{
    port = ui->lineEdit->text().toInt();
    ChatWindow * chat_window = new ChatWindow(this);
    this->close();
    chat_window->showFullScreen();
}

#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "secondwindow.h"
#include <QMessageBox>
#include <QListWidget>
#include <QTimer>

QListWidgetItem * item_;
extern QString new_user_log;
// Определители вызовов
bool showed = false;
bool showed_appends = false;
// Счетчик вызовов
unsigned int count = 1;

ChatWindow::ChatWindow(QListWidgetItem *item, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    item_ = item;
    ui->lineEdit->setText(item->text());
    // Коннекты для определения нажатия на клавишу ОК, прихода нового сообщения, нового клиента, ушедшего клиента
    connect(ui->lineEdit_2, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(&client, SIGNAL(newMessage(QString,QString)), this, SLOT(appendMessage(QString,QString)));
    connect(&client, SIGNAL(newParticipant(QString)), this, SLOT(newParticipant(QString)));
    connect(&client, SIGNAL(participantLeft(QString)), this, SLOT(participantLeft(QString)));

    myNickName = client.nickName();
    newParticipant(myNickName);
    QTimer::singleShot(10 * 1000, this, SLOT(showInformation()));
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

// Кнопка назад, ведущая к поисковой странице
void ChatWindow::on_pushButton_clicked()
{
    participantLeft(myNickName);
    SecondWindow * second_window = new SecondWindow(this);
    this->close();
    second_window->show();
}

// Кнопка отправить сообщение
void ChatWindow::on_pushButton_3_clicked()
{
    returnPressed();
}

// Вывод сообщения у отправителя
void ChatWindow::appendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;
    // Вывод принятого сообщения, которое приходит именно от другого пользователя
    QString total_msg;
    total_msg = "<" + ui->lineEdit->text() + ">: " + message;
    if (!showed_appends && count > 1)
    {
        ui->listWidget->addItem(total_msg );
    }
    showed_appends = false;
    count++;
}

// Нажатие на кнопку ОК
void ChatWindow::returnPressed()
{
    QString text = ui->lineEdit_2->text();
    if (text.isEmpty())
    {
        return;
    }
    client.sendMessage(text);
    // Вывод отправленного сообщения, которое написал текущий пользователь
    ui->listWidget->addItem("<" + new_user_log + ">: " + text);
    ui->lineEdit_2->clear();
    showed_appends = true;
    count = 0;

}

// Подключение нового user-a
void ChatWindow::newParticipant(const QString &nick)
{
    if (nick.isEmpty())
    {
        return;
    }
    if (!showed)
    {
        ui->listWidget->addItem(new_user_log + ", вы были\n успешно подключены к чату!");
        showed = true;
    }
}

// User отключился от диалога
void ChatWindow::participantLeft(const QString &nick)
{
    if (nick.isEmpty())
    {
        ui->listWidget->addItem(new_user_log + " вышел из диалога!");
        return;
    }
    QList<QListWidgetItem *> items = ui->listWidget->findItems(nick, Qt::MatchExactly);
    if (items.isEmpty())
    {
        return;
    }
    delete items.at(0);
    ui->listWidget->addItem(new_user_log + " вышел из диалога!");
}

// Информационное окно
void ChatWindow::showInformation()
{
    if (ui->listWidget->count() > 1)
    {
        QMessageBox::information(this, "Чат", ui->lineEdit->text() + " уже в сети! Приятного общения:)");
    }
    else
    {
        QMessageBox::information(this, "Чат", ui->lineEdit->text() + " еще не в сети:(");
    }
}

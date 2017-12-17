#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "secondwindow.h"
#include <QMessageBox>
#include <QListWidget>
#include <QTimer>

quint16 port = 8081;
QListWidgetItem * item_;
extern QString new_user_log;

ChatWindow::ChatWindow(QListWidgetItem *item, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    item_ = item;
    ui->lineEdit->setText(item->text());
    connect(ui->lineEdit_2, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(ui->lineEdit_2, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(&client, SIGNAL(newMessage(QString,QString)),
            this, SLOT(appendMessage(QString,QString)));
    connect(&client, SIGNAL(newParticipant(QString)),
            this, SLOT(newParticipant(QString)));
    connect(&client, SIGNAL(participantLeft(QString)),
            this, SLOT(participantLeft(QString)));

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
    SecondWindow * second_window = new SecondWindow(this);
    this->close();
    second_window->show();
}

// Кнопка отправить сообщение
void ChatWindow::on_pushButton_3_clicked()
{
    returnPressed();
}

void ChatWindow::appendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;

   ui->listWidget->addItem(message);
}

void ChatWindow::returnPressed()
{
    QString text = ui->lineEdit_2->text();
    if (text.isEmpty())
        return;

    client.sendMessage(text);
    appendMessage(myNickName, text);


    ui->lineEdit_2->clear();
}

void ChatWindow::newParticipant(const QString &nick)
{
    if (nick.isEmpty())
        return;
    ui->listWidget->addItem(nick);
}

void ChatWindow::participantLeft(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QList<QListWidgetItem *> items = ui->listWidget->findItems(nick,
                                                           Qt::MatchExactly);
    if (items.isEmpty())
        return;

    delete items.at(0);
}

void ChatWindow::showInformation()
{
    if (ui->listWidget->count() == 1) {
        QMessageBox::information(this, tr("Chat"),
                                 tr("Launch several instances of this "
                                    "program on your local network and "
                                    "start chatting!"));
    }
}

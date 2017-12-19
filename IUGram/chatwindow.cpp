#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "secondwindow.h"
#include "rsa.h"
#include <QTimer>
#include <QTime>

extern QString new_user_log;
extern int port;
extern QListWidgetItem * item_;

//// Определители вызовов
//bool showed = false;
//bool showed_appends = false;
//// Счетчик вызовов
//unsigned int count = 1;


ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setText(item_->text());
    socket = nullptr;
    UdpChat(item_->text(), port);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

// Кнопка назад, ведущая к поисковой странице
void ChatWindow::on_pushButton_clicked()
{
    QMessageBox::information(this,"Чат","<" + ui->lineEdit->text() + ">: вышел из чата!");
    SecondWindow * second_window = new SecondWindow(this);
    this->close();
    second_window->showFullScreen();
}

// Кнопка отправить сообщение
void ChatWindow::on_pushButton_3_clicked()
{
    returnPressed();
}

// Открытие соединения и отправка первого сообщения - Создание UDP-чата
void ChatWindow::UdpChat(QString nick, int port)
{
    // Если соединение уже открыто, то закрываем его
    if(socket != nullptr)
    {
      socket->close();
      delete socket;
      socket = nullptr;
    }
    // UDP - обмен сообщениями внутри локальной сети
    //  TCP/IP
    // 1. Создание сокета для работы с сетью
    socket = new QUdpSocket(this);
    // QHostAddress::Any позволяет принимать сообщения со всех IP адресов
    // Занятие определённого порта - все входящие сообщения на этот порт будут поступать в текущий диалог
    if(socket->bind(QHostAddress::Any, port))
    {
      // При получении данных (сигнал readyRead) вызывается метод (слот) read, который читает и обрабатывает сообщение
      connect(socket, SIGNAL(readyRead()), this, SLOT(read()));
    }
    else
    {
      // Какая-то программа на этом устройстве уже заняла порт port
      QMessageBox::critical(this, "Ошибка!", "Заданный порт уже занят другой программой. Выберите другой порт (1024 - ...) и попробуйте подключиться еще раз.");
      return;
    }
    send(new_user_log + " - в чате", USUAL_MESSAGE);
    QTime now = QTime::currentTime();
    QString nowStr = now.toString("hh:mm:ss");
    QString str = nowStr + " " + new_user_log;
    send(str, PERSON_ONLINE);
}

// Отправка сообщения
void ChatWindow::send(QString str, MessageType type)
{
    // Полный пакет данных будет в массиве data
    QByteArray data; // Массив данных для отправки
    // Последовательный вывод байт в data
    QDataStream out(&data, QIODevice::WriteOnly);
    out << qint8(type); // Тип сообщения
    out << str; // Само сообщение
    // Отправка полученного массива данных всем в локальный сети на порт указанный в интерфейсе
    socket->writeDatagram(data, QHostAddress::Broadcast, port);
}

// Получение сообщения
void ChatWindow::read()
{
    while (socket->hasPendingDatagrams())
    {
      // Массив (буфер) для полученных данных
      QByteArray buf;
      // Установка массиву размера, соответствующего размеру полученного пакета данных
      buf.resize(socket->pendingDatagramSize());
      QHostAddress* address = new QHostAddress();
      // Принятие данных, пришедших по сети
      socket->readDatagram(buf.data(), buf.size(), address);
      // Разбор полученного пакета
      QDataStream in(&buf, QIODevice::ReadOnly);
      // Получение типа пакета
      qint8 type = 0;
      in >> type;
      QString str;
      in >> str;
      if(str.length() == 0)
      {
        return;
      }
      if (type == USUAL_MESSAGE)
      {
        // Отображение строки в интерфейсе
        ui->listWidget->addItem(str);
      }
      else if (type == WHO_IS_ONLINE)
      {
        QTime now = QTime::currentTime();
        QString nowStr = now.toString("hh:mm:ss");
        send(nowStr + " " + new_user_log, PERSON_ONLINE);
      }
    }
}

// Нажатие на кнопку ОК
void ChatWindow::returnPressed()
{
    QString text = ui->lineEdit_2->text();
    if (text.isEmpty())
    {
        return;
    }
    // Вся строка сообщения: "ник: сообщение"
    send(new_user_log + ": " + text, USUAL_MESSAGE);
    ui->lineEdit_2->clear();
    QTime now = QTime::currentTime();
    QString nowStr = now.toString("hh:mm:ss");
    send(nowStr + " " + new_user_log, PERSON_ONLINE);
}

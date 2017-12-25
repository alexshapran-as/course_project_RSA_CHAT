#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "secondwindow.h"
#include "rsa.h"
#include <QTimer>
#include <QTime>

Rsa rsa;
Key keys;

long pub_key_otheruser;
long exp_key_otheruser;

extern QString new_user_log;
extern int port;
extern QListWidgetItem * item_;

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    keys = rsa.produce_keys(); // создание ключей
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
    send(new_user_log + " - вышел из чата", USUAL_MESSAGE);
    socket->close();
    SecondWindow * second_window = new SecondWindow(this);
    this->close();
    second_window->showFullScreen();
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
        if (str.length() == 0)
        {
            return;
        }
        unsigned int size = 0;
        if (str.length() > width_widget)
        {
            size = str.length() / width_widget + 1;
            for (unsigned int i = 0; i < size; ++i)
            {
                str.insert(width_widget * i, "-\n");
            }
        }

        if (type == USUAL_MESSAGE)
        {
            // Отображение строки в интерфейсе
            ui->listWidget->addItem(str);
        }
        else if (type == KEYS_MESSAGE)
        {
            QString temp_pub_key;
            for (auto it: str)
            {
                if (it != ':')
                {
                    temp_pub_key.push_back(it);
                }
                else break;
            }

            QString temp_exp_key;
            bool after = false;
            for (auto it: str)
            {
                if (it != ':' && !after)
                    continue;
                if (it == ':')
                {
                    after = true;
                    continue;
                }
                if (after)
                {
                    temp_exp_key.push_back(it);
                }
            }

            if (temp_exp_key.toLong() != keys.ekey &&
                    temp_pub_key.toLong() != keys.pkey)
            {
                exp_key_otheruser = temp_exp_key.toLong();
                pub_key_otheruser = temp_pub_key.toLong();
            }
            ui->listWidget->addItem(str);
        }
    }
}

// Кнопка отправить сообщение
void ChatWindow::on_pushButton_3_clicked()
{
    returnPressed();
}

// Нажатие на кнопку ОК
void ChatWindow::returnPressed()
{
    QString text = ui->lineEdit_2->text();
    if (text.isEmpty())
    {
        return;
    }
    // Если введен запрос на отправку ключей
    if (text.contains("/sendkeys"))
    {
        ui->listWidget->addItem(text);
        ui->lineEdit_2->clear();
        QString public_key = QString::number(keys.pkey);
        QString exp_key = QString::number(keys.ekey);
        send(public_key + ":" + exp_key, KEYS_MESSAGE);
        return;
    }
    // Шифрование
    QList<QListWidgetItem *> found = ui->listWidget->findItems("/sendkeys", Qt::MatchContains);
    if (found.count() > 0)
    {
        ui->listWidget->addItem("Сообщение до шифрования: \n" + text);
        for (auto & it : text)
        {
            it = QChar((int)rsa.endecrypt((long)it.unicode(), exp_key_otheruser, pub_key_otheruser));
        }
    }
    send(new_user_log + ": " + text, USUAL_MESSAGE);
    ui->lineEdit_2->clear();
    QTime now = QTime::currentTime();
    QString nowStr = now.toString("hh:mm:ss");
    send(nowStr + " " + new_user_log, PERSON_ONLINE);
}

// Выбор сообщения для расшифровки
void ChatWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    int index = item->text().indexOf(":");
    QString temp;
    for (int i = 0; i < index; ++i)
    {
        temp.push_back(item->text()[i]);
    }
    if (temp == new_user_log)
    {
        QMessageBox::information(this, "Информация", "Собственные сообщения не подлежат расшифровки.");
        return;
    }
    QString toDecrypt = item->text();
    toDecrypt.remove(0, ui->lineEdit->text().size() + 1);

    for (auto& it: toDecrypt)
    {
        it = QChar((int)rsa.endecrypt((long)it.unicode(),keys.dkey,keys.pkey));
    }
    toDecrypt.remove(0, 1);
    unsigned int size = 0;
    if (toDecrypt.length() > width_widget)
    {
        size = toDecrypt.length() / width_widget + 1;
        for (unsigned int i = 0; i < size; ++i)
        {
            toDecrypt.insert(width_widget * i, "\n");
        }
    }
    item->setText(ui->lineEdit->text() + ": " + toDecrypt);
}

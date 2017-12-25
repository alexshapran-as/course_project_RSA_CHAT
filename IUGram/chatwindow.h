#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QObject>
#include <QUdpSocket>

const unsigned int width_widget = 19;

enum MessageType {
  USUAL_MESSAGE, // Обычный текст
  PERSON_ONLINE, // Сообщение "Я-онлайн"
//  WHO_IS_ONLINE, // Запрос о статусе пользователей
  KEYS_MESSAGE, // сообщение с ключами
};

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = 0);
    ~ChatWindow();
public:
    Ui::ChatWindow *ui;

private slots:
    void returnPressed();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();

private:
    // Для работы по UDP-протоколу в
    // локальной сети
    QUdpSocket* socket;
    void UdpChat(QString nick, int port);
    void send(QString str, MessageType type);

private slots:
    void read();
    void on_listWidget_itemClicked(QListWidgetItem *item);
};

#endif // CHATWINDOW_H

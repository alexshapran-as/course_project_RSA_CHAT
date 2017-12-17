#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QObject>
#include "client.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QListWidgetItem *item, QWidget *parent = 0);
    ~ChatWindow();
public:
    Ui::ChatWindow *ui;

public slots:
    void appendMessage(const QString &from, const QString &message);

private slots:
    void returnPressed();
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void showInformation();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();

private:
    Client client;
    QString myNickName;
};

#endif // CHATWINDOW_H

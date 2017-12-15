#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

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
private slots:
    void on_pushButton_clicked();
};

#endif // CHATWINDOW_H

#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "iugram.h"
#include "chatwindow.h"

extern QLinkedList<QPair<QString, QString>> list_of_users;

SecondWindow::SecondWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
}

SecondWindow::~SecondWindow()
{
    delete ui;
}

// Кнопка поиска собеседника - лупа
void SecondWindow::on_pushButton_clicked()
{
    ui->listWidget->clear();
    // Вектор найденных юзеров
    QVector<QString> found_users;
    // Логин user-а, которого ищет текущий user
    QString user_find = ui->lineEdit->text();
    // Если в поле поиск ничего не введено
    if (user_find == "")
    {
        // Сообщение о том, что не был введен логин для поиска user-a
        QString temp = "Вы не ввели логин для поиска пользователя!";
        QMessageBox::information(this, "Не был введен логин для поиска пользователя!", temp);
        return;
    }
    for (auto it : list_of_users)
    {
        // Если среди списка зарегистрированных user-ов есть тот, которого ищут
        // (не учитывается регистр, поиск возможен даже для неполного логина, тогда выводятся
        // все логины, в которые входит ищущийся логн)
        if (it.second.contains(user_find, Qt::CaseInsensitive))
        {
            found_users << (it.second);
        }
    }
    if (!found_users.isEmpty())
    {
        // Вывод всех найденных user-ов
        for (auto item : found_users)
        {
            ui->listWidget->addItem(item);
        }
    }
    else
    {
         // Сообщение о том, что не удалось найти такого user-а
         QString temp = "Ни одного юзера не найдено:(";
         QMessageBox::information(this, "Поиск не дал результатов!", temp);
    }
}

// Кнопка выхода - крестик
void SecondWindow::on_pushButton_2_clicked()
{
    IUGram * first_window = new IUGram(this);
    this->close();
    first_window->show();
}

// Выбор пользователя для открытия чата с ним
void SecondWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ChatWindow * chat_window = new ChatWindow(item, this);
    this->close();
    chat_window->show();
}

// Нажатие кнопки ОК
void SecondWindow::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}

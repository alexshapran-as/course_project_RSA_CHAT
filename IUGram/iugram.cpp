#include "iugram.h"
#include "ui_iugram.h"
#include "settings.h"
#include "secondwindow.h"

QString new_user_pass;
QString new_user_log;
QLinkedList<QPair<QString, QString>> list_of_users;

IUGram::IUGram(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IUGram)
{
    ui->setupUi(this);
}

IUGram::~IUGram()
{
    delete ui;
}

void set_list_of_users()
{
    // Получение списка зерегистрированных пользователей (TODO: get() запросы)
//    manager = new QNetworkAccessManager();  // Инициализация менеджера
//    QUrl url("http://www...");              // Инициализация URL, с которого будут получаться данные
//    QNetworkRequest request;                // Отправляемый запрос
//    request.setUrl(url);                    // Устанавлвиваем URL в запрос
//    manager->get(request);                  // Выполняем запрос
//    QNetworkReply *reply;
//    QString temp_list_of_users;
//    temp_list_of_users.push_back(reply->readAll());
    QVector<QPair<QString, QString>> local_users;
    local_users.push_back(QPair<QString, QString>("S", "Sasha"));
    local_users.push_back(QPair<QString, QString>("A", "AKosoj"));
    for (auto it : local_users)
    {
        list_of_users.push_back(qMakePair(it.first,it.second));
    }
}

// Кнопка настройки - шестеренка
void IUGram::on_pushButton_clicked()
{
    Settings * settings_window = new Settings(this);
    this->close();
    settings_window->show();
}

// Кнопка войти
void IUGram::on_pushButton_2_clicked()
{
    // Считывание пароля нового user-а
    new_user_pass = ui->lineEdit->text();
    // Считывание логина нового user-а
    new_user_log = ui->lineEdit_2->text();

    // Если логин и пароль уже зарегистрированы для данного пользователя
    if (qFind(list_of_users.begin(), list_of_users.end(), QPair<QString, QString>(new_user_pass, new_user_log)) != list_of_users.end())
    {
        // Создание второго окна со списком сообщений
        SecondWindow * second_window = new SecondWindow(this);
        this->close();
        second_window->show();
    }
    else
    {
        // Сообщение о том, что вход не удался, нужно зарегистрироваться
        QString temp = "Неправильный логин или пароль!";
        QMessageBox::information(this, "Ошибка авторизации!", temp);
    }
}

// Кнопка зарегистрироваться
void IUGram::on_pushButton_3_clicked()
{
    // Считывание пароля нового user-а
    new_user_pass = ui->lineEdit->text();
    // Считывание логина нового user-а
    new_user_log = ui->lineEdit_2->text();

    // Если существует зарегистрированный пользователь с таким же логином
    for (auto it : list_of_users)
    {
        if (it.second == new_user_log)
        {
            bad_log = true;
        }
    }
    if (bad_log)
    {
        // Сообщение о том, что регистрация не удалась, потому что существует пользователь с таким логином
        QString temp = "Данный логин уже зарегистрирован! Пожалуйста, придумайте другой логин.";
        QMessageBox::information(this, "Ошибка регистрации!", temp);
        bad_log = false;
        return;
    }

    // Если одно из полей логин или пароль не заполнено
    if (new_user_log == "" || new_user_pass == "" || new_user_log == "                      Логин" || new_user_pass == "                     Пароль")
    {
        // Сообщение о том, что регистрация не удалась, потому что нельзя оставлять поля логин или пароль пустыми
        QString temp = "Оба поля (логин и пароль) должны быть заполнены!";
        QMessageBox::information(this, "Ошибка регистрации!", temp);
        return;
    }

    if (new_user_pass.size() < 8)
    {
        // Сообщение о том, что регистрация не удалась, потому что пароль очень короткий
        QString temp = "Минимальная длина пароля - 8 символов!";
        QMessageBox::information(this, "Ошибка регистрации!", temp);
        return;
    }
    // Если регистрация прошла успешно, то добавляем логин и пароль нового user-а в список (TODO post() запросы)
    list_of_users.push_back(qMakePair(new_user_pass,new_user_log));
    // Создание второго окна со списком сообщений
    SecondWindow * second_window = new SecondWindow(this);
    this->close();
    second_window->show();
}

// Нажатие на кнопку ввод/enter на клавиатуре при нахождении курсора в поле с логином
void IUGram::on_lineEdit_2_returnPressed()
{
    on_pushButton_2_clicked();
}

// Нажатие на кнопку ввод/enter на клавиатуре при нахождении курсора в поле с паролем
void IUGram::on_lineEdit_returnPressed()
{
    on_pushButton_2_clicked();
}

// Двойное нажатие по слову логин приводит к его исчезновению
void IUGram::on_lineEdit_2_selectionChanged()
{
    ui->lineEdit_2->setText("");
}

// Двойное нажатие по слову пароль приводит к его исчезновению
void IUGram::on_lineEdit_selectionChanged()
{
    ui->lineEdit->setText("");
    ui->lineEdit->setEchoMode(QLineEdit::Password);
}

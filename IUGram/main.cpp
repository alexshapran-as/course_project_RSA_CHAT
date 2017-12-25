#include "iugram.h"
#include "chatwindow.h"
#include <QApplication>
#include <QDesktopWidget>


int main(int argc, char *argv[])
{
    set_list_of_users();
    QApplication a(argc, argv);

    IUGram first_window;
    first_window.setGeometry(QRect(0, 0, a.desktop()->width(), a.desktop()->height()));
    first_window.show();

    return a.exec();
}

#include "connect.h"
#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connect w;
    w.show();
    w.setWindowTitle("龙泉寺皈依录入软件");
    //QFont font;
    //font.setFamily("KaiTi");
    //a.setFont(font);
    //a.setStyleSheet("QLineEdit, QTableView { border: 0px; }");

    return a.exec();
}

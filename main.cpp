#include "connect.h"
#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connect w;
    w.show();
    w.setWindowTitle("龙泉寺皈依录入软件");
    /*
    QFont font = a.font();
    font.setPointSize(16);
    font.setBold(true);
    font.setFamily("LiSu");
    a.setFont(font);
    */
    return a.exec();
}

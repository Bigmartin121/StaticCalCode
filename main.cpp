#include "statistics.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    statistics w;
    w.setWindowTitle("统计代码行数");
    w.show();
    return a.exec();
}

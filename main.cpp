#include "adsbparser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AdsbParser w;
    w.show();

    return a.exec();
}

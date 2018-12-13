#include "daypicker.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DayPicker w;
    w.show();

    return a.exec();
}

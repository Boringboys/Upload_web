#include "upload_web.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Upload_web w;
    w.show();

    return a.exec();
}

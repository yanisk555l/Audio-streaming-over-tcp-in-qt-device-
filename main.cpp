#include "BackendAudio.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    quint16 port = 15002;
    QString pathDirectory="/home/kali/xremote/audio/";
    BackendAudio* backAudio = new BackendAudio(port,pathDirectory);

    return a.exec();
}

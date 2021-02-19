#ifndef BACKENDAUDIO_H
#define BACKENDAUDIO_H

#include <QtCore>
#include <QtNetwork>
#include "audioinput.h"

class BackendAudio : public QObject
{
    Q_OBJECT
public:
    BackendAudio(quint16 port, QString pathDirectory);


public slots:
    void writeData(QByteArray data);
    void newConnection();
    void zeropointer();
    void readyRead();

private:
    QTcpServer *server;
    QTcpSocket *socket;
    AudioInput* input;
};

#endif // BACKENDAUDIO_H

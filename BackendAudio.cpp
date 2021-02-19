#include "BackendAudio.h"
#include<iostream>

BackendAudio::BackendAudio(quint16 port, QString pathDirectory)
{
    // create an AudioInput object to stream audio from Mics/Audio File
    input = new AudioInput(this, pathDirectory);
    // send data from input to client over TCP socket
    connect(input, SIGNAL(dataReady(QByteArray)), this, SLOT(writeData(QByteArray)));

    // create a QTcpServer to accept incomming TCP connexions
    socket = 0;
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), SLOT(newConnection()));
    server->listen(QHostAddress::Any, port);


}

void BackendAudio::readyRead()
{
    QByteArray data;
    while (socket->bytesAvailable() > 0)
        data.append(socket->readAll());
    std::string request= data.toStdString();
    std::string delimiter = ":";
    std::string action = request.substr(0, request.find(delimiter));

    if (action=="startA") {
        std::string path = request.substr(request.find(delimiter)+1, request.size());
        //std::cout<<path<<std::endl;
        input->streamFile(path);
    }else if (action=="startLA"){
        input->streamMics();
    }else if (action=="stopLA"){
        input->stopMics();
    }else if (action=="stopA"){
        input->stopStream();
    }else if (action=="pauseA"){
        input->pauseStream();
    }else if (action=="recA"){
        input->recordMic();
    }
    else if (action=="stopRA"){
            input->stopRec();
    }
}

void BackendAudio::newConnection()
{
    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(destroyed()), SLOT(zeropointer()));
}

void BackendAudio::zeropointer()
{
    socket = 0;
}

void BackendAudio::writeData(QByteArray data)
{
    if (socket)
        socket->write(data);
}

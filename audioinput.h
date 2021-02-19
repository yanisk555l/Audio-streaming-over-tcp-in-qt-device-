#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QtCore>
#include <QtMultimedia>
//#include "server.h"

class AudioInput : public QObject
{
    Q_OBJECT
public:
    AudioInput( QObject *parent, QString pathFolder) ;
    void streamFile( std::string path);
    void streamMics();
    void stopStream();
    void stopMics();
    void pauseStream();
    QString recordMic();
    void stopRec();



signals:
    void dataReady(QByteArray data);

public slots:
    void processBuffer(const QAudioBuffer &buffer);
    void readyRead();
     void mediaStatus(QMediaPlayer::MediaStatus state);

private:
    QAudioInput *audio;
    QIODevice *device;
    QAudioRecorder* audioRec;
    QMediaPlayer* media;
    QAudioProbe* probe;
    QAudioProbe* probeFile;
    bool isStreamingFile;
    bool isRecording;
    QString pathFolder;
    QString pathFileRemove;


};

#endif // AUDIOINPUT_H


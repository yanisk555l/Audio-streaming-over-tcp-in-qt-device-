#include "audioinput.h"
#include <iostream>
AudioInput::AudioInput( QObject *parent, QString pathDirectory) : QObject(parent)
{
    pathFolder=pathDirectory;
    isRecording=false;
    isStreamingFile=false;
    if(!QDir(pathFolder).exists()){
        QDir().mkpath(pathFolder);
    }
}

void AudioInput::stopMics(){
    probe->~QAudioProbe();
    if(!isRecording){
        audioRec->stop();
        QFile file (pathFileRemove);
        file.remove();
    }
}

void AudioInput::stopRec(){
    audioRec->stop();
    isRecording=false;
}

void AudioInput::pauseStream(){
    media->pause();
}

void AudioInput::stopStream(){
    media->stop();
    probeFile->~QAudioProbe();
    isStreamingFile=false;
}

QString AudioInput::recordMic(){
    audioRec = new QAudioRecorder;
    audioRec->defaultAudioInput();
    QAudioEncoderSettings audioSettings;
    audioSettings.setChannelCount(1);
    audioSettings.setCodec("audio/AMR");
    audioSettings.setBitRate(8);
    audioSettings.setQuality(QMultimedia::HighQuality);
    audioSettings.setSampleRate(8000);

    audioRec->setAudioSettings(audioSettings);
    QString day = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString hour = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString filePath= pathFolder+"audio-"+day+"-"+hour;
    audioRec->setOutputLocation(QUrl(filePath));
    audioRec->record();
    isRecording=true;
    return filePath;
}

void AudioInput::streamMics(){
    if(!isRecording){
        // run the microphone and save the output file to remove it later
        pathFileRemove= recordMic();
        isRecording=false;
    }
     probe = new QAudioProbe;
     probe->setSource(audioRec);
     connect(probe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processBuffer(QAudioBuffer)));

}


void AudioInput::streamFile(std::string path){

    if(isStreamingFile){
        media->play();
    }else{
        media = new QMediaPlayer;
        media->setMuted(true);
        QString Qpath = QString::fromUtf8(path.data(),path.size());
        media->setMedia(QUrl::fromLocalFile(Qpath));
        media->play();
        probeFile = new QAudioProbe;
        probeFile->setSource(media);
        connect(probeFile, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processBuffer(QAudioBuffer)));
        isStreamingFile=true;

        connect(media,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(mediaStatus(QMediaPlayer::MediaStatus)));

    }

}

void AudioInput::mediaStatus(QMediaPlayer::MediaStatus state)
{
    if(state == QMediaPlayer::EndOfMedia)
    {
        QByteArray data;
        data.append("eofAudio");
        stopStream();
        emit dataReady(data);;
    }
}

void AudioInput::processBuffer(const QAudioBuffer &buffer)
{
    QByteArray data;
    //qDebug()<<buffer.format();
    data.append(buffer.constData<char>(), buffer.byteCount());

    emit dataReady(data);
}

void AudioInput::readyRead()
{
    QByteArray data;
    //Check the number of samples in input buffer
    qint64 len = audio->bytesReady();

    //Read sound samples from input device to buffer
    if (len > 0)
    {
        data.resize(len);
        device->read(data.data(), len);
    }
    emit dataReady(data);
}

/*
void AudioInput::sendListAudioFiles(){
    QByteArray data;
    QDir directory(pathFolder);
    QStringList list = directory.entryList();
    foreach(QString filename, list) {
        data.append(filename.toUtf8().data());
    }
     emit dataReady(data);
}
*/

#include "music_player.h"
#include "ui_music_player.h"

/*
 *author: jorgo mihallari
 *date: 2022
 *description: this is a music player application. we don't know the name yet. written in Qt 6.4.
 *

*/


music_player::music_player(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::music_player)
{
    ui->setupUi(this);

    //create the QMediaPlayer class and QAudioOutput class
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);

    //SIGNALS and SLOTS, the QMediaPlayer signals go to the music_player(our ui) slot
    connect(player, &QMediaPlayer::positionChanged, this, &music_player::on_positionChanged); //change the progress slider
    connect(player, &QMediaPlayer::durationChanged, this, &music_player::on_durationChanged); //a new song is basically loaded here
    connect(player, &QMediaPlayer::positionChanged, this, &music_player::displaySongLength); //display the timestamp of the song



}
/*!
 * \brief music_player::~music_player
 *
 *
 */
music_player::~music_player()
{
    delete ui;
}

void music_player::on_progressSlider_sliderMoved(int position)
{
    player->setPosition(position);

}


void music_player::on_volumeSlider_sliderMoved(int position)
{ //this is a slot signal comes from slider moving

    qreal linearVolume = QAudio::convertVolume(position / qreal(100.0),
                                               QAudio::LogarithmicVolumeScale,
                                               QAudio::LinearVolumeScale);

    audioOutput->setVolume(linearVolume);

}


void music_player::on_play_clicked()
{
    player->play();
}


void music_player::on_stop_clicked()
{
    player->pause();
}

void music_player::on_positionChanged(int position)
{

    ui->progressSlider->setValue(position);

}

void music_player::on_durationChanged(int position)
{
    ui->progressSlider->setMaximum(position);

    //the following will be run everytime a new song is selected
    int myduration = player->duration();
    int song_remainder = myduration % 60000;

    int song_secs = song_remainder/1000;
    QString song_sec_str = QString::number(song_secs);
    int song_mins = myduration / 60000;
    QString song_mins_str = QString::number(song_mins);
    QString song_duration = song_mins_str + ":" + song_sec_str;
    ui->songLength->setText(song_duration);



}



void music_player::on_start_clicked()
{
    //Load and play the song
    player->setAudioOutput(audioOutput);

    player->setSource(QUrl::fromLocalFile("/C:/Users/buck_/Downloads/Southstar - miss you.mp3"));
    player->play();
    qDebug()<<player->errorString();

    QUrl song_name = player->source();

    QString song_namestr = song_name.fileName();

    ui->listWidget->addItem(song_namestr);
}

void music_player::displaySongLength(int position){

    int song_progress = player->position();

    //song progression Timer
    int song_remainder = song_progress % 60000;
    int secs_progress = song_remainder / 1000;

    if(secs_progress <= 9){
        QString secs_progress_str = "0" + QString::number(secs_progress);
        int mins_progress = song_progress / 60000;
        QString mins_progress_str = QString::number(mins_progress);
        QString running_song_progress = mins_progress_str + ":" + secs_progress_str;
        ui->label_4->setText(running_song_progress);
    }
    else{
        QString secs_progress_str = QString::number(secs_progress);
        int mins_progress = song_progress / 60000;
        QString mins_progress_str = QString::number(mins_progress);
        QString running_song_progress = mins_progress_str + ":" + secs_progress_str;
        ui->label_4->setText(running_song_progress);
    }

}




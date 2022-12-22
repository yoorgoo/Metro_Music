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


    songsmodel = new QStringListModel(this);
    songDirectory = new QDir;
    ptr_repeat = new bool;
    ptr_shuffle = new bool;



    //SIGNALS and SLOTS, the QMediaPlayer signals go to the music_player(our ui) slot
    connect(player, &QMediaPlayer::positionChanged, this, &music_player::on_positionChanged); //change the progress slider
    connect(player, &QMediaPlayer::durationChanged, this, &music_player::on_durationChanged); //a new song is basically loaded here
    connect(player, &QMediaPlayer::positionChanged, this, &music_player::displaySongLength); //display the timestamp of the song



    //===Image settings of buttons===//
    ui->repeat->setIcon(QIcon(":/resources/images/black_circle.png"));
    ui->repeat->setCheckable(true);

    ui->shuffle_b->setIcon(QIcon(":/resources/images/black_cross_arrows.png"));
    ui->shuffle_b->setCheckable(true);




}
/*!
 * \brief music_player::~music_player
 *
 *
 */
music_player::~music_player()
{
    delete ui;
    delete audioOutput;
    delete songsmodel;
    delete songDirectory;
    delete ptr_repeat;
    delete ptr_shuffle;

}

void music_player::on_progressSlider_sliderMoved(int position)
{
    player->setPosition(position);

}


void music_player::on_volumeSlider_sliderMoved(int position)
{ //this is a slot signal comes from slider moving

    qreal linearVolume = QAudio::convertVolume(position*1.3 / qreal(100.0),
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
    //QString song_sec_str = QString::number(song_secs);
    if(song_secs <= 9){
        QString song_sec_str = "0" + QString::number(song_secs);
        int song_mins = myduration / 60000;
        QString song_mins_str = QString::number(song_mins);
        QString song_duration = song_mins_str + ":" + song_sec_str;
        ui->songLength->setText(song_duration);
    }
    else{
        QString song_sec_str = QString::number(song_secs);
        int song_mins = myduration / 60000;
        QString song_mins_str = QString::number(song_mins);
        QString song_duration = song_mins_str + ":" + song_sec_str;
        ui->songLength->setText(song_duration);
    }
}

void music_player::on_start_clicked()
{
    /*
     * We have a major bug that can kill the program.
     * this occurs when we open a folder with music, play a song, and then open up another folder, and click next
     * What happens is the previous song is still in the player class. The songlist model has changed and so when it looks for a next song it doesn't exist in the model
     * To fix this we need to set the source of the player to NULL. Now when we update the model we won't have anything in the playe class.
     *
     */

    QString dirFolder = QFileDialog::getExistingDirectory(this, tr("Open Folder"),"C:/Users/buck_/Desktop/");
    QDir song_directory = dirFolder;
    *songDirectory = song_directory;

    QStringList songListView; //this gets pushed to the model


    //What if we open multiple folders, or switch folders. Now things are broken.
    //we need to clear the model of all the previous songs.
    player->stop();
    player->setSource(QUrl::fromLocalFile("NULL")); // This forces the player to not have audio. We need to click on a song. This is so we don't break things when switching folders


    //open a folder directory
    foreach(QFileInfo var, song_directory.entryInfoList()){
        //we need to differntiate between .mp3 files and all others
        if(var.suffix() == "mp3"){ //get the suffix of the file
            songListView.append(var.fileName());
            //songListView << var.fileName();
        }

     }
    //====This is were the model data is set====//
    songsmodel->setStringList(songListView);
    ui->songList->setModel(songsmodel);
    ui->songList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void music_player::displaySongLength(int position){

    int song_progress = player->position();

    //song progression Timer
    int song_remainder = song_progress % 60000;
    int secs_progress = song_remainder / 1000;

    if(secs_progress <= 9){ //run when <10 so we get :04 or :05 and so on
        QString secs_progress_str = "0" + QString::number(secs_progress);
        int mins_progress = song_progress / 60000;
        QString mins_progress_str = QString::number(mins_progress);
        QString running_song_progress = mins_progress_str + ":" + secs_progress_str;
        ui->label_4->setText(running_song_progress);

    }
    else{ //:run when >9 so we get :11, :12 and so on
        QString secs_progress_str = QString::number(secs_progress);
        int mins_progress = song_progress / 60000;
        QString mins_progress_str = QString::number(mins_progress);
        QString running_song_progress = mins_progress_str + ":" + secs_progress_str;
        ui->label_4->setText(running_song_progress);

    }

}


void music_player::on_nextSong_clicked()
{
    //We need to get the index of the current song playing. note this is not the index that we are currently highlighting
    //the player class should have the name of the song currently playing
    //we can find the index of the current song playing, get the index+1, find the song at index + 1 and play.
    //player->stop();

    if(player->hasAudio()){ //check for audio so we don't break things
        if(*ptr_shuffle == false)
        {
            int num_songs = songsmodel->stringList().size();
            //int song_index = player->activeAudioTrack(); //why am I using the player class? we need to get it from the song mondel
            QUrl curr_song = player->source(); //this includes the file path
            qDebug() << "filename" << curr_song.fileName(); //this is just the filename

            QString curr_song_str = curr_song.fileName(); //this is just the file name. we can look for it in the songs model and then get the index.
            qDebug() << "curr_song_str = " << curr_song_str;

            QString songDirectory_str = songDirectory->absolutePath(); //just the directory path
            qDebug() << "songDirectory_str = " << songDirectory_str;

            int song_index = songsmodel->stringList().indexOf(curr_song_str);
            qDebug() << song_index;
            int next_song_index = song_index + 1;

            if(next_song_index > (num_songs-1)){
                next_song_index = 0;
            }

            QString next_song_str = songsmodel->stringList().at(next_song_index);
            qDebug() << next_song_str;

            QString song_path = songDirectory_str +"/" +  next_song_str;
            //QString song_path = songsmodel->stringList().value(next_song_index);

            player->setSource(QUrl::fromLocalFile(song_path));
            player->play();
        }
        else
        {
            int num_songs = songsmodel->stringList().size();
            int rand_num = rand() % (num_songs + 1);
            //int song_index = player->activeAudioTrack(); //why am I using the player class? we need to get it from the song mondel
            QUrl curr_song = player->source(); //this includes the file path
            qDebug() << "filename" << curr_song.fileName(); //this is just the filename

            QString curr_song_str = curr_song.fileName(); //this is just the file name. we can look for it in the songs model and then get the index.
            qDebug() << "curr_song_str = " << curr_song_str;

            QString songDirectory_str = songDirectory->absolutePath(); //just the directory path
            qDebug() << "songDirectory_str = " << songDirectory_str;

            int song_index = songsmodel->stringList().indexOf(curr_song_str);
            qDebug() << song_index;
            int next_song_index = rand_num;

            if(next_song_index > (num_songs-1)){
                next_song_index = 0;
            }

            QString next_song_str = songsmodel->stringList().at(next_song_index);
            qDebug() << next_song_str;

            QString song_path = songDirectory_str +"/" +  next_song_str;
            //QString song_path = songsmodel->stringList().value(next_song_index);

            player->setSource(QUrl::fromLocalFile(song_path));
            player->play();

        }
    }



}


void music_player::on_songList_doubleClicked(const QModelIndex &index)
{
    player->setAudioOutput(audioOutput);

    QStringList song_nameslist = songsmodel->stringList();
    QString song_namestr = songsmodel->stringList().at(index.row());
    QString song_path = "C:/Users/buck_/Desktop/mymusic/" + song_namestr;
    //qDebug() <<song_path;
    player->setSource(QUrl::fromLocalFile(song_path));
    player->play();
}


void music_player::on_prevSong_clicked()
{

    //Flow of the function
    /*
     *
     * 1) Get the current song playing (QUrl), this includes the filepath
     * 2) Convert the QUrl to the song name as a (QString)
     * 3) Get the index of the song playing
     * 4) Calculate the prev_song index
     * 5) Check if the prev song is > 0
     * 6) Get the song name of the prev song
     * 7) Create the full song path of the prev song
     * 8) Play
     *
     * */

    if(player->hasAudio()){
        if(*ptr_shuffle == false)
        {
            QUrl curr_song = player->source(); //this includes the file path
            QString curr_song_str = curr_song.fileName(); //this is just the file name. we can look for it in the songs model and then get the index.
            QString songDirectory_str = songDirectory->absolutePath(); //just the directory path

            int song_index = songsmodel->stringList().indexOf(curr_song_str); //get the index of the current song in the songsmodel
            int prev_song_index = song_index -1; //set the index of the prev song

            if(prev_song_index < 0){
                prev_song_index = 0; //do we want to go to end??? or just stay at 0?
            }

            QString prev_song_str = songsmodel->stringList().at(prev_song_index);

            QString song_path = songDirectory_str + "/" +  prev_song_str;
            //QString song_path = songsmodel->stringList().value(next_song_index);

            player->setSource(QUrl::fromLocalFile(song_path));
            player->play();
        }

        else
        {

            int num_songs = songsmodel->stringList().size();
            int rand_num = rand() % (num_songs + 1);
            //int song_index = player->activeAudioTrack(); //why am I using the player class? we need to get it from the song mondel
            QUrl curr_song = player->source(); //this includes the file path
            qDebug() << "filename" << curr_song.fileName(); //this is just the filename

            QString curr_song_str = curr_song.fileName(); //this is just the file name. we can look for it in the songs model and then get the index.
            qDebug() << "curr_song_str = " << curr_song_str;

            QString songDirectory_str = songDirectory->absolutePath(); //just the directory path
            qDebug() << "songDirectory_str = " << songDirectory_str;

            int song_index = songsmodel->stringList().indexOf(curr_song_str);
            qDebug() << song_index;
            int next_song_index = rand_num;

            if(next_song_index > (num_songs-1)){
                next_song_index = 0;
            }

            QString next_song_str = songsmodel->stringList().at(next_song_index);
            qDebug() << next_song_str;

            QString song_path = songDirectory_str +"/" +  next_song_str;
            //QString song_path = songsmodel->stringList().value(next_song_index);

            player->setSource(QUrl::fromLocalFile(song_path));
            player->play();


        }

    }



}



void music_player::on_repeat_toggled(bool checked)
{
    *ptr_repeat = checked;
    qDebug() << "ptr_repeat = " << *ptr_repeat;

    if(checked) {
        ui->repeat->setIcon(QIcon(":/resources/images/blue_circle.png"));
        player->setLoops(-1);
    }
    else{
        ui->repeat->setIcon(QIcon(":/resources/images/black_circle.png"));
        player->setLoops(1);
    }
}




void music_player::on_shuffle_b_toggled(bool checked)
{
    *ptr_shuffle = checked;

    if(checked)
    {
        ui->shuffle_b->setIcon(QIcon(":/resources/images/blue_cross_arrows.png"));
    }
    else
    {
        ui->shuffle_b->setIcon(QIcon(":/resources/images/black_cross_arrows.png"));
    }
}



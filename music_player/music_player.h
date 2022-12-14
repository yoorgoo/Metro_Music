#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDebug>
#include <QListWidgetItem>
#include <QDir>
#include <QFileDialog>
#include <QtCore>
#include <QtGui>



QT_BEGIN_NAMESPACE
namespace Ui { class music_player; }
QT_END_NAMESPACE

/*!
 * \brief The music_player class
 */
class music_player : public QMainWindow
{
    Q_OBJECT

public:


    music_player(QWidget *parent = nullptr);
    ~music_player();

private slots:

    /*!
     * \brief on_progressSlider_sliderMoved
     * \param position position of the song in ms
     */
    void on_progressSlider_sliderMoved(int position);

    /*!
     * \brief on_volumeSlider_sliderMoved
     * \param position position of the volume
     */
    void on_volumeSlider_sliderMoved(int position);

    /*!
     * \brief on_play_clicked
     * starts the song from the position it was stopped at
     */
    void on_play_clicked();

    /*!
     * \brief on_stop_clicked
     * puases the song
     */
    void on_stop_clicked();


    /*!
     * \brief on_positionChanged
     * \param position position of the progress meter
     */
    void on_positionChanged(int position);

    /*!
     * \brief displaySongLength displays the length of the song in (min:seconds) format in a label
     * \param length takes the length of the file in milliseconds
     */
    void displaySongLength(int position);

    /*!
     * \brief on_durationChanged
     * \param position position of the progress meter
     */
    void on_durationChanged(int position);

    /*!
     * \brief on_start_clicked open up and selects folder you want to add for you playlist
     */
    void on_start_clicked();


    /*!
     * \brief on_nextSong_clicked go to the next song
     */
    void on_nextSong_clicked();

    /*!
     * \brief on_songList_doubleClicked handles the double click on a song in the list view
     * \param index index that gets double clicked in the list view
     */
    void on_songList_doubleClicked(const QModelIndex &index);


    /*!
     * \brief on_prevSong_clicked go to the previous song
     */
    void on_prevSong_clicked();



    void on_repeat_toggled(bool checked);

private:
    Ui::music_player *ui;

    QMediaPlayer* player;
    QAudioOutput* audioOutput;

    QStringListModel *songsmodel; //model that gets filled with stringlist of songs in a folder
    QDir* songDirectory; //folder directory that holds the songs



};
#endif // MUSIC_PLAYER_H

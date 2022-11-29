#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include "qlistwidget.h"
#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDebug>
#include <QListWidgetItem>
#include <QDir>
#include <QFileDialog>


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

    void on_start_clicked();

    /*!
     * \brief on_songList_itemDoubleClicked starts playing song if double clicked
     * \param item this represents the song in a directory
     */
    void on_songList_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::music_player *ui;

    QMediaPlayer* player;
    QAudioOutput* audioOutput;
};
#endif // MUSIC_PLAYER_H

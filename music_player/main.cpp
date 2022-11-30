#include "music_player.h"
#include <QApplication>
#include <QFile>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Set the Theme
    QFile styleSheetFile("C:/Users/buck_/Desktop/qt_music/Takezo.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet {QLatin1String(styleSheetFile.readAll()) };
    a.setStyleSheet(styleSheet);



    music_player w;
    w.show();
    return a.exec();
}

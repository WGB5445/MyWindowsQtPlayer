#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QCoreApplication>
#include <QtMultimedia/QMediaPlayer>
#include <QtSql>
#include <QFileDialog>
#include <QMediaPlaylist>
#include <QListWidget>
#include <QList>
#include <QEvent>
#include <QMouseEvent>

#include <QPoint>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
extern  QString Time(qint64 time);
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getIndex();

    QStringList list1;
    QPoint offset;

   void mousePressEvent(QMouseEvent *event);
   void  mouseMoveEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   void ReadLyric(QString site);
private slots:
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

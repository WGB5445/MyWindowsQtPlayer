#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMediaPlayer  *music = new QMediaPlayer(this); //实例化一个QMediaPlayer的对象并指定父类为MainWindow
    QMediaPlaylist *playlist = new QMediaPlaylist(this);//实例化一个QMediaPlaylist的对象并指定父类为MainWindow
    playlist->setPlaybackMode(QMediaPlaylist::Loop);//把播放列表设为循环播放模式（但我感觉好像没啥用没有应有的效果，或者我不会用）
    music->setPlaylist(playlist);//让播放器和播放列表链接起来
    ui->verticalSlider->setMaximum(100);//设置纵向的音量条最大值为100（默认为0~99）
    ui->verticalSlider->setValue(50);//设置纵向的音量条的初始值为50
    music->setVolume(50);//设置音乐播放器的初始音量为50
    this->setWindowFlags(Qt::FramelessWindowHint);//设定本MainWindow窗口无外框
    //创建一个文件对话框，提供选择开始播放时的列表歌曲
    list1=QFileDialog::getOpenFileNames(this,QString::fromLocal8Bit("文件"),"../",QString::fromLocal8Bit("音频文件(*.mp3)"));

    //如果获取的文件路径的个数是0个，就不执行以下操作
    //如果获取文件路径个数不是0，则执行
    if(!list1.isEmpty()){
        music->setMedia(QUrl::fromLocalFile(list1.first()));//将音乐播放器的播放曲目设置为列表中的第一个
        //根据列表中路径的个数来循环执行
        //目的是对UI界面中的listwidget进行插入歌曲名的操作
        for(int i=0;i<list1.size();i++){

         QString  path=QDir::toNativeSeparators(list1.at(i));  //将地址中的/转换为在windows下的格式\\
                                                               //如果想\\转为/可以使用 QDir::fromNativeSeparators

            playlist->addMedia(QUrl::fromLocalFile(path)); //将path存放的“地址”以地址的形式添加到歌曲列表里
            QString Name=path.split("\\").last();          //将地址按“\\”切割，将切割后的最后一个字符串（也就是歌曲名）保存在name中
            QListWidgetItem *item = new QListWidgetItem(Name);//实例化一个listwidgetitem的对象（可以理解listwidget里的一行）
            item->setText(Name);                        //给这个item设置歌曲名称

            ui->listWidget->addItem(item);             //将刚刚设置好的一行插入到listwidget中

        }
        playlist->setCurrentIndex(0);//设置当前音乐
        ui->listWidget->scrollToItem(ui->listWidget->item(0));//滚动到当前音乐
        music->setMedia(QUrl::fromLocalFile(list1[0]));//把歌曲列表的第一个设为播放
    }
    //播放按钮 信号链接到播放功能
    connect(ui->pushButton,&QPushButton::clicked,[=]{
        music->play();
    });
    //暂停按钮 信号链接到暂停功能
    connect(ui->pushButton_2,&QPushButton::clicked,[=]{
        music->pause();

    });
    //进度条位置获取，播放时间改变后，播放器发出信号，用函数接收信号并设置进度条为当前位置(因为当前进度条最大长度设置为歌曲时间长度，所以播放时间就是位置)
    //设置时间标签为当前时间
    connect(music,&QMediaPlayer::positionChanged,[=](qint64 position){


        ui->horizontalSlider->setValue(position);
        ui->label->setText(Time(position));


    });
    //进度条位置获取，进度条改变后，播放器发出信号，用函数接收信号并设置进度条为当前位置
    //设置时间标签为当前时间
    connect(music,&QMediaPlayer::durationChanged,[=](qint64 duration){


        ui->horizontalSlider->setRange(0,duration);//根据播放时长来设置滑块的范围
        ui->horizontalSlider->setEnabled(duration>0);//通过是否有播放时长，来判断是否有歌，由此设置是否可以被响应
        ui->label_2->setText(Time(duration));//设置时长标签的显示，为当前歌曲的总时长
        ui->horizontalSlider->setPageStep(duration/10);//设置滑块的一步的长度为总时长的10分之一

    });
    //当列表当前索引改变时，也就是换歌时，让对应的item处于被选择的状态
    connect(playlist,&QMediaPlaylist::currentIndexChanged,[=](int value){

        ui->listWidget->item(value)->setSelected(true);
    });
    //上一曲的按键点击
    connect(ui->pushButton_3,&QPushButton::clicked,[=](){


        int row = playlist->mediaCount();//获取歌曲的数量
        int current  = playlist->currentIndex();//获取当前歌曲索引位置
        //如果当前为第一首歌，点击上一曲时，应转到最后一首歌
        if(current == 0){
            current = playlist->mediaCount();
        }
        //设置为上一曲，所以减一
        current = current - 1;
        playlist->setCurrentIndex(current);//设置当前歌曲
        ui->listWidget->scrollToItem(ui->listWidget->item(current));//滚动到当前歌曲对应的item

        music->setMedia(QUrl::fromLocalFile(list1[current]));//把当前歌曲路径名给播放器
        music->play();//开始播放

    });
    connect(ui->pushButton_4,&QPushButton::clicked,[=](){


        int row = playlist->mediaCount();//获取歌曲数量
        int current  = playlist->currentIndex();//获取当前位置
        if(current == row - 1){
            current= -1 ;
        }
        current = current + 1;
        playlist->setCurrentIndex(current);//设置当前音乐
        ui->listWidget->scrollToItem(ui->listWidget->item(current));//滚动到当前音乐
        music->setMedia(QUrl::fromLocalFile(list1[current]));
        qDebug()<<list1[current];
        music->play();





    });
    connect(ui->verticalSlider,&QSlider::valueChanged,[=](int val){
        music->setVolume(val);
    });



    connect(ui->pushButton_5,&QPushButton::clicked,[=]{
        this->close();
    });
    connect(ui->horizontalSlider,&QSlider::sliderMoved,music,&QMediaPlayer::setPosition);
    connect(ui->listWidget,&QListWidget::itemDoubleClicked,[=](QListWidgetItem *item){
        playlist->setCurrentIndex(ui->listWidget->row(item));
        music->setMedia(QUrl::fromLocalFile(list1[ui->listWidget->row(item)]));
        music->play();
    });
    connect(ui->pushButton_7,&QPushButton::clicked,[=]{
        int i = list1.count();
        QStringList list=QFileDialog::getOpenFileNames(this,QString::fromLocal8Bit("文件"),"../",QString::fromLocal8Bit("音频文件(*.mp3)"));
        //用个文件登陆框来选择.mp3文件，包括多个文件

        if(!list.isEmpty()){
            for(int a = 0;a < list.size();a++)
            {
                list1.push_back(list[a]);
            }
            music->setMedia(QUrl::fromLocalFile(list1.first()));
            ui->listWidget->clear();

            for(int i=0;i<list1.size();i++){

             QString   path=QDir::toNativeSeparators(list1.at(i));

                playlist->addMedia(QUrl::fromLocalFile(path));
                QString Name=path.split("\\").last();
                QListWidgetItem *item = new QListWidgetItem(Name);
                item->setText(Name);

                ui->listWidget->addItem(item);
            }
        }
    });
}





















MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enterEvent(QEvent *event)
{
    QRect rc;
        QRect rect;
        rect = this->geometry();
        rc.setRect(rect.x(),rect.y(),rect.width(),rect.height());
        if(rect.top()<0)
        {
            rect.setX(rc.x());
            rect.setY(0);
            move(rc.x(),-2);
        }

}

void MainWindow::leaveEvent(QEvent *event)
{
    QRect rc;
        QRect rect;
        rect = this->geometry();
        rc.setRect(rect.x(),rect.y(),rect.width(),rect.height());
        if(rect.top()<0)
        {
            move(rc.x(),-rc.height()+2);
        }
}



void MainWindow::mousePressEvent(QMouseEvent *event)
{
    offset=event->globalPos()-pos();
    event->accept();
    if(event->button()==Qt::LeftButton)
    {
        setCursor(Qt::OpenHandCursor);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos()-offset);
    event->accept();
    setCursor(Qt::ClosedHandCursor);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    offset=QPoint();
    event->accept();
    setCursor(Qt::ArrowCursor);
}

void MainWindow::ReadLyric(QString site)
{
    QFile file(site);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->listWidget_2->clear();
        while (!file.atEnd())
        {

            QByteArray line = file.readLine();
            QString str(line);
            QListWidgetItem *item = new QListWidgetItem(str);
            item->setText(str);
            ui->listWidget_2->addItem(item);
        }
        file.close();

    }

}

void MainWindow::on_pushButton_2_clicked()
{

}
QString Time(qint64 time)//转换时间
{
    qint64 seconds=time/1000;
    const qint64 minutes=seconds/60;
    seconds-=minutes*60;
    return QStringLiteral("%1:%2")
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0'));
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QToolBar>
#include <QAction>
#include <QPixmap>
#include <QVideoFrame>
#include <QVideoProbe>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow),
                                          mediaPlayer(new QMediaPlayer(this)),
                                          videoWidget(new QVideoWidget(this)),
                                          videoProbe(new QVideoProbe(this))
{
    ui->setupUi(this);

    initUI(); // 初始化界面

    mediaPlayer->setVideoOutput(videoWidget); // 设置视频输出

    exportSettingsDialog = new exportSettings(nullptr);

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::updatePosition);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);

    // 连接 QVideoProbe 到 QMediaPlayer
    if (videoProbe->setSource(mediaPlayer))
    {
        connect(videoProbe, &QVideoProbe::videoFrameProbed, this, &MainWindow::processVideoFrame);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete exportSettingsDialog;

    // 先删除不依赖于布局的控件
    delete timeLabel;
    delete progressBar;
    delete playPauseButton;
    delete openButton;
    delete videoNameLabel;
    delete exportNameLabel;
    delete exportNameEdit;
    delete takePhotoButton;
    // 后删除视频相关控件
    delete videoWidget;
    delete mediaPlayer;
    delete videoProbe;
}

void MainWindow::initUI()
{
    // 设置主窗口的大小
    setWindowTitle("Video Player");
    resize(800, 600);

    // 创建视频名称标签
    videoNameLabel = new QLabel("No video selected", this);
    videoNameLabel->setAlignment(Qt::AlignCenter);

    // 创建导出项目名称标签和输入框
    exportNameLabel = new QLabel("导出项目名称:", this);
    exportNameEdit = new QLineEdit(this);
    exportNameEdit->setPlaceholderText("请输入导出项目名称");

    // 创建打开视频文件按钮
    openButton = new QPushButton("Open Video", this);
    connect(openButton, &QPushButton::clicked, this, &MainWindow::openVideoFile);

    // 创建视频显示控件
    videoWidget->setMinimumSize(640, 360); // 设置最小尺寸

    // 创建播放/暂停按钮
    playPauseButton = new QPushButton("Play", this);
    connect(playPauseButton, &QPushButton::clicked, this, &MainWindow::togglePlayPause);

    // 创建进度条
    progressBar = new QSlider(Qt::Horizontal, this);
    progressBar->setRange(0, 100);
    connect(progressBar, &QSlider::sliderMoved, this, &MainWindow::setPosition);

    // 创建播放时间标签
    timeLabel = new QLabel("00:00 / 00:00", this);

    // 创建拍照按钮
    takePhotoButton = new QPushButton("拍照", this);
    connect(takePhotoButton, &QPushButton::clicked, this, &MainWindow::takeScreenshot);

    // 创建底部布局
    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(playPauseButton);
    controlLayout->addWidget(progressBar);
    controlLayout->addWidget(timeLabel);

    // 创建布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(videoNameLabel);
    QHBoxLayout *exportNameLayout = new QHBoxLayout;
    exportNameLayout->addWidget(exportNameLabel);
    exportNameLayout->addWidget(exportNameEdit);
    mainLayout->addLayout(exportNameLayout);
    mainLayout->addWidget(videoWidget);
    mainLayout->addWidget(openButton);
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(takePhotoButton);

    // 创建一个中央控件并设置布局
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 创建工具栏
    QToolBar *toolBar = addToolBar("Tools");

    QAction *exportSettingsAction = new QAction("Export Settings", this);
    connect(exportSettingsAction, &QAction::triggered, this, &MainWindow::openExportSettings);
    toolBar->addAction(exportSettingsAction);

    QAction *exportVideoAction = new QAction("Export Video", this);
    connect(exportVideoAction, &QAction::triggered, this, &MainWindow::exportVideo);
    toolBar->addAction(exportVideoAction);
}

void MainWindow::openVideoFile()
{
    // 打开文件对话框选择视频文件
    QString fileName = QFileDialog::getOpenFileName(this, "Open Video", "", "Video Files (*.mp4 *.avi *.mkv)");
    if (!fileName.isEmpty())
    {
        mediaPlayer->setMedia(QUrl::fromLocalFile(fileName)); // 设置视频文件路径
        mediaPlayer->play();                                  // 播放视频

        videoNameLabel->setText(fileName); // 显示视频文件名
    }
}

void MainWindow::openExportSettings()
{
    // 实现导出设置功能
    // 这里只是一个示例，实际功能可以根据需求实现
    exportSettingsDialog->show();
    qDebug("Export settings clicked!");
}

void MainWindow::exportVideo()
{
    // 实现导出视频功能
    // 这里只是一个示例，实际功能可以根据需求实现
    qDebug("Export video clicked!");
}

void MainWindow::togglePlayPause()
{
    if (mediaPlayer->state() == QMediaPlayer::PlayingState)
    {
        mediaPlayer->pause();
        playPauseButton->setText("Play");
    }
    else
    {
        mediaPlayer->play();
        playPauseButton->setText("Pause");
    }
}

void MainWindow::setPosition(int position)
{
    mediaPlayer->setPosition(position);
}

void MainWindow::updatePosition(qint64 position)
{
    progressBar->setValue(position);
    updateDurationInfo(position / 1000);
}

void MainWindow::updateDuration(qint64 duration)
{
    progressBar->setRange(0, duration);
    updateDurationInfo(duration / 1000);
}

void MainWindow::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || mediaPlayer->duration())
    {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
                          currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((mediaPlayer->duration() / 3600) % 60,
                        (mediaPlayer->duration() / 60) % 60,
                        mediaPlayer->duration() % 60,
                        (mediaPlayer->duration() * 1000) % 1000);
        QString format = "mm:ss";
        if (mediaPlayer->duration() > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    timeLabel->setText(tStr);
}

void MainWindow::takeScreenshot()
{
    if (!mediaPlayer->isVideoAvailable())
    {
        return;
    }

    // 获取当前视频帧
    qDebug() << realFrame.size();

    // 获取导出路径和项目名称
    QString exportPath = exportSettingsDialog->getExportPath();
    QString exportName = exportNameEdit->text();

    // 确保导出目录存在
    QDir dir(exportPath);
    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    // 生成文件名
    QString fileName;
    if (exportName.isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入导出项目名称"));
        return;
    }
    else
    {
        fileName = QString("%1/%2/%3.jpg")
                       .arg(exportPath)
                       .arg(exportName)
                       .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    }

    // 判断图像是否为正常图像
    if (realFrame.size().isEmpty() || realFrame.isNull())
    {
        statusBar()->showMessage(tr("无效的图像"), 3000);
        return;
    }

    // 确保导出项目文件夹存在
    QDir projectDir(QString("%1/%2").arg(exportPath).arg(exportName));
    if (!projectDir.exists())
    {
        projectDir.mkpath(".");
    }

    // 保存截图
    if (realFrame.save(fileName))
    {
        statusBar()->showMessage(tr("截图已保存到: %1").arg(fileName), 3000);
    }
    else
    {
        statusBar()->showMessage(tr("截图保存失败"), 3000);
    }
}

// 处理视频帧
void MainWindow::processVideoFrame(const QVideoFrame &frame)
{
    QVideoFrame cloneFrame(frame);
    cloneFrame.map(QAbstractVideoBuffer::ReadOnly);

    QImage image(cloneFrame.bits(),
                 cloneFrame.width(),
                 cloneFrame.height(),
                 QVideoFrame::imageFormatFromPixelFormat(cloneFrame.pixelFormat()));

    cloneFrame.unmap();

    realFrame = image.copy();

    // // 保存或处理 QImage
    // // 例如：保存到文件
    // QString fileName = "screenshot.png";
    // image.save(fileName);
}

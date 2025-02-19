/************************************************************
 * Copyright 2025 LiuJiaLe
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * 文件: mainwindow.cpp
 *
 * 模块描述:
 *   该模块负责视频播放器的主窗口实现，包括视频播放、截图和导出功能。
 *
 * 主要功能:
 *   1. 视频文件的打开和播放
 *   2. 视频播放控制（播放/暂停）
 *   3. 视频截图和导出功能
 *
 * 函数列表:
 *   1. MainWindow                - 构造函数，初始化UI和信号连接
 *   2. ~MainWindow               - 析构函数，释放资源
 *   3. initUI                    - 初始化用户界面
 *   4. openVideoFile             - 打开视频文件
 *   5. openExportSettings        - 打开导出设置对话框
 *   6. exportVideo               - 导出视频
 *   7. togglePlayPause           - 切换播放/暂停状态
 *   8. setPosition               - 设置视频播放位置
 *   9. updatePosition            - 更新播放位置
 *   10. updateDuration           - 更新视频时长
 *   11. updateDurationInfo       - 更新播放时间信息
 *   12. takeScreenshot           - 截取视频截图
 *   13. processVideoFrame        - 处理视频帧
 *
 * 版本历史:
 *   - 版本 1.0 (2025-02-05) - LiuJiaLe
 *     * 初始版本创建
 ***********************************************************/
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
#include <QThread>

/***********************************************************
 * 函数名称: MainWindow
 * 函数功能: 主窗口类的构造函数
 * 参数说明:
 *   parent - 父窗口指针,默认为nullptr
 * 返回值: 无
 * 备注: 初始化UI界面,创建并连接媒体播放器相关组件
 ***********************************************************/
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

/***********************************************************
 * 函数名称: ~MainWindow
 * 函数功能: 主窗口类的析构函数
 * 参数说明: 无
 * 返回值: 无
 * 备注: 释放资源,删除相关控件
 ***********************************************************/
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

/***********************************************************
 * 函数名称: initUI
 * 函数功能: 初始化用户界面
 * 参数说明: 无
 * 返回值: 无
 * 备注: 设置主窗口大小,标题,创建控件并设置布局
 ***********************************************************/
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

/***********************************************************
 * 函数名称: openVideoFile
 * 函数功能: 打开视频文件
 * 参数说明: 无
 * 返回值: 无
 * 备注: 打开文件对话框选择视频文件
 ***********************************************************/
void MainWindow::openVideoFile()
{
    // 打开文件对话框选择视频文件
    QString fileName = QFileDialog::getOpenFileName(this, "Open Video", "", "Video Files (*.mp4 *.avi *.mkv)");
    if (!fileName.isEmpty())
    {
        mediaPlayer->setMedia(QUrl::fromLocalFile(fileName)); // 设置视频文件路径

        mediaPlayer->play(); // 播放视频

        videoNameLabel->setText(fileName); // 显示视频文件名
    }
}

/***********************************************************
 * 函数名称: openExportSettings
 * 函数功能: 打开导出设置对话框
 * 参数说明: 无
 * 返回值: 无
 * 备注: 打开导出设置对话框
 ***********************************************************/
void MainWindow::openExportSettings()
{
    // 实现导出设置功能
    // 这里只是一个示例，实际功能可以根据需求实现
    exportSettingsDialog->show();
    qDebug("Export settings clicked!");
}

/***********************************************************
 * 函数名称: exportVideo
 * 函数功能: 导出视频
 * 参数说明: 无
 * 返回值: 无
 * 备注: 导出视频
 ***********************************************************/
void MainWindow::exportVideo()
{
    // 实现导出视频功能
    // 这里只是一个示例，实际功能可以根据需求实现
    qDebug("Export video clicked!");
}

/***********************************************************
 * 函数名称: togglePlayPause
 * 函数功能: 切换播放/暂停状态
 * 参数说明: 无
 * 返回值: 无
 * 备注: 切换播放/暂停状态
 ***********************************************************/
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

/***********************************************************
 * 函数名称: setPosition
 * 函数功能: 设置视频播放位置
 * 参数说明: position - 视频播放位置
 * 返回值: 无
 * 备注: 设置视频播放位置
 ***********************************************************/
void MainWindow::setPosition(int position)
{
    mediaPlayer->setPosition(position);
}

/***********************************************************
 * 函数名称: updatePosition
 * 函数功能: 更新视频播放位置
 * 参数说明: position - 视频播放位置
 * 返回值: 无
 * 备注: 更新视频播放位置
 ***********************************************************/
void MainWindow::updatePosition(qint64 position)
{
    progressBar->setValue(position);
    updateDurationInfo(position / 1000);
}

/***********************************************************
 * 函数名称: updateDuration
 * 函数功能: 更新视频时长
 * 参数说明: duration - 视频时长
 * 返回值: 无
 * 备注: 更新视频时长
 ***********************************************************/
void MainWindow::updateDuration(qint64 duration)
{
    progressBar->setRange(0, duration);
    updateDurationInfo(duration / 1000);
}

/***********************************************************
 * 函数名称: updateDurationInfo
 * 函数功能: 更新视频时长信息
 * 参数说明: currentInfo - 当前视频时长
 * 返回值: 无
 * 备注: 更新视频时长信息
 ***********************************************************/
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

/***********************************************************
 * 函数名称: takeScreenshot
 * 函数功能: 拍照
 * 参数说明: 无
 * 返回值: 无
 * 备注: 拍照
 ***********************************************************/
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

/***********************************************************
 * 函数名称: processVideoFrame
 * 函数功能: 处理视频帧
 * 参数说明: frame - 视频帧
 * 返回值: 无
 * 备注: 处理视频帧
 ***********************************************************/
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

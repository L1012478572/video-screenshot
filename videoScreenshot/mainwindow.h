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
 * 文件: mainwindow.h
 *
 * 模块描述:
 *   该模块定义了视频播放器的主窗口类，包括视频播放、截图和导出功能的接口。
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QToolBar>
#include <QAction>
#include <QSlider>
#include <QTime>
#include <QVideoProbe>

#include "exportsettings.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openVideoFile();                             // 打开视频文件
    void openExportSettings();                        // 打开导出设置对话框
    void exportVideo();                               // 导出视频
    void togglePlayPause();                           // 切换播放/暂停状态
    void setPosition(int position);                   // 设置视频播放位置
    void updatePosition(qint64 position);             // 更新播放位置
    void updateDuration(qint64 duration);             // 更新视频时长
    void updateDurationInfo(qint64 currentInfo);      // 更新播放时间信息
    void takeScreenshot();                            // 截取视频截图
    void processVideoFrame(const QVideoFrame &frame); // 处理视频帧

private:
    Ui::MainWindow *ui;
    exportSettings *exportSettingsDialog; // 导出设置对话框

    void initUI(); // 初始化用户界面

    QVideoProbe *videoProbe;
    QMediaPlayer *mediaPlayer;    // 媒体播放器
    QVideoWidget *videoWidget;    // 视频显示控件
    QLabel *videoNameLabel;       // 显示视频名称的标签
    QPushButton *openButton;      // 打开视频文件按钮
    QPushButton *playPauseButton; // 播放/暂停按钮
    QSlider *progressBar;         // 进度条
    QLabel *timeLabel;            // 播放时间标签
    QLabel *exportNameLabel;      // 导出项目名称标签
    QLineEdit *exportNameEdit;    // 导出项目名称输入框
    QPushButton *takePhotoButton; // 拍照按钮

    QImage realFrame; // 当前视频帧图像
};

#endif // MAINWINDOW_H

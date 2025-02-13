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
    void openVideoFile();                        // 打开视频文件
    void openExportSettings();                   // 导出设置
    void exportVideo();                          // 导出视频
    void togglePlayPause();                      // 播放/暂停视频
    void setPosition(int position);              // 设置视频位置
    void updatePosition(qint64 position);        // 更新进度条位置
    void updateDuration(qint64 duration);        // 更新视频总时长
    void updateDurationInfo(qint64 currentInfo); // 更新播放时间信息
    void takeScreenshot();                       // 拍摄视频截图
    void processVideoFrame(const QVideoFrame &frame);

private:
    Ui::MainWindow *ui;
    exportSettings *exportSettingsDialog; // 改用更明确的变量名

    void initUI(); // 初始化界面

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

    QImage realFrame;
};

#endif // MAINWINDOW_H

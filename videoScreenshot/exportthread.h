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
 * 文件: exportthread.h
 *
 * 模块描述:
 *   该模块定义了视频导出的线程类，负责处理视频帧的导出操作。
 *
 * 主要功能:
 *   1. 设置视频文件路径
 *   2. 设置导出路径和名称
 *   3. 设置导出模式和参数
 *   4. 处理视频帧并导出
 *
 * 函数列表:
 *   1. exportThread              - 构造函数，初始化线程
 *   2. ~exportThread             - 析构函数，释放资源
 *   3. setVideoFile              - 设置视频文件路径
 *   4. setExportPath             - 设置导出路径
 *   5. setExportName             - 设置导出名称
 *   6. setExportMode             - 设置导出模式
 *   7. setInterval               - 设置间隔帧数
 *   8. setRandomCount            - 设置随机截图数
 *   9. setOrthogonalCount        - 设置正交分布数
 *   10. run                      - 线程运行函数，处理视频导出
 *   11. processVideoFrame        - 处理视频帧
 *
 * 版本历史:
 *   - 版本 1.0 (2025-02-05) - LiuJiaLe
 *     * 初始版本创建
 ***********************************************************/

#ifndef EXPORTTHREAD_H
#define EXPORTTHREAD_H

#include <QThread>
#include <QMediaPlayer>
#include <QVideoProbe>
#include <QImage>
#include <QString>

class exportThread : public QThread
{
    Q_OBJECT

public:
    explicit exportThread(QObject *parent = nullptr);
    ~exportThread();

    void setVideoFile(const QString &filePath); // 设置视频文件路径
    void setExportPath(const QString &path);    // 设置导出路径
    void setExportName(const QString &name);    // 设置导出名称
    void setExportMode(int mode);               // 设置导出模式
    void setInterval(int interval);             // 设置间隔帧数
    void setRandomCount(int count);             // 设置随机截图数
    void setOrthogonalCount(int count);         // 设置正交分布数

protected:
    void run() override; // 线程运行函数，处理视频导出

private slots:
    void processVideoFrame(const QVideoFrame &frame); // 处理视频帧

private:
    QMediaPlayer *mediaPlayer; // 媒体播放器
    QVideoProbe *videoProbe;   // 视频探测器
    QImage currentFrame;       // 当前视频帧

    QString videoFilePath; // 视频文件路径
    QString exportPath;    // 导出路径
    QString exportName;    // 导出名称
    int exportMode;        // 导出模式
    int interval;          // 间隔帧数
    int randomCount;       // 随机截图数
    int orthogonalCount;   // 正交分布数
};

#endif // EXPORTTHREAD_H
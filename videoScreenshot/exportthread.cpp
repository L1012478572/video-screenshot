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
 * 文件: exportthread.cpp
 *
 * 模块描述:
 *   该模块实现了视频导出的线程类，负责处理视频帧的导出操作。
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

#include "exportthread.h"
#include <QDir>
#include <QDateTime>
#include <QDebug>

/***********************************************************
 * 函数名称: exportThread
 * 函数功能: 导出线程类的构造函数
 * 参数说明:
 *   parent - 父对象指针,默认为nullptr
 * 返回值: 无
 * 备注: 初始化媒体播放器和视频探测器
 ***********************************************************/
exportThread::exportThread(QObject *parent) : QThread(parent),
                                              mediaPlayer(new QMediaPlayer(this)),
                                              videoProbe(new QVideoProbe(this))
{
  // 连接 QVideoProbe 到 QMediaPlayer
  if (videoProbe->setSource(mediaPlayer))
  {
    connect(videoProbe, &QVideoProbe::videoFrameProbed, this, &exportThread::processVideoFrame);
  }
}

/***********************************************************
 * 函数名称: ~exportThread
 * 函数功能: 导出线程类的析构函数
 * 参数说明: 无
 * 返回值: 无
 * 备注: 释放媒体播放器和视频探测器资源
 ***********************************************************/
exportThread::~exportThread()
{
  delete mediaPlayer;
  delete videoProbe;
}

/***********************************************************
 * 函数名称: run
 * 函数功能: 线程运行函数，处理视频导出
 * 参数说明: 无
 * 返回值: 无
 * 备注: 设置视频源并根据导出模式处理视频帧
 ***********************************************************/
void exportThread::run()
{
  // 设置视频源
  mediaPlayer->setMedia(QUrl::fromLocalFile(videoFilePath));

  // 等待媒体加载完成
  while (mediaPlayer->mediaStatus() != QMediaPlayer::LoadedMedia)
  {
    QThread::msleep(100);
  }

  // 平均间隔导出
  if (exportMode == 0)
  {
  }
  // 随机导出
  else if (exportMode == 1)
  {
  }
  // 正交分布导出
  else if (exportMode == 2)
  {
  }
}

/***********************************************************
 * 函数名称: setVideoFile
 * 函数功能: 设置视频文件路径
 * 参数说明:
 *   filePath - 视频文件路径
 * 返回值: 无
 * 备注: 设置视频文件路径
 ***********************************************************/
void exportThread::setVideoFile(const QString &filePath)
{
  videoFilePath = filePath;
}

/***********************************************************
 * 函数名称: setExportPath
 * 函数功能: 设置导出路径
 * 参数说明:
 *   path - 导出路径
 * 返回值: 无
 * 备注: 设置导出路径
 ***********************************************************/
void exportThread::setExportPath(const QString &path)
{
  exportPath = path;
}

/***********************************************************
 * 函数名称: setExportName
 * 函数功能: 设置导出名称
 * 参数说明:
 *   name - 导出名称
 * 返回值: 无
 * 备注: 设置导出名称
 ***********************************************************/
void exportThread::setExportName(const QString &name)
{
  exportName = name;
}

/***********************************************************
 * 函数名称: setExportMode
 * 函数功能: 设置导出模式
 * 参数说明:
 *   mode - 导出模式
 * 返回值: 无
 * 备注: 设置导出模式
 ***********************************************************/
void exportThread::setExportMode(int mode)
{
  exportMode = mode;
}

/***********************************************************
 * 函数名称: setInterval
 * 函数功能: 设置间隔帧数
 * 参数说明:
 *   value - 间隔帧数
 * 返回值: 无
 * 备注: 设置间隔帧数
 ***********************************************************/
void exportThread::setInterval(int value)
{
  interval = value;
}

/***********************************************************
 * 函数名称: setRandomCount
 * 函数功能: 设置随机截图数
 * 参数说明:
 *   count - 随机截图数
 * 返回值: 无
 * 备注: 设置随机截图数
 ***********************************************************/
void exportThread::setRandomCount(int count)
{
  randomCount = count;
}

/***********************************************************
 * 函数名称: setOrthogonalCount
 * 函数功能: 设置正交分布数
 * 参数说明:
 *   count - 正交分布数
 * 返回值: 无
 * 备注: 设置正交分布数
 ***********************************************************/
void exportThread::setOrthogonalCount(int count)
{
  orthogonalCount = count;
}

/***********************************************************
 * 函数名称: processVideoFrame
 * 函数功能: 处理视频帧
 * 参数说明:
 *   frame - 视频帧
 * 返回值: 无
 * 备注: 处理视频帧并根据导出设置保存
 ***********************************************************/
void exportThread::processVideoFrame(const QVideoFrame &frame)
{
  QVideoFrame cloneFrame(frame);
  cloneFrame.map(QAbstractVideoBuffer::ReadOnly);

  QImage image(cloneFrame.bits(),
               cloneFrame.width(),
               cloneFrame.height(),
               QVideoFrame::imageFormatFromPixelFormat(cloneFrame.pixelFormat()));

  cloneFrame.unmap();

  currentFrame = image.copy();

  // 根据导出设置保存图像
  // 例如：保存到文件
  QString fileName = QString("%1/%2/%3.jpg")
                         .arg(exportPath)
                         .arg(exportName)
                         .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
  if (currentFrame.save(fileName))
  {
    qDebug() << "Frame saved to:" << fileName;
  }
  else
  {
    qDebug() << "Failed to save frame.";
  }
}

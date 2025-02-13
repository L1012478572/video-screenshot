#include "exportthread.h"

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

exportThread::~exportThread()
{
  delete mediaPlayer;
  delete videoProbe;
}

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
}

void exportThread::setVideoFile(const QString &filePath)
{
  videoFilePath = filePath;
}

void exportThread::setExportPath(const QString &path)
{
  exportPath = path;
}

void exportThread::setExportName(const QString &name)
{
  exportName = name;
}

void exportThread::setExportMode(int mode)
{
  exportMode = mode;
}

void exportThread::setInterval(int value)
{
  interval = value;
}

void exportThread::setRandomCount(int count)
{
  randomCount = count;
}

void exportThread::setOrthogonalCount(int count)
{
  orthogonalCount = count;
}

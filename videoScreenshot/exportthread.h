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

    // 设置导出参数
    void setVideoFile(const QString &filePath);
    void setExportPath(const QString &path);
    void setExportName(const QString &name);
    void setExportMode(int mode);
    void setInterval(int interval);
    void setRandomCount(int count);
    void setOrthogonalCount(int count);

protected:
    void run() override;

private slots:
    void processVideoFrame(const QVideoFrame &frame);

private:
    QMediaPlayer *mediaPlayer;
    QVideoProbe *videoProbe;
    QImage currentFrame;

    QString videoFilePath;
    QString exportPath;
    QString exportName;
    int exportMode;
    int interval;
    int randomCount;
    int orthogonalCount;
};

#endif // EXPORTTHREAD_H
#ifndef EXPORTSETTINGS_H
#define EXPORTSETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

namespace Ui
{
    class exportSettings;
}

class exportSettings : public QWidget
{
    Q_OBJECT

public:
    explicit exportSettings(QWidget *parent = nullptr);
    ~exportSettings();

    enum ExportMode
    {
        EQUAL_INTERVAL = 0,
        RANDOM,
        ORTHOGONAL
    };

    QString getExportPath() { return lineEditPath->text(); }

private:
    void initUI();
    void loadSettings();
    void saveSettings();

private slots:
    void onExportModeChanged(int index);
    void onPathSelectClicked();

private:
    Ui::exportSettings *ui;
    QSettings *settings;

    QVBoxLayout *mainLayout;
    QHBoxLayout *pathLayout;
    QLabel *pathLabel;
    QLineEdit *lineEditPath;
    QHBoxLayout *modeLayout;
    QLabel *modeLabel;
    QComboBox *comboBoxMode;
    QLabel *labelInterval;
    QSpinBox *spinBoxInterval;
    QLabel *labelRandomCount;
    QSpinBox *spinBoxRandomCount;
    QLabel *labelOrthogonalCount;
    QSpinBox *spinBoxOrthogonalCount;
    QPushButton *pushButtonPath;

    // 默认参数
    const QString DEFAULT_EXPORT_PATH = QDir::homePath() + "/Pictures/Screenshots";
    const int DEFAULT_INTERVAL = 30;
    const int DEFAULT_RANDOM_COUNT = 10;
    const int DEFAULT_ORTHOGONAL_COUNT = 10;
};

#endif // EXPORTSETTINGS_H

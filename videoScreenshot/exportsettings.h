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
 * 文件: exportsettings.h
 *
 * 模块描述:
 *   该模块定义了导出设置的界面类，包括路径选择和导出模式的配置接口。
 *
 * 主要功能:
 *   1. 设置导出路径
 *   2. 选择导出模式
 *   3. 保存和加载导出设置
 *
 * 函数列表:
 *   1. exportSettings            - 构造函数，初始化UI和加载设置
 *   2. ~exportSettings           - 析构函数，保存设置并释放资源
 *   3. initUI                    - 初始化用户界面
 *   4. loadSettings              - 加载保存的设置
 *   5. saveSettings              - 保存当前设置
 *   6. onExportModeChanged       - 根据导出模式更新UI
 *   7. onPathSelectClicked       - 选择导出路径
 *
 * 版本历史:
 *   - 版本 1.0 (2025-02-05) - LiuJiaLe
 *     * 初始版本创建
 ***********************************************************/

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

    QString getExportPath() { return lineEditPath->text(); } // 获取导出路径

private:
    void initUI();       // 初始化用户界面
    void loadSettings(); // 加载保存的设置
    void saveSettings(); // 保存当前设置

private slots:
    void onExportModeChanged(int index); // 根据导出模式更新UI
    void onPathSelectClicked();          // 选择导出路径

private:
    Ui::exportSettings *ui;
    QSettings *settings;

    QVBoxLayout *mainLayout;          // 主布局
    QHBoxLayout *pathLayout;          // 路径布局
    QLabel *pathLabel;                // 路径标签
    QLineEdit *lineEditPath;          // 路径输入框
    QHBoxLayout *modeLayout;          // 模式布局
    QLabel *modeLabel;                // 模式标签
    QComboBox *comboBoxMode;          // 模式选择框
    QLabel *labelInterval;            // 间隔帧数标签
    QSpinBox *spinBoxInterval;        // 间隔帧数选择框
    QLabel *labelRandomCount;         // 随机截图数标签
    QSpinBox *spinBoxRandomCount;     // 随机截图数选择框
    QLabel *labelOrthogonalCount;     // 正交分布数标签
    QSpinBox *spinBoxOrthogonalCount; // 正交分布数选择框
    QPushButton *pushButtonPath;      // 选择路径按钮

    // 默认参数
    const QString DEFAULT_EXPORT_PATH = QDir::homePath() + "/Pictures/Screenshots";
    const int DEFAULT_INTERVAL = 30;
    const int DEFAULT_RANDOM_COUNT = 10;
    const int DEFAULT_ORTHOGONAL_COUNT = 10;
};

#endif // EXPORTSETTINGS_H
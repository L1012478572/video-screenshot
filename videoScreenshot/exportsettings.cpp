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
 * 文件: exportsettings.cpp
 *
 * 模块描述:
 *   该模块负责导出设置的界面实现，包括路径选择和导出模式的配置。
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

#include "exportsettings.h"
#include "ui_exportsettings.h"
#include <QFileDialog>
#include <QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

/***********************************************************
 * 函数名称: exportSettings
 * 函数功能: 导出设置类的构造函数
 * 参数说明:
 *   parent - 父窗口指针,默认为nullptr
 * 返回值: 无
 * 备注: 初始化UI界面,加载保存的设置
 ***********************************************************/
exportSettings::exportSettings(QWidget *parent) : QWidget(parent),
                                                  ui(new Ui::exportSettings)
{
    ui->setupUi(this);
    settings = new QSettings("VideoScreenshot", "ExportSettings");
    initUI();
    loadSettings();
}

/***********************************************************
 * 函数名称: ~exportSettings
 * 函数功能: 导出设置类的析构函数
 * 参数说明: 无
 * 返回值: 无
 * 备注: 保存设置并释放资源
 ***********************************************************/
exportSettings::~exportSettings()
{
    saveSettings();
    // 先删除不依赖于布局的控件
    delete settings;
    delete pathLabel;
    delete lineEditPath;
    delete modeLabel;
    delete comboBoxMode;
    delete labelInterval;
    delete spinBoxInterval;
    delete labelRandomCount;
    delete spinBoxRandomCount;
    delete labelOrthogonalCount;
    delete spinBoxOrthogonalCount;
    delete pushButtonPath;

    // 后删除布局,从内到外
    delete pathLayout;
    delete modeLayout;
    delete mainLayout;

    delete ui;
}

/***********************************************************
 * 函数名称: initUI
 * 函数功能: 初始化用户界面
 * 参数说明: 无
 * 返回值: 无
 * 备注: 创建并设置布局和控件
 ***********************************************************/
void exportSettings::initUI()
{
    // 添加layout
    // 创建主布局
    mainLayout = new QVBoxLayout(this);

    // 创建导出路径布局
    pathLayout = new QHBoxLayout();
    pathLabel = new QLabel(tr("导出路径:"), this);
    lineEditPath = new QLineEdit(this);
    pathLayout->addWidget(pathLabel);
    pathLayout->addWidget(lineEditPath);

    // 创建导出模式布局
    modeLayout = new QHBoxLayout();
    modeLabel = new QLabel(tr("导出模式:"), this);
    modeLayout->addWidget(modeLabel);

    // 添加到主布局
    mainLayout->addLayout(pathLayout);
    mainLayout->addLayout(modeLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
    // 设置导出模式选项
    comboBoxMode = new QComboBox(this);
    comboBoxMode->addItem(tr("等间距导出"), EQUAL_INTERVAL);
    comboBoxMode->addItem(tr("随机导出"), RANDOM);
    comboBoxMode->addItem(tr("正交分布导出"), ORTHOGONAL);
    // 添加到布局中
    modeLayout->addWidget(comboBoxMode);

    // 连接信号和槽
    connect(comboBoxMode, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &exportSettings::onExportModeChanged);

    // 创建选择路径按钮
    pushButtonPath = new QPushButton(tr("选择路径"), this);
    pathLayout->addWidget(pushButtonPath);
    connect(pushButtonPath, &QPushButton::clicked,
            this, &exportSettings::onPathSelectClicked);

    // 创建并设置数值范围
    labelInterval = new QLabel(tr("间隔帧数:"), this);
    spinBoxInterval = new QSpinBox(this);
    spinBoxInterval->setRange(1, 9999);
    modeLayout->addWidget(labelInterval);
    modeLayout->addWidget(spinBoxInterval);

    labelRandomCount = new QLabel(tr("随机截图数:"), this);
    spinBoxRandomCount = new QSpinBox(this);
    spinBoxRandomCount->setRange(1, 9999);
    modeLayout->addWidget(labelRandomCount);
    modeLayout->addWidget(spinBoxRandomCount);

    labelOrthogonalCount = new QLabel(tr("正交分布数:"), this);
    spinBoxOrthogonalCount = new QSpinBox(this);
    spinBoxOrthogonalCount->setRange(1, 9999);
    modeLayout->addWidget(labelOrthogonalCount);
    modeLayout->addWidget(spinBoxOrthogonalCount);
}

/***********************************************************
 * 函数名称: loadSettings
 * 函数功能: 加载保存的设置
 * 参数说明: 无
 * 返回值: 无
 * 备注: 从QSettings加载设置并应用到UI
 ***********************************************************/
void exportSettings::loadSettings()
{
    // 加载保存的设置
    QString exportPath = settings->value("exportPath", DEFAULT_EXPORT_PATH).toString();
    int exportMode = settings->value("exportMode", EQUAL_INTERVAL).toInt();
    int interval = settings->value("interval", DEFAULT_INTERVAL).toInt();
    int randomCount = settings->value("randomCount", DEFAULT_RANDOM_COUNT).toInt();
    int orthogonalCount = settings->value("orthogonalCount", DEFAULT_ORTHOGONAL_COUNT).toInt();

    // 应用设置到UI
    lineEditPath->setText(exportPath);
    comboBoxMode->setCurrentIndex(exportMode);
    spinBoxInterval->setValue(interval);
    spinBoxRandomCount->setValue(randomCount);
    spinBoxOrthogonalCount->setValue(orthogonalCount);

    // 根据当前模式显示/隐藏相关控件
    onExportModeChanged(exportMode);
}

/***********************************************************
 * 函数名称: saveSettings
 * 函数功能: 保存当前设置
 * 参数说明: 无
 * 返回值: 无
 * 备注: 将当前UI设置保存到QSettings
 ***********************************************************/
void exportSettings::saveSettings()
{
    // 保存当前设置
    settings->setValue("exportPath", lineEditPath->text());
    settings->setValue("exportMode", comboBoxMode->currentIndex());
    settings->setValue("interval", spinBoxInterval->value());
    settings->setValue("randomCount", spinBoxRandomCount->value());
    settings->setValue("orthogonalCount", spinBoxOrthogonalCount->value());
}

/***********************************************************
 * 函数名称: onExportModeChanged
 * 函数功能: 根据导出模式更新UI
 * 参数说明: index - 当前选择的导出模式索引
 * 返回值: 无
 * 备注: 显示或隐藏与导出模式相关的控件
 ***********************************************************/
void exportSettings::onExportModeChanged(int index)
{
    // 根据选择的模式显示/隐藏相关控件
    spinBoxInterval->setVisible(index == EQUAL_INTERVAL);
    labelInterval->setVisible(index == EQUAL_INTERVAL);

    spinBoxRandomCount->setVisible(index == RANDOM);
    labelRandomCount->setVisible(index == RANDOM);

    spinBoxOrthogonalCount->setVisible(index == ORTHOGONAL);
    labelOrthogonalCount->setVisible(index == ORTHOGONAL);
}

/***********************************************************
 * 函数名称: onPathSelectClicked
 * 函数功能: 选择导出路径
 * 参数说明: 无
 * 返回值: 无
 * 备注: 打开目录选择对话框并设置导出路径
 ***********************************************************/
void exportSettings::onPathSelectClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("选择导出目录"),
                                                    lineEditPath->text(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty())
    {
        lineEditPath->setText(dir);
    }
}

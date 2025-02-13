#include "exportsettings.h"
#include "ui_exportsettings.h"
#include <QFileDialog>
#include <QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

exportSettings::exportSettings(QWidget *parent) : QWidget(parent),
                                                  ui(new Ui::exportSettings)
{
    ui->setupUi(this);
    settings = new QSettings("VideoScreenshot", "ExportSettings");
    initUI();
    loadSettings();
}

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

void exportSettings::saveSettings()
{
    // 保存当前设置
    settings->setValue("exportPath", lineEditPath->text());
    settings->setValue("exportMode", comboBoxMode->currentIndex());
    settings->setValue("interval", spinBoxInterval->value());
    settings->setValue("randomCount", spinBoxRandomCount->value());
    settings->setValue("orthogonalCount", spinBoxOrthogonalCount->value());
}

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

#include "exportsettings.h"
#include "ui_exportsettings.h"

exportSettings::exportSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::exportSettings)
{
    ui->setupUi(this);
}

exportSettings::~exportSettings()
{
    delete ui;
}

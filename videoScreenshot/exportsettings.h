#ifndef EXPORTSETTINGS_H
#define EXPORTSETTINGS_H

#include <QWidget>

namespace Ui {
class exportSettings;
}

class exportSettings : public QWidget
{
    Q_OBJECT

public:
    explicit exportSettings(QWidget *parent = nullptr);
    ~exportSettings();

private:
    Ui::exportSettings *ui;
};

#endif // EXPORTSETTINGS_H

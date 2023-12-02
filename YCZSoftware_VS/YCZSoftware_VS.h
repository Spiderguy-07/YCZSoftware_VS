#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_YCZSoftware_VS.h"

class YCZSoftware_VS : public QMainWindow
{
    Q_OBJECT

public:
    YCZSoftware_VS(QWidget *parent = nullptr);
    ~YCZSoftware_VS();

private:
    Ui::YCZSoftware_VSClass ui;
};

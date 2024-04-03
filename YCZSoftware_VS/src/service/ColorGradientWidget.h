#pragma once

#include <QMainWindow>
#include <QColorDialog>
#include "ui_ColorGradientWidget.h"
#include "ColorGradientPreview.h"
#include "ColorWidget.h"

class ColorGradientWidget : public QMainWindow
{
	Q_OBJECT

signals:
	void sendGradColor(QColor begin_color, QColor end_color, int steps, QString Field);


public:
	ColorGradientWidget(QStringList fields, QWidget *parent = nullptr);
	~ColorGradientWidget();

private:
	Ui::ColorGradientWidgetClass ui;

	//ColorGradientPreview* preview_color = new ColorGradientPreview();

	QColor begin_color;
	QColor end_color;
	int num_color;
	QStringList fields;

private slots:

	void onActionBegin();
	void onActionEnd();
	void onActionNum();

	void onActionConfirm();
	void onActionCancel();
};

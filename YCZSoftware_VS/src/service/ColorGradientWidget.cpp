#include "ColorGradientWidget.h"


ColorGradientWidget::ColorGradientWidget(QStringList fields, QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	begin_color = QColor(Qt::blue);
	end_color = QColor(Qt::red);
	ui.begincolor_widget->setColor(begin_color);
	ui.endcolor_widget->setColor(end_color);
	ui.PreviewWidget->setGradientColors(begin_color, end_color);
	ui.PreviewWidget->setNumSteps(10);
	ui.num_Color->setValue(10);

	for (const auto& field : fields) {
		ui.cmbField->addItem(field);
	}

	this->connect(ui.Bt_BeginColor, &QAbstractButton::clicked, this, &ColorGradientWidget::onActionBegin);
	this->connect(ui.Bt_EndColor, &QAbstractButton::clicked, this, &ColorGradientWidget::onActionEnd);
	connect(ui.num_Color, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this, [this]() { this->onActionNum(); });

	this->connect(ui.bt_confirm, &QAbstractButton::clicked, this, &ColorGradientWidget::onActionConfirm);
	this->connect(ui.bt_cancel, &QAbstractButton::clicked, this, &ColorGradientWidget::onActionCancel);
}

ColorGradientWidget::~ColorGradientWidget()
{}

void ColorGradientWidget::onActionBegin()
{
	//begin_color = QColorDialog::getColor(Qt::red, this, tr("Color"));
	QColor initialColor = Qt::red;
	QColor chosenColor = QColorDialog::getColor(initialColor, this, tr("Select Color"));
	if (chosenColor.isValid())
	{
		begin_color = chosenColor;
	}
	//ui.lineEdit->setStyleSheet(begin_color);
	ui.PreviewWidget->setGradientColors(begin_color, end_color);
	ui.begincolor_widget->setColor(begin_color);
}

void ColorGradientWidget::onActionEnd()
{
	QColor initialColor = Qt::red;
	QColor chosenColor = QColorDialog::getColor(initialColor, this, tr("Select Color"));
	if (chosenColor.isValid())
	{
		end_color = chosenColor;
	}
	//end_color = QColorDialog::getColor(Qt::red, this, tr("Color"));
	ui.PreviewWidget->setGradientColors(begin_color, end_color);
	ui.endcolor_widget->setColor(end_color);
}

void ColorGradientWidget::onActionNum()
{
	num_color = ui.num_Color->value();
	if (num_color > 1)
	{
		ui.PreviewWidget->setNumSteps(num_color);
	}
}

void ColorGradientWidget::onActionConfirm()
{
	QString Filed = ui.cmbField->currentText();
	emit sendGradColor(begin_color, end_color, num_color, Filed);
	this->close();
}

void ColorGradientWidget::onActionCancel()
{
	this->close();
}

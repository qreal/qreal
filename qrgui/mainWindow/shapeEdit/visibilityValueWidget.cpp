#include "visibilityValueWidget.h"

#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLineEdit>

VisibilityValueWidget::VisibilityValueWidget(QWidget *parent)
	: QWidget(parent), mWidget(0)
{}

VisibilityValueWidget::~VisibilityValueWidget()
{
	delete mWidget;
}

void VisibilityValueWidget::setPropertyInfo(const VisibilityConditionsDialog::PropertyInfo &info)
{
	delete mWidget;
	makeWidget(info.type);
	addValues(info.values);
}

void VisibilityValueWidget::setValue(const QString &value)
{
	if (QComboBox *comboBox = dynamic_cast<QComboBox *>(mWidget)) {
		comboBox->setCurrentIndex(comboBox->findText(value));
	} else if (QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(mWidget)) {
		lineEdit->setText(value);
	} else if (QSpinBox *spinBox = dynamic_cast<QSpinBox *>(mWidget)) {
		spinBox->setValue(value.toInt());
	}
}

QString VisibilityValueWidget::value() const
{
	if (QComboBox *comboBox = dynamic_cast<QComboBox *>(mWidget)) {
		return comboBox->currentText();
	}
	if (QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(mWidget)) {
		return lineEdit->text();
	}
	if (QSpinBox *spinBox = dynamic_cast<QSpinBox *>(mWidget)) {
		return QString::number(spinBox->value());
	}
	return QString();
}

void VisibilityValueWidget::makeWidget(VisibilityConditionsDialog::Type type)
{
	switch (type)
	{
		case VisibilityConditionsDialog::Int:
				{
					QSpinBox *spinBox = new QSpinBox(this);
					spinBox->setMinimum(INT_MIN);
					spinBox->setMaximum(INT_MAX);
					mWidget = spinBox;
				}
				break;
		case VisibilityConditionsDialog::String:
				mWidget = new QLineEdit(this);
				break;
		default:
				mWidget = new QComboBox(this);
	}

	mWidget->setGeometry(0, 0, width(), height());
	mWidget->show();
}

void VisibilityValueWidget::addValues(const QStringList &values)
{
	if (QComboBox *widget = dynamic_cast<QComboBox *>(mWidget)) {
		widget->clear();
		widget->addItems(values);
	}
}

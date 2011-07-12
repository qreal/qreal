#include "enumWidget.h"

#include "QLabel"
#include "QGridLayout"

EnumWidget::EnumWidget(Element *element, const QString &propertyName, const QStringList &values, QWidget *parent) :
	QSpinBox(parent),
	Control(propertyName),
	element(element)
{
	qreal curValueWidth;
	qreal maxValueWidth = 0;
	QFontMetrics metrics(font());

	int i = 1;
	foreach (const QString value, values) {
		curValueWidth = metrics.width(value);
		if (curValueWidth > maxValueWidth) {
			maxValueWidth = curValueWidth;
		}
		textToInt.insert(value, i);
		i++;
	}

	setMinimumWidth(minimumWidth() + maxValueWidth);
	setValue(valueFromText(element->logicalProperty(propertyName)));

	connect(
		this, SIGNAL(valueChanged(QString)),
		this, SLOT(setNewValue(QString))
	);
}

QString EnumWidget::textFromValue(int index) const {
	return textToInt.key(index);
}

int EnumWidget::valueFromText(const QString &text) const {
	return textToInt.value(text);
}

void EnumWidget::setNewValue(const QString &text) const {
	element->setLogicalProperty(propertyName, text);
}

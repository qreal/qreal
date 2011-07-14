#include "enumWidget.h"

#include "QLabel"
#include "QGridLayout"

EnumWidget::EnumWidget(Element *element, const QString &propertyName, const QStringList &values, QWidget *parent) :
	QSpinBox(parent),
	EmbeddedControl(propertyName),
	element(element)
{
	qreal curValueWidth;
	qreal maxValueWidth = 0;
	QFontMetrics metrics(font());

	int i = 0;
	foreach (const QString value, values) {
		i++;
		curValueWidth = metrics.width(value);
		if (curValueWidth > maxValueWidth) {
			maxValueWidth = curValueWidth;
		}
		possibleValuesToInt.insert(value, i);
	}

	setMinimum(1);
	setMaximum(i);

	setMinimumWidth(minimumWidth() + maxValueWidth);
	setValue(valueFromText(element->logicalProperty(propertyName)));

	connect(
		this, SIGNAL(valueChanged(QString)),
		this, SLOT(setNewValue(QString))
	);
}

EnumWidget* EnumWidget::castToWidget() const {
	return const_cast<EnumWidget*>(this);
}

QString EnumWidget::textFromValue(int index) const {
	return possibleValuesToInt.key(index);
}

int EnumWidget::valueFromText(const QString &text) const {
	return possibleValuesToInt.value(text);
}

void EnumWidget::setNewValue(const QString &text) const {
	if (possibleValuesToInt.contains(text)) {
		element->setLogicalProperty(propertyName, text);
	}
}

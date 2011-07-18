#pragma once

#include "controlEntity.h"
#include "../element.h"

#include <QMap>
#include <QSpinBox>

class EnumWidget : public QSpinBox, public ControlEntity
{
	Q_OBJECT
	public:
		EnumWidget(Element *element, const QString &propertyName, const QStringList &values, QWidget *parent = 0);
		EnumWidget* castToWidget() const;
	protected:
		QString textFromValue(int val) const;
		int valueFromText(const QString &text) const;
	private:
		Element *element;
		QMap<QString, int> possibleValuesToInt;
	private slots:
		void setNewValue(const QString &text) const;
};

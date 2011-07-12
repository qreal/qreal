#pragma once

#include "control.h"
#include "../element.h"

#include <QMap>
#include <QSpinBox>

class EnumWidget : public QSpinBox, public Control
{
	Q_OBJECT
	public:
		EnumWidget(Element *element, const QString &propertyName, const QStringList &values, QWidget *parent = 0);
	protected:
		QString textFromValue(int val) const;
		int valueFromText(const QString &text) const;
	private:
		Element *element;
		QMap<QString, int> textToInt;
	private slots:
		void setNewValue(const QString &text) const;
};

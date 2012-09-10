#pragma once

#include <QtGui/QSpinBox>

#include "abstractSpinBox.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class SpinBox : public AbstractSpinBox
{
	Q_OBJECT

	Q_PROPERTY(int maximum READ maximum WRITE setMaximum USER true)
	Q_PROPERTY(int minimum READ minimum WRITE setMinimum USER true)
	Q_PROPERTY(QString prefix READ prefix WRITE setPrefix USER true)
	Q_PROPERTY(QString suffix READ suffix WRITE setSuffix USER true)
	Q_PROPERTY(int singleStep READ singleStep WRITE setSingleStep USER true)
	Q_PROPERTY(int value READ value WRITE setValue USER true)

public:
	SpinBox(ToolController *controller);

private slots:
	int maximum() const;
	int minimum() const;
	QString prefix() const;
	QString suffix() const;
	int singleStep() const;
	int value() const;

	void setMaximum(int maximum);
	void setMinimum(int minimum);
	void setPrefix(QString const &prefix);
	void setSuffix(QString const &suffix);
	void setSingleStep(int step);
	void setValue(int value);

	void valueChanged(int i);

private:
	QSpinBox *mSpinBox;

};

}
}

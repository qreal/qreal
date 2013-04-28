#pragma once

#include <QtWidgets/QAbstractSpinBox>

#include "tool.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class AbstractSpinBoxProxy : public ToolProxy
{
	Q_OBJECT

	Q_PROPERTY(bool accelerated READ isAccelerated WRITE setAccelerated USER true DESIGNABLE true)
	Q_PROPERTY(QAbstractSpinBox::CorrectionMode correctionMode READ correctionMode WRITE setCorrectionMode USER true DESIGNABLE true)
	Q_PROPERTY(bool hasButtons READ hasButtons WRITE setHasButtons USER true DESIGNABLE true)
	Q_PROPERTY(bool readonly READ isReadonly WRITE setReadonly USER true DESIGNABLE true)
	Q_PROPERTY(QString specialValueText READ specialValueText WRITE setSpecialValueText USER true DESIGNABLE true)
	Q_PROPERTY(bool wrapping READ hasWrapping WRITE setWrapping USER true DESIGNABLE true)

protected:
	explicit AbstractSpinBoxProxy(QAbstractSpinBox *spinBox);

private:
	bool isAccelerated() const;
	QAbstractSpinBox::CorrectionMode correctionMode() const;
	bool hasButtons() const;
	bool isReadonly() const;
	QString specialValueText() const;
	bool hasWrapping() const;

	void setAccelerated(bool accelerated);
	void setCorrectionMode(QAbstractSpinBox::CorrectionMode mode);
	void setHasButtons(bool hasButtons);
	void setReadonly(bool readonly);
	void setSpecialValueText(QString const &text);
	void setWrapping(bool wrapping);

	QAbstractSpinBox *mAbstractSpinBox;
};

class AbstractSpinBox : public Tool
{
	Q_OBJECT

protected:
	AbstractSpinBox(QAbstractSpinBox *spinBox, ToolController *controller);

private:
	QAbstractSpinBox *mAbstractSpinBox;
};

}
}

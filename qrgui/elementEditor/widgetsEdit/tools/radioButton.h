#pragma once

#include <QtWidgets/QRadioButton>

#include "abstractButton.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class RadioButtonProxy : public AbstractButtonProxy
{
	Q_OBJECT

	Q_PROPERTY(bool checked READ isChecked WRITE setChecked USER true DESIGNABLE true)

public:
	explicit RadioButtonProxy(QRadioButton *radioButton);

private slots:
	void toggled(bool checked);

private:
	bool isChecked() const;
	void setChecked(bool checked);

	QRadioButton *mRadioButton;
};

class RadioButton : public AbstractButton
{
	Q_OBJECT

public:
	explicit RadioButton(ToolController *controller);

private:
	QRadioButton *mRadioButton;
};

}
}

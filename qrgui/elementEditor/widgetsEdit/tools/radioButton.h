#pragma once

#include <QtGui/QRadioButton>

#include "abstractButton.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class RadioButton : public AbstractButton
{
	Q_OBJECT

	Q_PROPERTY(bool checked READ isChecked WRITE setChecked USER true)

public:
	RadioButton(ToolController *controller);

private slots:
	void toggled(bool checked);

private:
	bool isChecked() const;
	void setChecked(bool checked);

	QRadioButton *mRadioButton;

};

}
}

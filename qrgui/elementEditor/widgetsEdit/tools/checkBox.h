#pragma once

#include <QtGui/QCheckBox>

#include "abstractButton.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class CheckBox : public AbstractButton
{
	Q_OBJECT

	Q_PROPERTY(bool checked READ isChecked WRITE setChecked USER true)

public:
	CheckBox(ToolController *controller);

private slots:
	void toggled(bool isChecked);

private:
	bool isChecked() const;
	void setChecked(bool checked);

	QCheckBox *mCheckBox;

};

}
}

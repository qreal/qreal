#pragma once

#include <QtGui/QPushButton>

#include "abstractButton.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class PushButton : public AbstractButton
{
	Q_OBJECT

	Q_PROPERTY(bool flat READ isFlat WRITE setFlat USER true DESIGNABLE true)

public:
	PushButton(ToolController *controller);

private:
	bool isFlat() const;
	void setFlat(bool flat);

	QPushButton *mPushButton;
};

}
}

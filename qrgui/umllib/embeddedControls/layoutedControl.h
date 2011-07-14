#pragma once

#include "controlEntity.h"

#include <QWidget>
#include <QLayout>
#include <QBoxLayout>

class LayoutedControl : public QWidget
{
	Q_OBJECT
	public:
		LayoutedControl(ControlEntity *control);
		ControlEntity* getControl() const;
	private:
		ControlEntity* control;
		QLayout* layout;
};

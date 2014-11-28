#pragma once

#include <QtWidgets/QWidget>

#include "utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT WidgetFinder {
public:
	/// Returns widget defined by type(class name) and object name.
	static QWidget *widget(QWidget *parent, QString const &type, QString const &name = "");
};

}

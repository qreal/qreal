#pragma once

#include <QtCore/QObject>

class WidgetsHelperInterface : public QObject
{
public:
	/// Loads specified WTF-format document, creates
	/// new instance of template-satisfying widget and
	/// assigns it to binded node element
	virtual bool initWidget(QString const &filename) = 0;
};

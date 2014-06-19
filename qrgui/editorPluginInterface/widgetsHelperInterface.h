#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>

#include "propertyEditorInterface.h"

class WidgetsHelperInterface : public QObject
{
public:
	virtual ~WidgetsHelperInterface() {}

	/// Loads specified WTF-format document, creates
	/// new instance of template-satisfying widget and
	/// assigns it to binded node element
	virtual bool initWidget(QString const &filename) = 0;

	/// Returns a list of property editors in specified
	/// with @see initWidget(...) function template.
	virtual QMap<QString, PropertyEditorInterface *> propertyEditors() const = 0;
};

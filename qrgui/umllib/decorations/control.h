#pragma once

#include <QWidget>

class Control {
	public:
		Control(const QString &propertyName);
		QString getProperty() const;

	protected:
		const QString &propertyName;
};

#pragma once

#include <QWidget>

class Control {
	public:
		Control(const QString &propertyName);

		QString getPropertyName() const;
		virtual QWidget* castToWidget() const = 0;

	protected:
		const QString propertyName;
};

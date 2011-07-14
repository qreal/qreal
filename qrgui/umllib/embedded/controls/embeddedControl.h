#pragma once

#include <QWidget>

class EmbeddedControl {
	public:
		EmbeddedControl(const QString &propertyName);

		QString getPropertyName() const;
		virtual QWidget* castToWidget() const = 0;

	protected:
		const QString propertyName;
};

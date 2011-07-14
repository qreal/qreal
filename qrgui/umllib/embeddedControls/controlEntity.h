#pragma once

#include <QWidget>

class ControlEntity {
	public:
		ControlEntity(const QString &propertyName);

		QString getPropertyName() const;
		virtual QWidget* castToWidget() const = 0;

	protected:
		const QString propertyName;
};

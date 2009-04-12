#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

namespace qReal {

	class Id {
	public:
		explicit Id(QString editor = "", QString diagram = "", QString element = "", QString id = "");
		QString editor() const;
		QString diagram() const;
		QString element() const;
		QString id() const;
		unsigned idSize() const;
		QUrl toUrl() const;
		QString toString() const;
		// Деструктор по умолчанию ок.
		// Конструктор копирования по умолчанию ок.
	private:
		QString mEditor;
		QString mDiagram;
		QString mElement;
		QString mId;

		// Проверка корректности ид-шника (нужные имена непусты). Используется только для отладки.
		bool checkIntegrity() const;
	};

}

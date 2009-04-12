#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QHash>

namespace qReal {

	class Id {
	public:
		static Id loadFromString(QString const &string);

		explicit Id(QString const &editor = "", QString  const &diagram = "",
			QString  const &element = "", QString  const &id = "");
		Id(Id const &base, QString const &additional);
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

	inline bool operator==(const Id &i1, const Id &i2)
	{
		return i1.editor() == i2.editor()
			&& i1.diagram() == i2.diagram()
			&& i1.element() == i2.element()
			&& i1.id() == i2.id();
	}

	inline uint qHash(const Id &key)
	{
		return qHash(key.editor()) ^ qHash(key.diagram()) ^ qHash(key.element())
				^ qHash(key.id());
	}

}

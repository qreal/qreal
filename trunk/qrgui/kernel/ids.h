#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

namespace qReal {

	typedef QUrl Id;

	class IdParser {
	public:
		static Id createId();
		static Id append(Id const &source, QString part);

		static QString getEditor(Id const &id);
		static QString getDiagram(Id const &id);
		static QString getElement(Id const &id);
		static QString getId(Id const &id);
		static unsigned getIdSize(Id const &id);
	private:
		static QStringList getPath(Id const &id);
		static QString getPathElement(Id const & id, int const &index);
	};

}

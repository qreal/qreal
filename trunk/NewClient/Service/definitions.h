#pragma once

#include <QString>
#include <QList>

namespace qReal {

	typedef QString IdType;

	typedef QList<IdType> IdTypeList;

	typedef QString PropertyName;

	namespace model {

		class ModelTreeItem;
		typedef QList<ModelTreeItem*> ModelTreeItemPointerList;

	}

	const IdType ROOT_ID = "root";

	const char PATH_DIVIDER = '#';

	const QString DEFAULT_MIME_TYPE = "application/x-real-uml-data";

}

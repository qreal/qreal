#pragma once

#include <QString>
#include <QList>
#include "ids.h"

namespace qReal {

	typedef Id IdType;

	typedef QList<IdType> IdTypeList;

	typedef QString PropertyName;

	namespace model {

		class ModelTreeItem;
		typedef QList<ModelTreeItem*> ModelTreeItemPointerList;

	}

	const IdType ROOT_ID = Id("ROOT_ID","ROOT_ID","ROOT_ID","ROOT_ID");

	const char PATH_DIVIDER = '#';

	const QString DEFAULT_MIME_TYPE = "application/x-real-uml-data";

	namespace roles {
		enum {
			positionRole = Qt::UserRole + 1,
		};
	}
}

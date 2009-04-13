#pragma once

#include <QString>
#include <QList>
#include "../../qrgui/kernel/ids.h"

namespace qReal {

	typedef Id IdType;

	typedef QList<IdType> IdTypeList;

	typedef QString PropertyName;

	namespace model {

		class ModelTreeItem;
		typedef QList<ModelTreeItem*> ModelTreeItemPointerList;

	}

	const IdType ROOT_ID = Id();

	const char PATH_DIVIDER = '#';

	const QString DEFAULT_MIME_TYPE = "application/x-real-uml-data";

}

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

}

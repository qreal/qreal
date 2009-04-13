#pragma once

#include "../Service/definitions.h"

#include <QList>

namespace qReal {

	namespace model {

		class ModelTreeItem
		{
		public:
			ModelTreeItem(IdType const &id, ModelTreeItem *parent);
			IdType id() const;
			ModelTreeItem *parent() const;
			void setParent(ModelTreeItem *parent);
			ModelTreeItemPointerList children() const;
			void addChild(ModelTreeItem *child);
			void removeChild(ModelTreeItem *child);
			int row();

		private:
			ModelTreeItem *mParent;
			const IdType mId;
			ModelTreeItemPointerList mChildren;
		};

	}

}

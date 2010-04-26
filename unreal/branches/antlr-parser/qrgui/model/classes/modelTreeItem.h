#pragma once

#include "../../kernel/definitions.h"

#include <QList>

namespace qReal {

	namespace model {

		namespace details {

			class ModelTreeItem
			{
			public:
				typedef QList<ModelTreeItem*> PointerList;

				ModelTreeItem(Id const &id, ModelTreeItem *parent);
				Id id() const;
				ModelTreeItem *parent() const;
				void setParent(ModelTreeItem *parent);
				PointerList children() const;
				void addChild(ModelTreeItem *child);
				void removeChild(ModelTreeItem *child);
				int row();
				void clearChildren();

			private:
				ModelTreeItem *mParent;
				const Id mId;
				PointerList mChildren;
			};

		}

	}

}

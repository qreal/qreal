#pragma once

#include "../../kernel/roles.h"
#include "../../kernel/NewType.h"

#include <QList>

namespace qReal {

	namespace model {

		namespace details {

			class ModelTreeItem
			{
			public:
				typedef QList<ModelTreeItem*> PointerList;

                                ModelTreeItem(NewType const &type, ModelTreeItem *parent);
                                NewType type() const;
				ModelTreeItem *parent() const;
				void setParent(ModelTreeItem *parent);
				PointerList children() const;
				void addChild(ModelTreeItem *child);
				void removeChild(ModelTreeItem *child);
				int row();
				void clearChildren();

			private:
				ModelTreeItem *mParent;
                                const NewType mType;
				PointerList mChildren;
			};

		}

	}

}

#pragma once

#include "../Service/definitions.h"

namespace qReal {

	namespace model {

		class ModelTreeItem
		{
		public:
	//		ModelTreeItem(ModelTreeItem *parent);

		private:
			ModelTreeItem *mParent;
			IdType mId;
	//		QList<ModelTreeItem*> mChildren;
		};

	}

}
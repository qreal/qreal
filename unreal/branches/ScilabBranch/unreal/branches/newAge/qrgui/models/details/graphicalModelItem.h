#pragma once

#include "abstractModelItem.h"

namespace qReal {

	namespace models {

		namespace details {

			class GraphicalModelItem : public AbstractModelItem {
			public:
				GraphicalModelItem(Id const &id, Id const &logicalId, GraphicalModelItem *parent);
				Id logicalId() const;
			private:
				Id const mLogicalId;
			};

		}

	}

}

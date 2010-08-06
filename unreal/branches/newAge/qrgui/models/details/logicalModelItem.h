#pragma once

#include "abstractModelItem.h"

namespace qReal {

	namespace models {

		namespace details {

			class LogicalModelItem : public AbstractModelItem {
			public:
				LogicalModelItem(Id const &id, LogicalModelItem *parent);
			};

		}

	}

}

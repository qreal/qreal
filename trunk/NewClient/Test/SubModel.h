#pragma once

#include "../Model/Model.h"
#include "../Service/definitions.h"

namespace qReal {

	using namespace model;

	namespace test {

		class SubModel : public Model {
		public:
			void addItem(const IdType &id, const IdType &parent,
					const PropertyName &name, const QPointF &position) {
				addElementToModel(treeItems.value(parent),
					id,"",name,position,Qt::MoveAction);
			};
		};

	}

}

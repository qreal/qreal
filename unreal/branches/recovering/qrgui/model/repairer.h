#pragma once

#include "model.h"

namespace qReal {
	namespace model {
		class Model;

		class Repairer : QObject {
			Q_OBJECT

			public:
				Repairer(Model *model);
			private:
				Model *mModel;
				bool enabled;
		};
	}
}

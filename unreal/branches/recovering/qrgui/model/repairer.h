#pragma once

#include "model.h"
#include "message.h"

namespace qReal {
	namespace model {
		class Model;

		class Repairer {

			public:
				Repairer(Model *model);
			private:
				Model *mModel;
				bool enabled;
		};
	}
}

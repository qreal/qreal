#pragma once

#include "diffState.h"

#include <QVariant>

namespace qrRepo
{
	namespace details
	{
		class Modification
		{
		public:
			Modification();
			DiffState state();
			QVariant oldValue();
			QVariant newValue();
			void setState(DiffState state);
			void setOldValue(QVariant value);
			void setNewValue(QVariant value);

		private:
			DiffState mState;
			QVariant mOldValue;
			QVariant mNewValue;
		};
	}
}

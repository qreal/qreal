#pragma once

#include "../../qrgui/kernel/ids.h"
#include "classes/object.h"
#include "diffState.h"
#include "modification.h"

namespace qrRepo
{

	namespace details
	{

		class Difference
		{
		public:
			Difference(Object *objectInRepo, Object *objectInWorkingCopy);
			Difference(Object *object, DiffState state);

		private:
			void findChildrenDifference();
			void findPropertyDifference();

			qReal::Id mId;
			qReal::Id mLogicId;

			DiffState mState;
			Object *mOldObject;
			Object *mNewObject;

			DiffState mParentState;
			qReal::Id mOldParent;
			qReal::Id mNewParent;

			DiffState mChildrenState;
			QHash<qReal::Id, qrRepo::details::DiffState> mChildren;
			DiffState mPropertyState;
			QHash<QString, Modification> mProperties;
		};
	}
}

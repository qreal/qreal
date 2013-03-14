#include "createRemoveCommandImplementation.h"
#include "../../umllib/nodeElement.h"

using namespace qReal::commands;

CreateRemoveCommandImplementation::CreateRemoveCommandImplementation(
		models::LogicalModelAssistApi *logicalApi
		, models::GraphicalModelAssistApi *graphicalApi
		, const Id &logicalParent
		, const Id &graphicalParent
		, const Id &id
		, bool isFromLogicalModel
		, const QString &name
		, const QPointF &position)
	: mLogicalApi(logicalApi)
	, mGraphicalApi(graphicalApi)
	, mLogicalParent(logicalParent)
	, mGraphicalParent(graphicalParent)
	, mId(id)
	, mIsFromLogicalModel(isFromLogicalModel)
	, mName(name)
	, mPosition(position)
{
}

Id CreateRemoveCommandImplementation::create()
{
	mId = mGraphicalApi->createElement(mGraphicalParent, mId
			, mIsFromLogicalModel, mName, mPosition);
	return mId;
}

void CreateRemoveCommandImplementation::remove()
{
	if (mIsFromLogicalModel) {
		mLogicalApi->removeElement(mId);
	} else {
		Id const logicalId = mGraphicalApi->logicalId(mId);
		if (!mLogicalApi->logicalRepoApi().exist(logicalId)) {
			mGraphicalApi->removeElement(mId);
			return;
		}
		IdList const graphicalIds = mGraphicalApi->graphicalIdsByLogicalId(logicalId);
		mGraphicalApi->removeElement(mId);
		// Checking that the only graphical part is our element itself
		// (bijection between graphical and logical parts)
		if (graphicalIds.count() == 1 && graphicalIds[0] == mId) {
			mLogicalApi->removeReferencesTo(logicalId);
			mLogicalApi->removeReferencesFrom(logicalId);
			mLogicalApi->removeElement(logicalId);
		}
	}
}

bool CreateRemoveCommandImplementation::equals(CreateRemoveCommandImplementation const &other) const
{
	return mLogicalApi == other.mLogicalApi
			&& mGraphicalApi == other.mGraphicalApi
			&& mLogicalParent == other.mLogicalParent
			&& mGraphicalParent == other.mGraphicalParent
			&& mId == other.mId
			&& mIsFromLogicalModel == other.mIsFromLogicalModel
			&& mName == other.mName
			&& mPosition == other.mPosition;
}

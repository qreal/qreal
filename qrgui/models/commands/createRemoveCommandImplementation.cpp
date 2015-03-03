#include "createRemoveCommandImplementation.h"

#include <QtCore/QTimer>

#include "models/logicalModelAssistApi.h"
#include "models/graphicalModelAssistApi.h"

using namespace qReal;
using namespace commands;

CreateRemoveCommandImplementation::CreateRemoveCommandImplementation(
		models::LogicalModelAssistApi &logicalApi
		, models::GraphicalModelAssistApi &graphicalApi
		, const models::Exploser &exploser
		, const Id &logicalParent
		, const Id &graphicalParent
		, const Id &id
		, bool isFromLogicalModel
		, const QString &name
		, const QPointF &position)
	: mLogicalApi(logicalApi)
	, mGraphicalApi(graphicalApi)
	, mExploser(exploser)
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
	if (!mGraphicalApi.editorManagerInterface().hasElement(mId.type())) {
		return Id();
	}

	mId = mGraphicalApi.createElement(mGraphicalParent, mId
			, mIsFromLogicalModel, mName, mPosition, mOldLogicalId);

	if (!mGraphicalPropertiesSnapshot.isEmpty()) {
		mGraphicalApi.setProperties(mId, mGraphicalPropertiesSnapshot);
	}

	const Id logicalId = mGraphicalApi.logicalId(mId);
	if (mLogicalApi.logicalRepoApi().exist(logicalId)
			&& !mLogicalPropertiesSnapshot.isEmpty()) {
		mGraphicalApi.setProperties(logicalId, mLogicalPropertiesSnapshot);
	}

	refreshAllPalettes();
	return mId;
}

Id CreateRemoveCommandImplementation::id() const
{
	return mId;
}

void CreateRemoveCommandImplementation::remove()
{
	if (mIsFromLogicalModel) {
		mLogicalApi.removeReferencesTo(mId);
		mLogicalApi.removeReferencesFrom(mId);
		mLogicalApi.removeElement(mId);
		mGraphicalApi.removeElement(mId);
	} else {
		mGraphicalPropertiesSnapshot = mGraphicalApi.properties(mId);
		const Id logicalId = mGraphicalApi.logicalId(mId);
		if (!mLogicalApi.logicalRepoApi().exist(logicalId)) {
			mGraphicalApi.removeElement(mId);
			refreshAllPalettes();
			return;
		}

		mOldLogicalId = logicalId;
		mLogicalPropertiesSnapshot = mGraphicalApi.properties(logicalId);
		const IdList graphicalIds = mGraphicalApi.graphicalIdsByLogicalId(logicalId);
		mGraphicalApi.removeElement(mId);
		// Checking that the only graphical part is our element itself
		// (bijection between graphical and logical parts)
		if (graphicalIds.count() == 1 && graphicalIds[0] == mId) {
			mLogicalApi.removeReferencesTo(logicalId);
			mLogicalApi.removeReferencesFrom(logicalId);
			mLogicalApi.removeElement(logicalId);
		}
	}

	refreshAllPalettes();
}

bool CreateRemoveCommandImplementation::equals(const CreateRemoveCommandImplementation &other) const
{
	return mLogicalParent == other.mLogicalParent
			&& mGraphicalParent == other.mGraphicalParent
			&& mId == other.mId
			&& mIsFromLogicalModel == other.mIsFromLogicalModel
			&& mName == other.mName
			&& mPosition == other.mPosition
			;
}

void CreateRemoveCommandImplementation::setNewPosition(const QPointF &position)
{
	mPosition = position;
}

void CreateRemoveCommandImplementation::refreshAllPalettes()
{
	// Calling refreshing immideately may cause segfault because of deletting drag source
	QTimer::singleShot(0, &mExploser, SIGNAL(explosionsSetCouldChange()));
}

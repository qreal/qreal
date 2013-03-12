#include "createRemoveCommandImplementation.h"
#include "../../umllib/nodeElement.h"

using namespace qReal::commands;

CreateRemoveCommandImplementation::CreateRemoveCommandImplementation(
		models::LogicalModelAssistApi *logicalApi
		, models::GraphicalModelAssistApi *graphicalApi
		, const Id &parent
		, const Id &id
		, bool isFromLogicalModel
		, const QString &name
		, const QPointF &position)
	: mLogicalApi(logicalApi)
	, mGraphicalApi(graphicalApi)
	, mParent(parent)
	, mId(id)
	, mIsFromLogicalModel(isFromLogicalModel)
	, mName(name)
	, mPosition(position)
{
}

Id CreateRemoveCommandImplementation::create()
{
	return mGraphicalApi->createElement(mParent, mId, mIsFromLogicalModel, mName, mPosition);
}

void CreateRemoveCommandImplementation::remove()
{
	if (mIsFromLogicalModel) {
		mLogicalApi->removeElement(mId);
	} else {
		mGraphicalApi->removeElement(mId);
	}
}

#include "createRemoveCommandImplementation.h"
#include "../../umllib/nodeElement.h"

using namespace qReal::commands;

CreateRemoveCommandImplementation::CreateRemoveCommandImplementation(
		models::GraphicalModelAssistApi *api
		, const Id &parent
		, const Id &id
		, bool isFromLogicalModel
		, const QString &name
		, const QPointF &position)
	: mApi(api)
	, mParent(parent)
	, mId(id)
	, mIsFromLogicalModel(isFromLogicalModel)
	, mName(name)
	, mPosition(position)
{
}

Id CreateRemoveCommandImplementation::create()
{
	return mApi->createElement(mParent, mId, mIsFromLogicalModel, mName, mPosition);
}

void CreateRemoveCommandImplementation::remove()
{
}

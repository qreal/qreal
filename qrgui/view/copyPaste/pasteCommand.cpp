#include "pasteCommand.h"

using namespace qReal::commands;

PasteCommand::PasteCommand(EditorViewScene *scene
		, EditorViewMViface const *mvIface
		, QPointF const &offset
		, bool isGraphicalCopy
		, QHash<Id, Id> *copiedIds)
	: mScene(scene), mMVIface(mvIface)
	, mOffset(offset), mCopiedIds(copiedIds)
	, mIsGraphicalCopy(isGraphicalCopy)
{
}

Id PasteCommand::result() const
{
	return mResult;
}

bool PasteCommand::execute()
{
	if (mIsGraphicalCopy) {
		mResult = pasteGraphicalCopy();
	} else {
		mResult = pasteNewInstance();
	}
	restoreElement();
	return true;
}

bool PasteCommand::restoreState()
{
	return true;
}

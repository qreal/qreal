#pragma once

#include "createElementCommand.h"
#include "removeElementCommand.h"
#include "../qrgui/view/editorViewScene.h"
#include "../../models/graphicalModelAssistApi.h"
#include "../../models/logicalModelAssistApi.h"

namespace qReal
{
namespace commands
{

class InsertIntoEdgeCommand : public AbstractCommand
{
public:
	InsertIntoEdgeCommand(EditorViewScene *scene, models::LogicalModelAssistApi *logicalAssistApi
			,models::GraphicalModelAssistApi *graphicalAssistApi
			, Id const &firstElem, Id const &lastElem, Id const &parent, QPointF const &scenePos
			, QPointF const &shift, bool isFromLogicalModel, CreateElementCommand *createCommand = 0);
	~InsertIntoEdgeCommand();

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	void initCommand(CreateElementCommand *&command, Id const &type);
	void makeLink(CreateElementCommand *command, NodeElement *src, NodeElement *dst);

	void returnElementsToOldPositions();

	EditorViewScene *mScene;

	models::LogicalModelAssistApi *mLogicalAssistApi;
	models::GraphicalModelAssistApi *mGraphicalAssistApi;

	Id mFirstId;
	Id mLastId;
	Id mParentId;

	Id mOldEdge;

	QPointF mPos;
	QPolygon mConfiguration;
	QPointF mShift;

	bool mIsLogical;

	QMap<Id, QPointF> mElementShifting;

	CreateElementCommand *mCreateFirst;
	CreateElementCommand *mCreateSecond;
	RemoveElementCommand *mRemoveOldEdge;

	CreateElementCommand *mCreateCommand;
};

}
}

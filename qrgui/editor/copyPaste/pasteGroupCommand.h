#pragma once

#include "controller/commands/abstractCommand.h"
#include "editor/editorViewScene.h"

namespace qReal {
namespace commands {

class PasteGroupCommand : public AbstractCommand
{
public:
	PasteGroupCommand(EditorViewScene *scene, bool isGraphicalCopy);
	virtual ~PasteGroupCommand();

	bool isEmpty() const;

protected:
	bool execute();
	bool restoreState();

private:
	void prepareCommands();
	QHash<Id, Id> *preparePasteNodesCommands(QList<NodeData> &nodesData, QPointF const &offset);
	void preparePasteEdgeCommand(EdgeData const &edgeData, QPointF const &offset);

	void pullDataFromClipboard(QList<NodeData> &nodesData, QList<EdgeData> &edgesData) const;

	EditorViewScene *mScene;
	bool const mIsGraphicalCopy;
	QHash<Id, Id> *mCopiedIds;
	bool mIsEmpty;
};

}
}

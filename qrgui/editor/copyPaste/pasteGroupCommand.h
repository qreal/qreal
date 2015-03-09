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
	QHash<Id, Id> *preparePasteNodesCommands(QList<NodeData> &nodesData, const QPointF &offset);
	void preparePasteEdgeCommand(const EdgeData &edgeData, const QPointF &offset);

	void pullDataFromClipboard(QList<NodeData> &nodesData, QList<EdgeData> &edgesData) const;

	EditorViewScene *mScene;
	const bool mIsGraphicalCopy;
	QHash<Id, Id> *mCopiedIds;
	bool mIsEmpty;
};

}
}

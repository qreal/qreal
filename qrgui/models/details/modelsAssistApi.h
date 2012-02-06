#pragma once

#include <QtCore/QVariant>
#include <QtCore/QPointF>
#include <QtCore/QModelIndex>
#include <QtCore/QUuid>

#include "../../../qrkernel/ids.h"
//#include "../../toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"

namespace qReal {

class EditorManager;
class ConstraintsManager;

namespace models {

namespace details {

namespace modelsImplementation {
class AbstractModel;
}

class ModelsAssistApi : public QObject
{
	Q_OBJECT

public:
	ModelsAssistApi(details::modelsImplementation::AbstractModel &model, EditorManager const &editorManager, ConstraintsManager const &constraintsManager);
	EditorManager const &editorManager() const;
	ConstraintsManager const &constraintsManager() const;
	Id createElement(Id const &parent, Id const &id, bool isFromLogicalModel, QString const &name, QPointF const &position);

	/// Stacks item element before sibling (they should have the same parent)
	void stackBefore(Id const &element, Id const &sibling);

	void setTo(Id const &elem, Id const &newValue);
	Id to(Id const &elem) const;


	void setFrom(Id const &elem, Id const &newValue);
	Id from(Id const &elem) const;

	QModelIndex indexById(Id const &id) const;
	Id idByIndex(QModelIndex const &index) const;
	QPersistentModelIndex rootIndex() const;
	Id rootId() const;

	bool hasRootDiagrams() const;
	int childrenOfRootDiagram() const;
	int childrenOfDiagram(const Id &parent) const;

	void setProperty(Id const &elem, QVariant const &newValue, int const role);
	QVariant property(Id const &elem, int const role) const;
	int roleIndexByName(Id const &elem, QString const &roleName) const;

signals:
	void propertyChanged(Id const &elem);

private:

	ModelsAssistApi(ModelsAssistApi const &);
	ModelsAssistApi& operator =(ModelsAssistApi const &);

	details::modelsImplementation::AbstractModel &mModel;
	EditorManager const &mEditorManager;
	ConstraintsManager const &mConstraintsManager;
};
}
}
}

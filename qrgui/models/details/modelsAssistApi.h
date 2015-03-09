#pragma once

#include <QtCore/QVariant>
#include <QtCore/QPointF>
#include <QtCore/QModelIndex>
#include <QtCore/QUuid>

#include <qrkernel/ids.h>

#include "qrgui/plugins/pluginManager/editorManagerInterface.h"

namespace qReal {

class EditorManagerInterface;

namespace models {

namespace details {

namespace modelsImplementation {
class AbstractModel;
}

class ModelsAssistApi
{
public:
	ModelsAssistApi(details::modelsImplementation::AbstractModel &model
			, const EditorManagerInterface &editorManagerInterface);
	const EditorManagerInterface &editorManagerInterface() const;
	Id createElement(const Id &parent, const Id &id, const Id &logicalId
			, bool isFromLogicalModel, const QString &name, const QPointF &position);

	/// Stacks item element before sibling (they should have the same parent)
	void stackBefore(const Id &element, const Id &sibling);

	void setTo(const Id &elem, const Id &newValue);
	Id to(const Id &elem) const;


	void setFrom(const Id &elem, const Id &newValue);
	Id from(const Id &elem) const;

	QModelIndex indexById(const Id &id) const;
	Id idByIndex(const QModelIndex &index) const;
	QPersistentModelIndex rootIndex() const;
	Id rootId() const;

	bool hasRootDiagrams() const;
	int childrenOfRootDiagram() const;
	int childrenOfDiagram(const Id &parent) const;

	void setProperty(const Id &elem, const QVariant &newValue, const int role);
	QVariant property(const Id &elem, const int role) const;
	int roleIndexByName(const Id &elem, const QString &roleName) const;

private:

	ModelsAssistApi(const ModelsAssistApi &);
	ModelsAssistApi& operator =(const ModelsAssistApi &);

	details::modelsImplementation::AbstractModel &mModel;
	const EditorManagerInterface &mEditorManagerInterface;
};
}
}
}

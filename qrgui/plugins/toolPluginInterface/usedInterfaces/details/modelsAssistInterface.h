#pragma once

#include <QtCore/QVariant>
#include <QtCore/QPointF>
#include <QtCore/QModelIndex>
#include <QtCore/QUuid>

#include <qrkernel/ids.h>

namespace qReal {

class EditorManagerInterface;

namespace details {

class ModelsAssistInterface
{
public:
	virtual Id createElement(const Id &parent, const Id &type) = 0;
	virtual Id createElement(const Id &parent, const Id &id
		, bool isFromLogicalModel, const QString &name
		, const QPointF &position, const Id &preferedLogicalId = Id()) = 0;
	virtual IdList children(const Id &element) const = 0;
	virtual void changeParent(const Id &element, const Id &parent, const QPointF &position = QPointF()) = 0;

	virtual void setName(const Id &elem, const QString &newValue) = 0;
	virtual QString name(const Id &elem) const = 0;

	virtual void setTo(const Id &elem, const Id &newValue) = 0;
	virtual Id to(const Id &elem) const = 0;

	virtual void setFrom(const Id &elem, const Id &newValue) = 0;
	virtual Id from(const Id &elem) const = 0;

	virtual QModelIndex indexById(const Id &id) const = 0;
	virtual Id idByIndex(QModelIndex const &index) const = 0;
	virtual QPersistentModelIndex rootIndex() const = 0;
	virtual Id rootId() const = 0;

	virtual bool hasRootDiagrams() const = 0;
	virtual int childrenOfRootDiagram() const = 0;
	virtual int childrenOfDiagram(const Id &parent) const = 0;

	virtual void removeElement(const Id &id) = 0;

	/// Returns reference to Editor Manager object providing information about language metamodel.
	virtual const EditorManagerInterface &editorManagerInterface() const = 0;
};

}
}

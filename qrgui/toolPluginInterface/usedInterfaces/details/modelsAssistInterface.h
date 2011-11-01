#pragma once

#include <QtCore/QVariant>
#include <QtCore/QPointF>
#include <QtCore/QModelIndex>
#include <QtCore/QUuid>

#include "../../../../qrkernel/ids.h"

namespace qReal {
namespace details {

class ModelsAssistInterface
{
public:
	virtual Id createElement(Id const &parent, Id const &type) = 0;
	virtual Id createElement(Id const &parent, Id const &id, bool isFromLogicalModel, QString const &name, QPointF const &position) = 0;
	virtual IdList children(Id const &element) const = 0;
	virtual void changeParent(Id const &element, Id const &parent, QPointF const &position = QPointF()) = 0;

	virtual void setTo(Id const &elem, Id const &newValue) = 0;
	virtual Id to(Id const &elem) const = 0;

	virtual void setFrom(Id const &elem, Id const &newValue) = 0;
	virtual Id from(Id const &elem) const = 0;

	virtual QModelIndex indexById(Id const &id) const = 0;
	virtual Id idByIndex(QModelIndex const &index) const = 0;
	virtual QPersistentModelIndex rootIndex() const = 0;
	virtual Id rootId() const = 0;

	virtual bool hasRootDiagrams() const = 0;
	virtual int childrenOfRootDiagram() const = 0;
	virtual int childrenOfDiagram(const Id &parent) const = 0;
};

}
}

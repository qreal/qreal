#pragma once

#include "../../../qrkernel/ids.h"
#include "details/modelsAssistInterface.h"
#include "../../../qrrepo/graphicalRepoApi.h"

namespace qReal {

class GraphicalModelAssistInterface : public details::ModelsAssistInterface
{
public:
	virtual qrRepo::GraphicalRepoApi const &graphicalRepoApi() const = 0;
	virtual qrRepo::GraphicalRepoApi &mutableGraphicalRepoApi() const = 0;
	virtual Id createElement(Id const &parent, Id const &type) = 0;
	virtual Id createElement(Id const &parent, Id const &id, bool isFromLogicalModel, bool isFromLibraryModel, QString const &name, QPointF const &position) = 0;
	virtual Id copyElement(Id const &source) = 0;
	virtual IdList children(Id const &element) const = 0;
	virtual void changeParent(Id const &element, Id const &parent, QPointF const &position) = 0;
	virtual void copyProperties(Id const &dest, Id const &src) = 0;
	virtual IdList temporaryRemovedLinksFrom(Id const &elem) const = 0;
	virtual IdList temporaryRemovedLinksTo(Id const &elem) const = 0;
	virtual IdList temporaryRemovedLinksNone(Id const &elem) const = 0;
	virtual void removeTemporaryRemovedLinks(Id const &elem) = 0;

	virtual void setConfiguration(Id const &elem, QPolygon const &newValue) = 0;
	virtual QPolygon configuration(Id const &elem) const = 0;

	virtual void setPosition(Id const &elem, QPointF const &newValue) = 0;
	virtual QPointF position(Id const &elem) const = 0;

	virtual void setToPort(Id const &elem, qreal const &newValue) = 0;
	virtual qreal toPort(Id const &elem) const = 0;

	virtual void setFromPort(Id const &elem, qreal const &newValue) = 0;
	virtual qreal fromPort(Id const &elem) const = 0;

	virtual void setName(Id const &elem, QString const &newValue) = 0;
	virtual QString name(Id const &elem) const = 0;

	virtual void setToolTip(Id const &elem, QString const &newValue) = 0;
	virtual QString toolTip(Id const &elem) const = 0;

	virtual Id logicalId(Id const &elem) const = 0;
	virtual IdList graphicalIdsByLogicalId(Id const &logicalId) const = 0;

	virtual bool isGraphicalId(Id const &id) const = 0;
};

}

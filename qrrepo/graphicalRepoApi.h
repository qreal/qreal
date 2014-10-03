#pragma once

#include "../qrkernel/roles.h"
#include "commonRepoApi.h"

namespace qrRepo {

/// Methods for working with graphical model in a repository.
class GraphicalRepoApi : public CommonRepoApi
{
public:
	virtual ~GraphicalRepoApi() {}

	virtual qReal::Id copy(qReal::Id const &src) = 0;

	virtual void addChild(qReal::Id const &id, qReal::Id const &logicalId, qReal::Id const &child) = 0;

	virtual qreal fromPort(qReal::Id const &id) const = 0;
	virtual void setFromPort(qReal::Id const &id, qreal fromPort) = 0;

	virtual qreal toPort(qReal::Id const &id) const = 0;
	virtual void setToPort(qReal::Id const &id, qreal toPort) = 0;

	virtual QVariant position(qReal::Id const &id) const = 0;
	virtual QVariant configuration(qReal::Id const &id) const = 0;

	virtual void setPosition(qReal::Id const &id, QVariant const &position) = 0;
	virtual void setConfiguration(qReal::Id const &id, QVariant const &configuration) = 0;

	virtual qReal::IdList graphicalElements() const = 0;
	virtual qReal::IdList graphicalElements(qReal::Id const &type) const = 0;

	virtual bool isGraphicalElement(qReal::Id const &id) const = 0;
	virtual qReal::Id logicalId(qReal::Id const &id) const = 0;

	virtual qReal::IdList temporaryRemovedLinksAt(qReal::Id const &id, QString const &direction) const = 0;
	virtual void setTemporaryRemovedLinks(qReal::Id const &id, qReal::IdList const &value, QString const &direction) = 0;
	virtual void removeTemporaryRemovedLinks(qReal::Id const &id) = 0;

	virtual void copyProperties(qReal::Id const &dest, qReal::Id const &src) = 0;
	virtual QMap<QString, QVariant> properties(qReal::Id const &id) = 0;
	virtual void setProperties(qReal::Id const &id, QMap<QString, QVariant> const &properties) = 0;

	virtual void createGraphicalPart(qReal::Id const &id, int partIndex) = 0;

	virtual QList<int> graphicalParts(qReal::Id const &id) const = 0;

	virtual QVariant graphicalPartProperty(qReal::Id const &id, int partIndex, QString const &propertyName) const = 0;

	virtual void setGraphicalPartProperty(
			qReal::Id const &id
			, int partIndex
			, QString const &propertyName
			, QVariant const &value
			) = 0;
};

}

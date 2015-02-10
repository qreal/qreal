#pragma once

#include <qrkernel/roles.h>

#include "commonRepoApi.h"

namespace qrRepo {

/// Methods for working with graphical model in a repository.
class GraphicalRepoApi : public CommonRepoApi
{
public:
	virtual ~GraphicalRepoApi() {}

	virtual qReal::Id copy(const qReal::Id &src) = 0;

	virtual void addChild(const qReal::Id &id, const qReal::Id &logicalId, const qReal::Id &child) = 0;

	virtual qreal fromPort(const qReal::Id &id) const = 0;
	virtual void setFromPort(const qReal::Id &id, qreal fromPort) = 0;

	virtual qreal toPort(const qReal::Id &id) const = 0;
	virtual void setToPort(const qReal::Id &id, qreal toPort) = 0;

	virtual QVariant position(const qReal::Id &id) const = 0;
	virtual QVariant configuration(const qReal::Id &id) const = 0;

	virtual void setPosition(const qReal::Id &id, const QVariant &position) = 0;
	virtual void setConfiguration(const qReal::Id &id, const QVariant &configuration) = 0;

	virtual qReal::IdList graphicalElements() const = 0;
	virtual qReal::IdList graphicalElements(const qReal::Id &type) const = 0;

	virtual bool isGraphicalElement(const qReal::Id &id) const = 0;
	virtual qReal::Id logicalId(const qReal::Id &id) const = 0;

	virtual qReal::IdList temporaryRemovedLinksAt(const qReal::Id &id, const QString &direction) const = 0;
	virtual void setTemporaryRemovedLinks(const qReal::Id &id, const qReal::IdList &value
			, const QString &direction) = 0;

	virtual void removeTemporaryRemovedLinks(const qReal::Id &id) = 0;

	virtual void copyProperties(const qReal::Id &dest, const qReal::Id &src) = 0;
	virtual QMap<QString, QVariant> properties(const qReal::Id &id) = 0;
	virtual void setProperties(const qReal::Id &id, QMap<QString, QVariant> const &properties) = 0;

	virtual void createGraphicalPart(const qReal::Id &id, int partIndex) = 0;

	virtual QList<int> graphicalParts(const qReal::Id &id) const = 0;

	virtual QVariant graphicalPartProperty(const qReal::Id &id, int partIndex, const QString &propertyName) const = 0;

	virtual void setGraphicalPartProperty(
			const qReal::Id &id
			, int partIndex
			, const QString &propertyName
			, const QVariant &value
			) = 0;
};

}

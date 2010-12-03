#include "../qrgui/kernel/roles.h"
#include "commonRepoApi.h"

namespace qrRepo {

class GraphicalRepoApi : public CommonRepoApi
{
public:
	virtual ~GraphicalRepoApi(){}

	virtual qReal::IdList outgoingLinks(qReal::Id const &id) const = 0;
	virtual qReal::IdList incomingLinks(qReal::Id const &id) const = 0;
	virtual qReal::IdList links(qReal::Id const &id) const = 0;
	virtual void addChild(qReal::Id const &id, qReal::Id const &logicalId, qReal::Id const &child) = 0;

	virtual double fromPort(qReal::Id const &id) const = 0;
	virtual void setFromPort(qReal::Id const &id, double fromPort) = 0;

	virtual double toPort(qReal::Id const &id) const = 0;
	virtual void setToPort(qReal::Id const &id, double toPort) = 0;

	virtual QVariant position(qReal::Id const &id) const = 0;
	virtual QVariant configuration(qReal::Id const &id) const = 0;

	virtual void setPosition(qReal::Id const &id, QVariant const &position) = 0;
	virtual void setConfiguration(qReal::Id const &id, QVariant const &configuration) = 0;
};

}

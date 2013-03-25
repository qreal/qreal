#pragma once

#include "../../controller/commands/edgeElementCommand.h"
#include "../../controller/commands/trackingEntity.h"

namespace qReal
{
namespace commands
{

class ReshapeEdgeCommand : public EdgeElementCommand, public TrackingEntity
{
public:
	ReshapeEdgeCommand(EdgeElement const *edge);
	virtual ~ReshapeEdgeCommand() {}

	virtual void startTracking();
	virtual void stopTracking();

protected:
	bool execute();
	bool restoreState();

private:
	void saveConfiguration(QPolygonF &target, Id &src, Id &dst);
	void applyConfiguration(QPolygonF const &configuration, Id const &src, Id const &dst);

	QPolygonF mOldConfiguration;
	QPolygonF mNewConfiguration;
	Id mOldSrc;
	Id mOldDst;
	Id mNewSrc;
	Id mNewDst;
};

}
}

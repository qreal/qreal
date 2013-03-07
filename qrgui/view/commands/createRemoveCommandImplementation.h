#pragma once

#include "../../models/graphicalModelAssistApi.h"

namespace qReal
{
namespace commands
{

class CreateRemoveCommandImplementation
{
public:
	CreateRemoveCommandImplementation(
			models::GraphicalModelAssistApi *api
			, Id const &parent
			, Id const &id
			, bool isFromLogicalModel
			, QString const &name
			, QPointF const &position);

	Id create();
	void remove();

private:
	models::GraphicalModelAssistApi *mApi;
	Id mParent;
	Id mId;
	bool mIsFromLogicalModel;
	QString mName;
	QPointF mPosition;
};

}
}

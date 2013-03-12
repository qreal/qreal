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
			models::LogicalModelAssistApi *logicalApi
			, models::GraphicalModelAssistApi *graphicalApi
			, Id const &parent
			, Id const &id
			, bool isFromLogicalModel
			, QString const &name
			, QPointF const &position);

	Id create();
	void remove();

private:
	models::LogicalModelAssistApi *mLogicalApi; // Doesn`t take ownership
	models::GraphicalModelAssistApi *mGraphicalApi; // Doesn`t take ownership
	Id mParent;
	Id mId;
	bool mIsFromLogicalModel;
	QString mName;
	QPointF mPosition;
};

}
}

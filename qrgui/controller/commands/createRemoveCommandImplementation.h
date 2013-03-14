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
			, const Id &logicalParent
			, const Id &graphicalParent
			, Id const &id
			, bool isFromLogicalModel
			, QString const &name
			, QPointF const &position);

	Id create();
	void remove();

	bool equals(CreateRemoveCommandImplementation const &other) const;

private:
	models::LogicalModelAssistApi *mLogicalApi; // Doesn`t take ownership
	models::GraphicalModelAssistApi *mGraphicalApi; // Doesn`t take ownership
	Id mLogicalParent;
	Id mGraphicalParent;
	Id mId;
	bool mIsFromLogicalModel;
	QString mName;
	QPointF mPosition;
};

inline bool operator==(CreateRemoveCommandImplementation const &i1
		, CreateRemoveCommandImplementation const &i2)
{
	return i1.equals(i2);
}

}
}

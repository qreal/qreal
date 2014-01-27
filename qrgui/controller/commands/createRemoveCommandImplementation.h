#pragma once

#include "models/graphicalModelAssistApi.h"

namespace qReal {
namespace commands {

class CreateRemoveCommandImplementation
{
public:
	CreateRemoveCommandImplementation(
			models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, const Id &logicalParent
			, const Id &graphicalParent
			, Id const &id
			, bool isFromLogicalModel
			, QString const &name
			, QPointF const &position);

	Id create();
	void remove();

	Id id() const;

	bool equals(CreateRemoveCommandImplementation const &other) const;

private:
	void refreshAllPalettes();

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	Id const mLogicalParent;
	Id const mGraphicalParent;
	Id mId;
	bool const mIsFromLogicalModel;
	QString const mName;
	QPointF const mPosition;
	QMap<QString, QVariant> mLogicalPropertiesSnapshot;
	QMap<QString, QVariant> mGraphicalPropertiesSnapshot;
	Id mOldLogicalId;
};

inline bool operator==(CreateRemoveCommandImplementation const &i1
		, CreateRemoveCommandImplementation const &i2)
{
	return i1.equals(i2);
}

}
}

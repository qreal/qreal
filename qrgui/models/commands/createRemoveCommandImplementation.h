#pragma once

#include "models/exploser.h"

namespace qReal {
namespace commands {

class CreateRemoveCommandImplementation
{
public:
	CreateRemoveCommandImplementation(
			models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, models::Exploser const &exploser
			, Id const &logicalParent
			, Id const &graphicalParent
			, Id const &id
			, bool isFromLogicalModel
			, QString const &name
			, QPointF const &position);

	Id create();
	void remove();

	Id id() const;

	bool equals(CreateRemoveCommandImplementation const &other) const;

	/// @todo: Bad method, required only for linkers. Get rid of it.
	/// Modifies command setting new creation position.
	void setNewPosition(QPointF const &position);

private:
	void refreshAllPalettes();

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	models::Exploser const &mExploser;
	Id const mLogicalParent;
	Id const mGraphicalParent;
	Id mId;
	bool const mIsFromLogicalModel;
	QString const mName;
	QPointF mPosition;
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

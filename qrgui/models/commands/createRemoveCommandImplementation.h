#pragma once

#include "models/exploser.h"

namespace qReal {
namespace commands {

class QRGUI_MODELS_EXPORT CreateRemoveCommandImplementation
{
public:
	CreateRemoveCommandImplementation(
			models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, models::Exploser const &exploser
			, const Id &logicalParent
			, const Id &graphicalParent
			, const Id &id
			, bool isFromLogicalModel
			, const QString &name
			, const QPointF &position);

	Id create();
	void remove();

	Id id() const;

	bool equals(CreateRemoveCommandImplementation const &other) const;

	/// @todo: Bad method, required only for linkers. Get rid of it.
	/// Modifies command setting new creation position.
	void setNewPosition(const QPointF &position);

private:
	void refreshAllPalettes();

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	models::Exploser const &mExploser;
	const Id mLogicalParent;
	const Id mGraphicalParent;
	Id mId;
	const bool mIsFromLogicalModel;
	const QString mName;
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

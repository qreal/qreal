#pragma once

#include <QtCore/QString>
#include <QtCore/QStack>

#include <qrkernel/ids.h>

namespace generationRules {
namespace generator {

/// Class that provides information about current scope.
class CurrentScope
{
public:
	CurrentScope();

	/// Methods for work with stack of current "this" elements.
	/// Changes current "this" id (when callGeneratorFor is called, "this" changes).
	/// @param newCurrentVariableId - new "this" id.
	void changeCurrentId(const qReal::Id &newCurrentVariableId);

	/// Returns current variable ("this").
	qReal::Id currentId() const;

	/// Removes last added id from stack of ids.
	void removeLastCurrentId();

	/// Changes current generator name.
	void changeCurrentGeneratorName(const QString &newGeneratorName);

	/// Returns current generator name.
	QString currentGeneratorName() const;

private:
	QStack<qReal::Id> mStackOfThis;
	QString mCurrentGeneratorName;
};

}
}

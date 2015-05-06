#pragma once

#include <QtCore/QHash>
#include <QtCore/QString>

#include <qrkernel/ids.h>

namespace generationRules {
namespace generator {

class VariablesTable
{
public:
	VariablesTable();

	struct VariableData {
	public:
		VariableData()
			: mIdListIterator(qReal::IdList())
		{
		}

		VariableData(const QString &type, const qReal::IdList &listOfIds)
			: mType(type)
			, mListOfIds(listOfIds)
			, mIdListIterator(mListOfIds)
		{
			mCurrentId = mListOfIds.first();
		}

		qReal::Id currentId() const
		{
			return mCurrentId;
		}

		QString type() const
		{
			return mType;
		}

		void moveToNextId() const
		{
			if (mIdListIterator.hasNext()) {
				mCurrentId = mIdListIterator.next();
			}
		}

		QString stringRepresentation() const
		{
			return mType;
		}
	private:
		QString mType;
		qReal::IdList mListOfIds;
		mutable qReal::Id mCurrentId;
		mutable QListIterator<qReal::Id> mIdListIterator;
	};

	void addNewVariable(const QString &name, const QString &type, const qReal::IdList &listOfIds);
	void removeVariable(const QString &name);

	void clear();

	QString textRepresentation() const;

	bool containsVariable(const QString &name) const;
	QString typeByName(const QString &name) const;

	qReal::Id currentId(const QString &variableName) const;
	void movePointer(const QString &variableName);

private:
	QHash<QString, VariableData> mHashTable;
};

}
}

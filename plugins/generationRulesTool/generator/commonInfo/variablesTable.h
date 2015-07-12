/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QHash>
#include <QtCore/QStack>
#include <QtCore/QString>

#include <qrkernel/ids.h>

namespace generationRules {
namespace generator {

/// Class that provides information about identifiers: their name, type and list of ids.
class VariablesTable
{
public:
	VariablesTable();
	~VariablesTable();

	/// Structure with information about variable.
	struct VariableData {
	public:
		/// Default constructor.
		VariableData()
			: mIdListIterator(qReal::IdList())
		{
		}

		/// Constructor.
		/// @param type - variable type.
		/// @param listOfIds - list of ids to iterate.
		VariableData(const QString &type, const qReal::IdList &listOfIds)
			: mType(type)
			, mListOfIds(listOfIds)
			, mIdListIterator(mListOfIds)
		{
			mCurrentId = mIdListIterator.next();
		}

		/// Returns current id in list.
		qReal::Id currentId() const
		{
			return mCurrentId;
		}

		/// Returns variable type.
		QString type() const
		{
			return mType;
		}

		/// Moves pointer to the next id.
		void moveToNextId()
		{
			if (mIdListIterator.hasNext()) {
				mCurrentId = mIdListIterator.next();
			}
		}

		/// Returns string representation for output.
		QString stringRepresentation() const
		{
			int length = mListOfIds.length();

			QString result;
			for (const auto id : mListOfIds) {
				result += id.toString();
			}

			return mType + " " + QString::number(length) + " " + result;
		}

		/// Returns true if next element exists.
		bool nextExists()
		{
			return mIdListIterator.hasNext();
		}

	private:
		QString mType;
		qReal::IdList mListOfIds;
		qReal::Id mCurrentId;
		QListIterator<qReal::Id> mIdListIterator;
	};

	/// Adds new variable to the table.
	/// @param name - new variable name.
	/// @param type - variable type.
	/// @param listOfIdds - list of ids to iterate.
	void addNewVariable(const QString &name, const QString &type, const qReal::IdList &listOfIds);

	/// Removes variable from the table.
	/// @param name - variable name.
	void removeVariable(const QString &name);

	/// Clears the table.
	void clear();

	/// Returns string representation of the table (for debug).
	QString textRepresentation() const;

	/// Returns true if variable with given name exists.
	/// @param name - variable name.
	bool containsVariable(const QString &name) const;

	/// Returns type of variable.
	/// @param name - variable name.
	QString typeByName(const QString &name) const;

	/// Returns current id in iterated list.
	/// @param variableName - variable name.
	qReal::Id currentId(const QString &variableName);

	/// Moves pointer in ids list to next id.
	/// @param variableName - variable name.
	void movePointer(const QString &variableName);

	/// Returns true if next element in list exists.
	/// @param variableName - variable name.
	bool nextIdExists(const QString &variableName);

private:
	QHash<QString, VariableData*> mHashTable;  // has ownership.
};

}
}

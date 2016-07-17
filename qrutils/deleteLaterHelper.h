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

#include <QtCore/QObject>

namespace utils {

/// A helper class that shedules deletion for the given object instead of deleting it immediately.
/// An analogue of QObject::deleteLater, but can be use for arbitrary type, not only QObject subtypes.
template<typename T>
class DeleteLaterHelper : public QObject
{
public:
	/// Shedules deletion for the given object and deletes when Qt event loop will process this event
	/// instead of deleting in immediately.
	/// An analogue of QObject::deleteLater, but can be use for arbitrary type, not only QObject subtypes.
	static void deleteLater(T *toDelete)
	{
		static_cast<QObject *>(new DeleteLaterHelper<T>(toDelete))->deleteLater();
	}

private:
	DeleteLaterHelper(T *toDelete)
		: mToDelete(toDelete)
	{
	}

	~DeleteLaterHelper()
	{
		delete mToDelete;
	}

	T *mToDelete;
};

}

#pragma once

#include <QtCore/QObject>

namespace utils {

/// A helper class that shedules deletion for the given object instead of deletting in immediately.
/// An analogue of QObject::deleteLater, but can be use for arbitrary type, not only QObject subtypes.
template<typename T>
class DeleteLaterHelper : public QObject
{
public:
	/// Shedules deletion for the given object and deletes when Qt event loop will process this event
	/// instead of deletting in immediately.
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

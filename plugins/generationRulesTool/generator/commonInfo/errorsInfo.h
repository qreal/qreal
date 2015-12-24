#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>

namespace generationRules {
namespace generator {

/// Class that provides information about occured errors.
class ErrorsInfo
{
public:
	/// Constructor.
	ErrorsInfo()
		: mErrorsList()
	{
	}

	/// Adds error to error list.
	/// @param errorText - information about error.
	void addError(const QString &errorText)
	{
		mErrorsList.push_back(errorText);
	}

	/// Returns list of errors.
	QStringList errorsList() const
	{
		return mErrorsList;
	}

private:
	QStringList mErrorsList;
};

}
}

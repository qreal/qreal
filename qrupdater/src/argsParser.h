#pragma once

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QMultiMap>

#include <qrkernel/exception/exception.h>
#include <qrkernel/version.h>

namespace qrUpdater {

/// Parses command line arguments and returns each of them in convenient form.
class ArgsParser
{
public:
	/// Thrown when updater cannot accept user command line input.
	class BadArgumentsException : public qReal::Exception
	{
	public:
		explicit BadArgumentsException(QString const &message);
	};

	ArgsParser();

	/// Parses command line args. May throw BadArgumentsException when something is wrong.
	void parse() throw (BadArgumentsException);

	/// @return List of module names what would be found and installed.
	QStringList units() const;

	/// @return Url to the map file "unit <-> fileURL".
	QString detailsUrl() const;

	/// @return Version that currently installed on the host.
	qReal::Version version() const;

	/// @return True if updates installation must be started immediatly.
	bool hardUpdate() const;

private:
	bool hasEmptyArgs() const;

	/// Help message that user gets if entered wrong arguments.
	QString const mInputMask;
	bool mHasHardUpdateParam;
	QStringList mKeywords;
	QMultiMap<QString, QString> mParams;
};

}


#pragma once

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QMultiMap>

#include <qrkernel/exception/exception.h>
#include <qrkernel/version.h>

namespace qrUpdater {

/// Represents a mode that will be used by updater in the current session.
enum Mode {
	/// Updater will just retrieve from the server light-weight about available versions
	/// and write into standard output information about the newest versions.
	check = 0
	/// Updater will check for the new versions and download their installers if such present.
	, download
	/// Updater will install already downloaded versions.
	, install
	/// Updater will download new updates and install them immediately.
	, downloadAndInstall
};

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

	/// Returns a mode in which updater should work.
	Mode mode() const;

	/// @return List of module names what would be found and installed.
	QStringList units() const;

	/// @return Url to the map file "unit <-> fileURL".
	QString detailsUrl() const;

	/// @return Absolute or relative path to an updated appication.
	QString pathToApplication() const;

	/// @return Version that currently installed on the host.
	qReal::Version version() const;

private:
	bool parseMode(QString const &input);
	bool hasEmptyArgs() const;

	/// Help message that user gets if entered wrong arguments.
	QString const mInputMask;
	Mode mMode;
	QStringList mKeywords;
	QMultiMap<QString, QString> mParams;
};

}


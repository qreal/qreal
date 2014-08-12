#include "argsParser.h"

using namespace qrUpdater;

ArgsParser::ArgsParser()
	: mInputMask(
			"Usage:\n"
			"\tqrmaintenance (--check | --download | --install | --download-and-install) "
			"-unit [unit-name] -version [version] -url [url-to-file] -path [path-to-updated-app]\n"
			"Modes:"
			"\t--check: Updater will just retrieve from the server light-weight about available versions "
			"and write into standard output information about the newest versions.\n"
			"\t--download: Updater will check for the new versions and download their installers if such present.\n"
			"\t--install: Updater will install already downloaded versions.\n"
			"\t--download-and-install: Updater will download new updates and install them immediately.\n"
			"\n"
			"Example:\n qrmaintenance --download -unit cool-module-name -version 2.8a "
			"-url http://example.com/updates.xml -path ./qrgui\n"
	)
{
	mKeywords << "-unit" << "-version" << "-url" << "-path";
}

void ArgsParser::parse() throw (BadArgumentsException)
{
	QStringList const arguments = QCoreApplication::arguments();

	// First argument is application name, second - mode, then keywords
	if (arguments.size() < mKeywords.size() * 2 + 2 || !parseMode(arguments[1])) {
		throw BadArgumentsException(mInputMask);
	}

	QString curParam;
	for (int i = 2; i < arguments.size(); ++i) {
		if (mKeywords.contains(arguments.at(i).toLower())) {
			// If we got on the key then storing it and proceeding iterations.
			curParam = arguments.at(i).toLower();
			continue;
		}

		if (!curParam.isEmpty()) {
			// Else we got value bound to the previous key.
			mParams.insertMulti(curParam, arguments.at(i));
		}
	}

	if (hasEmptyArgs()) {
		throw BadArgumentsException(mInputMask);
	}
}

Mode ArgsParser::mode() const
{
	return mMode;
}

QStringList ArgsParser::units() const
{
	return mParams.values("-unit");
}

QString ArgsParser::detailsUrl() const
{
	return mParams.value("-url");
}

qReal::Version ArgsParser::version() const
{
	return qReal::Version::fromString(mParams.value("-version"));
}

QString ArgsParser::pathToApplication() const
{
	return mParams.value("-path");
}

bool ArgsParser::parseMode(QString const &input)
{
	if (input == "--check") {
		mMode = check;
		return true;
	}

	if (input == "--download") {
		mMode = download;
		return true;
	}

	if (input == "--install") {
		mMode = install;
		return true;
	}

	if (input == "--download-and-install") {
		mMode = downloadAndInstall;
		return true;
	}

	return false;
}

bool ArgsParser::hasEmptyArgs() const
{
	for (QString const &keyword : mKeywords) {
		if (mParams.values(keyword).isEmpty()) {
			return true;
		}
	}

	return false;
}

ArgsParser::BadArgumentsException::BadArgumentsException(QString const &message)
	: qReal::Exception(message)
{
}

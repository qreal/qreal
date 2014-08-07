#include "argsParser.h"

using namespace qrUpdater;

ArgsParser::ArgsParser()
	: mInputMask(
			"Usage:\n"
			"qrupdater -unit [unit-name] -version [version] -url [url-to-file] | [-hard]\n"
			"Example:\n qrupdater -unit windows -version 0.3.2 -url http://127.0.0.1/updates.xml\n"
			"or\n"
			"qrupdater -unit cool-module-name -version 2.8a -url http://example.com/updates.xml -hard\n"
	)
	, mHasHardUpdateParam(false)
{
	mKeywords << "-unit" << "-version" << "-url";
}

void ArgsParser::parse() throw (BadArgumentsException)
{
	QStringList const arguments = QCoreApplication::arguments();

	if (arguments.size() < mKeywords.size() * 2) {
		throw BadArgumentsException(mInputMask);
	}

	mHasHardUpdateParam = arguments.contains("-hard", Qt::CaseInsensitive);

	QString curParam;
	for (int i = 0; i < arguments.size(); ++i) {
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

bool ArgsParser::hardUpdate() const
{
	return mHasHardUpdateParam;
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

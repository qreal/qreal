#include "argsParser.h"

using namespace qrUpdater;

ArgsParser::ArgsParser()
	: mHasHardUpdateParam(false)
	, mInputMask(
			"usage:\n"
			"qrUpdater -unit [unit-name] -version [version] -url [url-to-file] | [-hard]\n"
			"example:\n updater -unit windows -version 0.3.2 -url http://127.0.0.1/updates.xml\n"
			"or\n"
			"qrUpdater -unit cool-module-name -version 2.8a -url http://example.com/updates.xml -hard\n"
	)

{
	mKeywords << "-unit" << "-version" << "-url";
}

void ArgsParser::parse() throw (BadArguments)
{
	QStringList const arguments = QCoreApplication::arguments();

	if (arguments.size() < mKeywords.size() * 2) {
		throw BadArguments(mInputMask);
	}

	mHasHardUpdateParam = arguments.contains("-hard", Qt::CaseInsensitive);

	QString curParam = "";
	for(int i = 0; i < arguments.size(); i++) {
		if (mKeywords.contains(arguments.at(i).toLower())) {
			curParam = arguments.at(i).toLower();
			continue;
		}

		if (curParam.isEmpty()) {
			continue;
		}

		mParams.insertMulti(curParam, arguments.at(i));
	}

	if (hasEmptyArgs()) {
		throw BadArguments(mInputMask);
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

QString ArgsParser::version() const
{
	return mParams.value("-version");
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

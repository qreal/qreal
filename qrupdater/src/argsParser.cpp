#include "argsParser.h"

ArgsParser::ArgsParser()
	: hasHardUpdateParam(false)
{
	mKeywords << "-unit" << "-version" << "-url";
}

void ArgsParser::parse() throw (BadArguments)
{
	QStringList arguments = QCoreApplication::arguments();

	if (arguments.size() < mKeywords.size() * 2) {
		throw BadArguments();
	}
	hasHardUpdateParam = arguments.contains("-hard", Qt::CaseInsensitive);

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
		throw BadArguments();
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
	return hasHardUpdateParam;
}

bool ArgsParser::hasEmptyArgs() const
{
	foreach (QString keyword, mKeywords) {
		if (mParams.values(keyword).isEmpty()) {
			return true;
		}
	}
	return false;
}

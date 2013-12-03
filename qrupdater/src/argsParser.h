#pragma once

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QMultiMap>

namespace qrUpdater {

//! @class ArgsParser parses arguments of command line
//! determine what of modules to update
class ArgsParser
{
public:
	class Error{};
	class BadArguments: public Error{};

	ArgsParser();

	void parse() throw (BadArguments);

	QStringList units() const;
	QString detailsUrl() const;
	QString version() const;
	bool hardUpdate() const;

protected:
	bool hasEmptyArgs() const;

	QStringList mKeywords;
	QMultiMap<QString, QString> mParams;
	bool hasHardUpdateParam;
};

}


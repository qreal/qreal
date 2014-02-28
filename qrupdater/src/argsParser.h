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
	class Error
	{
	public:
		Error(QString const &error)
			: errorMsg(error)
		{}

		QString errorMsg;
	};

	class BadArguments: public Error
	{
	public:
		BadArguments(QString const &error)
			: Error(error)
		{}
	};


	ArgsParser();

	//! Parses command line args, call it before use other
	void parse() throw (BadArguments);

	//! @return list of module names what we'l find and install
	QStringList units() const;
	//! @return url to map file "unit <-> fileURL"
	QString detailsUrl() const;
	//! @return current host program version
	QString version() const;
	//! @return True if we'll start update installing immediatly
	bool hardUpdate() const;

protected:
	bool hasEmptyArgs() const;

	bool mHasHardUpdateParam;
	//! Description of arguments
	QString const mInputMask;
	QStringList mKeywords;
	QMultiMap<QString, QString> mParams;
};

}


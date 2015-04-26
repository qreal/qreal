#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>

namespace git
{
struct Tag
{
	Tag()
	{
	}

	Tag(QString const &oper, QString const &strTag = QString(), bool bTag = false)
		: operation(oper)
		, stringTag(strTag)
		, boolTag(bTag)
	{
	}

	QString operation;
	QString stringTag;
	bool boolTag;
};
}

Q_DECLARE_METATYPE(git::Tag)

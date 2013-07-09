#pragma once

#include <QtCore/QObject>
#include <QString>

namespace versioning
{
struct Tag
{
	Tag()
	{
	}

	Tag(QString const &oper, QString const &strTag, bool bTag)
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

Q_DECLARE_METATYPE(versioning::Tag)

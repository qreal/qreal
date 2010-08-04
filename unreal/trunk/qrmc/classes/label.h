#pragma once

#include <QtXml/QDomElement>

namespace utils {
	class OutFile;
}

class MetaCompiler;

class Label
{
public:
	// Default copy constructor is ok.

	bool init(QDomElement const &element, int index, bool nodeLabel);

	QString generateInit(MetaCompiler *compiler, bool isNode) const;
	QString generateUpdate(MetaCompiler *compiler) const;
	QString generateDefinition(MetaCompiler *compiler) const;

private:
	QString titleName() const;

	QString mX;
	QString mY;
	int mIndex;
	QString mText;
	QString mTextBinded;
	QString mReadOnly;
	QString mBackground;
	QString mCenter;
};

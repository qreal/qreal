#pragma once

#include <QtXml/QDomElement>
#include "scalableCoordinate.h"

namespace qrmc {
	class MetaCompiler;

	class Label
	{
	public:
		// Default copy constructor is ok.

		bool init(QDomElement const &element, int index, bool nodeLabel, int width, int height);

		QString generateInit(MetaCompiler *compiler, bool isNode) const;
		QString generateUpdate(MetaCompiler *compiler) const;
		QString generateDefinition(MetaCompiler *compiler) const;

	private:
		void initCoordinate(ScalableCoordinate &field, QString coordinate, int maxValue);
		QString titleName() const;

		ScalableCoordinate mX;
		ScalableCoordinate mY;
		int mIndex;
		QString mText;
		QString mTextBinded;
		QString mReadOnly;
		QString mBackground;
		QString mCenter;
	};
}

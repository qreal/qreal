#pragma once

#include <QDomElement>

#include "port.h"
#include "scalableCoordinate.h"

namespace qrmc {
	class LinePort : public Port
	{
	public:
		virtual bool init(QDomElement const &element, int width, int height);

		virtual Port* clone() const;

		virtual QString generateSdf(MetaCompiler *compiler) const;
		virtual QString generateInit(MetaCompiler *compiler) const;

	private:
		ScalableCoordinate mStartX;
		ScalableCoordinate mStartY;
		ScalableCoordinate mEndX;
		ScalableCoordinate mEndY;

		int mWidth;
		int mHeight;

		void initCoordinate(ScalableCoordinate &field, QString coordinate, int maxValue);
		QString generate(const QString &lineTemplate, bool isScaled) const;
	};
}

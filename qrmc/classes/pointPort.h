#pragma once

#include "port.h"

#include <QDomElement>

namespace qrmc {
	class MetaCompiler;

	class PointPort : public Port
	{
	public:
		virtual bool init(const QDomElement &element, int width, int height);
		virtual Port* clone() const;

		virtual QString generateSdf(MetaCompiler *compiler) const;
		virtual QString generateInit(MetaCompiler *compiler) const;
		virtual void generatePortList(const QStringList &portTypes);//oldfix

	private:
		qreal mX;
		qreal mY;
		int mWidth;
		int mHeight;
	};
}

#pragma once

#include <QDomElement>

namespace qrmc {
	class MetaCompiler;

	class Port
	{
	public:
		virtual bool init(QDomElement const &element, int width, int height) = 0;
		virtual Port *clone() const = 0;

		virtual QString generateSdf(MetaCompiler *compiler) const = 0;
		virtual QString generateInit(MetaCompiler *compiler) const = 0;
	protected:
		MetaCompiler *mCompiler;
	};
}

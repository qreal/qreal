#pragma once

#include <QDomElement>
#include <QtCore/QStringList>

namespace qrmc {
	class MetaCompiler;

	class Port
	{
	public:
		virtual ~Port() {};
		virtual bool init(const QDomElement &element, int width, int height) = 0;
		virtual Port *clone() const = 0;

		virtual QString generateSdf(MetaCompiler *compiler) const = 0;
		virtual QString generateInit(MetaCompiler *compiler) const = 0;
		virtual void generatePortList(const QStringList &portTypes) = 0;//fix

		QString type() const;//fix

	protected:
		MetaCompiler *mCompiler;
		QString mType;//fix
	};
}

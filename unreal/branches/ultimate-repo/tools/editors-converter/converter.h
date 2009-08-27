#pragma once

#include <QtCore>
#include <QtXml>

namespace converter
{

	class Converter
	{
	public:
		Converter(QStringList editors);
	private:
		void convert(QDomDocument &document, QString const &name);
		QDomDocument readXml(QString const &fileName);
		void writeXml(QDomDocument const &document, QString const &fileName);
	};

}

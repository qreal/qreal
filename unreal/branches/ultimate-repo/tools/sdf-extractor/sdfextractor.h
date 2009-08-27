#pragma once

#include <QDomNode>
#include <QDomDocument>

namespace sdfExtractor
{

	class SdfExtractor
	{
	public:
		SdfExtractor(QString const &fileName);
	private:
		QDomDocument readXml(QString const &fileName) const;
		void generateSdf(QDomNode const &svgNode, QString const &fileName,
			bool generateSvg) const;
		void processDomList(QDomNodeList const &list) const;
	};

}

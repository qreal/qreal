#pragma once

#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

namespace qrRepo {
namespace details {

class GraphicalPart
{
public:
	GraphicalPart();
	explicit GraphicalPart(QDomElement const &element);
	QVariant property(QString const &name) const;
	void setProperty(QString const &name, const QVariant &value);
	GraphicalPart *clone() const;
	QDomElement serialize(int index, QDomDocument &document) const;

private:
	QMap<QString, QVariant> mProperties;
};

}
}

#pragma once

#include <QtCore/QVariant>
#include <QtCore/QString>

namespace qrRepo {
namespace details {

class GraphicalPart
{
public:
	explicit GraphicalPart();
	QVariant property(QString const &name) const;
	void setProperty(QString const &name, const QVariant &value);
	GraphicalPart *clone() const;

private:
	QMap<QString, QVariant> mProperties;
};

}
}

#pragma once

#include "label.h"
#include "pointPort.h"
#include "linePort.h"

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QDir>

namespace qrmc {
class GraphicType;

class Shape
{
public:
	Shape(QString const &shape = "");
	~Shape();

	void init(QString const &shape);
	void setNode(GraphicType *node);

	void generate(QString &classTemplate) const;

	QString generateResourceLine(QString const &resourceTemplate) const;

private:
	void initLabels(QDomElement const &graphics);
	void initPorts(QDomElement const &graphics);
	void initPointPorts(QDomElement const &portsElement);
	void initLinePorts(QDomElement const &portsElement);

	void changeDir(QDir &dir) const;

	void generateSdf() const;
	void generatePortsSdf() const;

	bool hasLabels() const;
	bool hasPointPorts() const;
	bool hasLinePorts() const;
	bool hasPicture() const;
	bool hasPorts() const;

	int mWidth;
	int mHeight;
	QString mPicture;
	QList<Label*> mLabels;
	QList<Port*> mPorts;
	GraphicType *mNode;
};
}

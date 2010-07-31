#pragma once

#include "label.h"
#include "pointPort.h"
#include "linePort.h"

#include <QtCore/QList>
#include <QtCore/QString>

class GraphicType;

class Shape
{
public:
	Shape(QString const &shape = "");
	~Shape();

	void init(QString const &shape, GraphicType *node);

	void generateSdf() const;

private:
	void initLabels(QDomElement const &graphics);
	void initPorts(QDomElement const &graphics);
	void initPointPorts(QDomElement const &portsElement);
	void initLinePorts(QDomElement const &portsElement);

	int mWidth;
	int mHeight;
	QString mPicture;
	QList<Label*> mLabels;
	QList<Port*> mPorts;
	GraphicType *mNode;
};


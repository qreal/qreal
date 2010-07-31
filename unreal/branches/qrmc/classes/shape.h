#pragma once

#include "label.h"
#include "pointPort.h"
#include "linePort.h"

#include <QtCore/QList>
#include <QtCore/QString>

class Shape
{
public:
	Shape(QString const &shape);

	void init(QString const &shape);

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
};


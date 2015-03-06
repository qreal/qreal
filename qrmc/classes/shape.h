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
		Shape(const QString &shape = "");
		~Shape();

		void init(const QString &shape);
		void setNode(GraphicType *node);

		void generate(QString &classTemplate) const;

		QString generateResourceLine(const QString &resourceTemplate) const;

	private:
		void initLabels(QDomElement const &graphics);
		void initPorts(QDomElement const &graphics);
		void initPointPorts(QDomElement const &portsElement);
		void initLinePorts(QDomElement const &portsElement);

		void changeDir(QDir &dir) const;

		void generateSdf() const;

		bool hasLabels() const;
		bool hasPointPorts() const;
		bool hasLinePorts() const;
		bool hasPicture() const;

		int mWidth;
		int mHeight;
		QString mPicture;
		QList<Label*> mLabels;
		QList<Port*> mPorts;
		GraphicType *mNode;
	};
}

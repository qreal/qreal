#pragma once

#include <QtCore/QString>
#include <QtGui/QPainter>

class PortImpl
{
public:
	virtual ~PortImpl() {}

	virtual QString type() const = 0;
};

class NonTyped : public PortImpl
{
public:
	virtual QString type() const {
		return "NonTyped";
	}
};

class PortInterface
{
public:
	PortInterface(PortImpl *impl) : mImpl(impl) {}
	virtual ~PortInterface() { delete mImpl; }

	virtual void paint(QPainter *painter, QRectF const &contents) const = 0;
	QString type() const { return mImpl->type(); }

protected:
	PortImpl *mImpl;
};

class PortFactoryInterface
{
public:
	virtual PortInterface *createPort(QPointF const &point, bool propX, bool propY, int initWidth, int initHeight
			, PortImpl *impl) const = 0;
	virtual PortInterface *createPort(QLineF const &line, bool propX1, bool propY1, bool propX2, bool propY2
			, int initWidth, int initHeight, PortImpl *impl) const = 0;
};

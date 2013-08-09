#pragma once

#include <QtCore/QString>
#include <QtGui/QPainter>

/** @brief Abstract class for generated port types */
class PortImpl
{
public:
	virtual ~PortImpl() {}

	virtual QString type() const = 0;
};

class NonTyped : public PortImpl
{
public:
	virtual QString type() const
	{
		return "NonTyped";
	}
};

/** @brief A base class for classes that represent element's ports on a diagram */
class PortInterface
{
public:
	PortInterface(PortImpl *impl) : mImpl(impl) {}
	virtual ~PortInterface()
	{
		delete mImpl;
	}

	virtual void paint(QPainter *painter, QRectF const &contents) const = 0;
	QString type() const
	{
		return mImpl->type();
	}

protected:
	PortImpl *mImpl;
};

/** @brief An interface for creation of PortInterface implementations */
class PortFactoryInterface
{
public:
	virtual PortInterface *createPort(QPointF const &point, bool propX, bool propY, int initWidth, int initHeight
			, PortImpl *impl) const = 0;
	virtual PortInterface *createPort(QLineF const &line, bool propX1, bool propY1, bool propX2, bool propY2
			, int initWidth, int initHeight, PortImpl *impl) const = 0;
};

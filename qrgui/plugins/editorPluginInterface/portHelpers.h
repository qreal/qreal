/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

	virtual void paint(QPainter *painter, const QRectF &contents) const = 0;
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
	virtual ~PortFactoryInterface() {}

	virtual PortInterface *createPort(const QPointF &point, bool propX, bool propY, int initWidth, int initHeight
			, PortImpl *impl) const = 0;
	virtual PortInterface *createPort(const QLineF &line, bool propX1, bool propY1, bool propX2, bool propY2
			, int initWidth, int initHeight, PortImpl *impl) const = 0;
};

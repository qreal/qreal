/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

#include <QtXml/QDomElement>
#include <QtCore/QStringList>

namespace qrmc {

class MetaCompiler;

/// Represents a port, i.e. a place on a node where edges can be connected to.
class Port
{
public:
	virtual ~Port() {}

	/// Initializes port.
	/// @param element - XML document part with information about this port.
	/// @param width - initial width of a shape to which this port is attached.
	/// @param height - initial height of a shape to which this port is attached.
	virtual bool init(const QDomElement &element, int width, int height) = 0;

	/// Creates a copy of this port.
	virtual Port *clone() const = 0;

	/// Generates shape of this port as part of SDF document.
	virtual QString generateSdf(const MetaCompiler &compiler) const = 0;
	virtual QString generateInit(const MetaCompiler &compiler) const = 0;
	virtual void generatePortList(const QStringList &portTypes) = 0;

	/// Returns type of the port. Using types we can restrict certain edges to certain ports.
	QString type() const;

protected:
	/// Type of a port.
	QString mType;
};

}

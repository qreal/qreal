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

#include <QtXml/QDomElement>
#include <QtCore/QMap>

class Property;
class Diagram;

namespace utils {
	class OutFile;
}

class Type
{
public:
	Type(bool isResolved, Diagram *diagram);
	virtual ~Type();
	virtual Type* clone() const = 0;
	virtual bool init(const QDomElement &element, const QString &context);
	virtual bool resolve() = 0;
	virtual bool isResolving() const;
	bool isResolved() const;

	QString name() const;
	QString path() const;
	QString qualifiedName() const;
	QString displayedName() const;

	QMap<QString, Property*> properties() const;

	void setName(const QString &name);
	void setDiagram(Diagram *diagram);
	void setContext(const QString &newContext);
	void setDisplayedName(const QString &displayedName);
	virtual void generateCode(utils::OutFile &out) = 0;
	virtual void generateNameMapping(utils::OutFile &out) = 0;
	virtual bool generateObjectRequestString(utils::OutFile &out, bool isNotFirst) = 0;
	virtual bool generateProperties(utils::OutFile &out, bool isNotFirst, bool isReference) = 0;
	virtual bool generatePorts(utils::OutFile &out, bool isNotFirst) = 0;
	virtual bool generateContainedTypes(utils::OutFile &out, bool isNotFirst) = 0;
	virtual bool generatePossibleEdges(utils::OutFile &out, bool isNotFirst) = 0;
	virtual bool generateEnumValues(utils::OutFile &out, bool isNotFirst) = 0;
	virtual void generatePropertyTypes(utils::OutFile &out) = 0;
	virtual void generatePropertyDefaults(utils::OutFile &out) = 0;
	virtual void generatePropertyDescriptionMapping(utils::OutFile &out) = 0;
	virtual void generateMouseGesturesMap(utils::OutFile &out) = 0;
	virtual void generateExplosionsMap(utils::OutFile &out) = 0;

protected:
	void copyFields(Type *type) const;
	QString nativeContext() const;

	QMap<QString, Property*> mProperties;
	bool mResolvingFinished;
	Diagram *mDiagram;

private:
	/// Unqualified metatype name
	QString mName;
	/// Qualification context. For example for 'Kernel::Node' 'Node' is the name and 'Kernel' is the context.
	QString mContext;
	/// "Native" qualification context, type import does not modify it and is used for resolve.
	QString mNativeContext;
	QString mDisplayedName;
	QString mPath;
};

/* Copyright 2007-2016 QReal Research Group
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

#include <QtXml/QDomDocument>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QDir>
#include <QtCore/QPair>
#include <QtCore/QSet>

class XmlCompiler;
class Diagram;
class Type;
class NodeType;
class EnumType;
class PortType;

namespace utils {
	class OutFile;
}

class Editor
{
public:
	Editor(QDomDocument domDocument, XmlCompiler *xmlCompiler);
	~Editor();

	XmlCompiler *xmlCompiler() const;

	QString version() const;

	bool isLoaded();
	bool load(const QDir &currentDir);

	Type *findType(const QString &name);
	Type *findTypeByNormalizedName(const QString &name);

	QSet<EnumType *> getAllEnumTypes();
	Diagram *findDiagram(const QString &name);
	QMap<QString, Diagram*> diagrams();

	/// Returns name of editor that is extended by this metamodel. In most cases this method will return empty string.
	/// If some value is returned then this editor is some kind of "plugin" to the returned one.
	QString extendedEditor() const;

private:
	QString resolveInclude(const QDomElement &includeElement, const QDir &currentDir);

	XmlCompiler *mXmlCompiler;
	QDomDocument mXmlDomDocument;
	bool mLoadingComplete;
	QString mExtendedMetamodel;
	QString mVersion;
	QList<Editor*> mIncludes;
	QMap<QString, Diagram*> mDiagrams;
};

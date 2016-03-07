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
#include <QtXml/QDomElement>
#include <QtCore/QMap>
#include <QtCore/QPair>

class Type;
class Editor;

class Diagram
{
public:
	Diagram(const QString &name, const QString &nodeName, const QString &displayedName, Editor *editor);
	~Diagram();
	bool init(const QDomElement &diagramElement);
	bool resolve();
	Editor *editor() const;
	Type *findType(QString name);
	QMap<QString, Type*> types() const;
	QString name() const;
	QString nodeName() const;
	QString displayedName() const;
	QString getGroupsXML() const;
	QList<QPair<QString, QStringList>> paletteGroups() const;
	QMap<QString, QString> paletteGroupsDescriptions() const;
	bool shallPaletteBeSorted() const;

private:
	struct ImportSpecification {
		QString name;
		QString as;
		QString displayedName;
	};

	QMap<QString, Type*> mTypes;
	QString mDiagramName;
	QString mDiagramNodeName;
	QString mDiagramDisplayedName;
	Editor *mEditor;
	QString mGroupsXML;
	QList<QPair<QString, QStringList>> mPaletteGroups;
	QMap<QString, QString> mPaletteGroupsDescriptions;
	QList<ImportSpecification> mImports;
	bool mShallPaletteBeSorted;

	bool initGraphicTypes(const QDomElement &graphicTypesElement);
	bool initNonGraphicTypes(const QDomElement &nonGraphicTypesElement);
	void initPalette(const QDomElement &paletteElement);
};

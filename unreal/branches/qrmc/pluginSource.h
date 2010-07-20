#pragma once

#include <QtCore/QString>
#include "../../trunk/qrrepo/repoApi.h"

QString const importType = "MetaEntityImport";
QString const metamodelDiagramName = "MetamodelDiagram";

QString const setShapeProperty = "set Shape";

class PluginSource
{
public:
	PluginSource(QString const &name);
	~PluginSource();

	void init(qrRepo::RepoApi &repo, qReal::Id metamodelId);
	bool generate(QString const &sourceTemplate, QMap<QString, QString> const &utils);

private:

	struct Property { // smells like bullshit. replace with classes?
		QString name;
		QString type;
		QString defaultValue;
	};

	struct Element {
		QString name;
		QString displayedName;
		qReal::Id id;
		bool isGraphicalObject;
		QList<Property> properties;
		QList<qReal::Id> parents;
	};

	struct Diagram : public Element {
		QList< Element > elements;
	};
	void initDiagram(qReal::Id &diagramId);
	void initProperties(qReal::Id &id);
	void initElement(Element &el, qReal::Id &id);

	void generateDiagramsMap();
	void generateElementsMap();
	void generateMouseGesturesMap();
	void generatePropertyTypesMap();
	void generateGetGraphicalObject();

	void resolveImports();
	void resolveElements();
	void resolveElementProperties();
	void updateIsGraphicalProperty();

	qReal::Id findElement(QString diagram, QString name);

	qrRepo::RepoApi *mApi;
	QString mName;
	QList<Diagram> mDiagrams;
	QMap<QString, QString> mUtilsTemplate;
	QString mSourceTemplate;
};


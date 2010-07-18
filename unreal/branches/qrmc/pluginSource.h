#pragma once

#include <QtCore/QString>
#include "../../trunk/qrrepo/repoApi.h"

class PluginSource
{
public:
	PluginSource(QString const &name);
	~PluginSource();

	void init(qrRepo::RepoApi &repo, qReal::Id metamodelId);
	bool generate(QString const &sourceTemplate, QMap<QString, QString> const &utils);
	void generateDiagramsMap();
	void generateElementsMap();
	void generateMouseGesturesMap();
	void generatePropertyTypesMap();

private:

	struct Element { // this is bullshit. replace with classes?
		QString name;
		QString displayedName;
		qReal::Id id;
	};

	struct Diagram : public Element {
		QList< Element > elements;
	};

	void initDiagram(qrRepo::RepoApi &repo, qReal::Id diagramId);

	qrRepo::RepoApi *mApi;
	QString mName;
	QList<Diagram> mDiagrams;
	QMap<QString, QString> mUtilsTemplate;
	QString mSourceTemplate;
};


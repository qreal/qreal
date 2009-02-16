#include <QDomDocument>
#include <QDomNode>
#include <QFile>
#include "generator.h"
#include "editor_file.h"

EditorFile::EditorFile(QString filename, Generator *gen)
{
	generator = gen;
	name = filename;
	uniq_name = name.split("/").last();
	loading_done = false;
}

EditorFile::~EditorFile()
{
	while (!categories.isEmpty())
		delete categories.takeFirst();
}

bool EditorFile::load(void)
{
	QFile file(name);
	QDomDocument *doc;
	QDomElement child, metamodel;
	QString error = "";
	int errorLine = 0;
	int errorCol = 0;

	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "cannot open file" << name;
		return false;
	}

	doc = new QDomDocument("+1");
	if( !doc->setContent(&file, false, &error, &errorLine, &errorCol))
	{
		qDebug() << "parse error in " << name << " at ("
		         << errorLine << "," << errorCol << "): " << error;
		delete doc;
		file.close();
		return false;
	}
	file.close();

	metamodel = doc->firstChildElement("metamodel");
	if (metamodel.isNull())
	{
		qDebug() << "metamodel tag not found in" << name << "file";
		delete doc;
		return false;
	}

	// I. Load include dependencies
	for (child = metamodel.firstChildElement("include"); !child.isNull();
	     child = child.nextSiblingElement("include"))
	{
		QString inc = child.text();
		EditorFile *file;

		inc.append(".xml");
		if (!generator->loadFile(inc, &file))
		{
			qDebug() << "Cannot include file" << inc
			         << "to file " << name;
			delete doc;
			return false;
		}
		includes << file;
	}

	// II. Load diagrams part one:
	// Do not process inherited properties.
	for (child = metamodel.firstChildElement("diagram"); !child.isNull();
	     child = child.nextSiblingElement("diagram"))
	{
		name = child.attribute("name");
		Category *cat = new Category(name, this);
		if (!cat->init(child))
		{
			delete cat;
			delete doc;
			return false;
		}
		categories << cat;
	}

	// III. Load diagram part two:
	// Resolve all dependencies.
	FOR_ALL_CATEGORIES(this, f)
		if (!(*f)->resolve())
		{
			delete doc;
			return false;
		}

	loading_done = true;
	delete doc;
	return true;
}

const Entity* EditorFile::findEntityInIncludes(QString id) const
{
	QList<EditorFile *>::ConstIterator i;
	const Entity *res;

	for (i = includes.constBegin(); i != includes.constEnd(); i++)
	{
		res = (*i)->findEntityInCategories(id);
		if (res) return res;
		res = (*i)->findEntityInIncludes(id);
		if (res) return res;
	}
	return NULL;
}

const Entity* EditorFile::findEntityInCategories(QString id) const
{
	QList<Category *>::ConstIterator i;
	const Entity *res;

	for (i = categories.constBegin(); i != categories.constEnd(); i++)
	{
		res = (*i)->findEntity(id);
		if (res) return res;
	}
	return NULL;
}

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
	Category *cat;
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
		const Category *old_cat;

		QString cat_name = child.attribute("name");
		old_cat = generator->findCategory(cat_name);
		if (old_cat)
		{
			qDebug() << "Error processing file" << name << "Category"
			         << cat_name << "already loaded from file"
			         << old_cat->get_editor()->get_name();
			return false;
		}
		cat = new Category(cat_name, this);
		if (!cat->init(child))
		{
			delete cat;
			delete doc;
			return false;
		}
		categories << cat;
	}
	delete doc;

	// III. Load diagram part two:
	// Resolve all dependencies.
	Q_FOREACH(cat, categories)
		if (!cat->resolve())
			return false;

	loading_done = true;
	return true;
}

const Entity* EditorFile::findEntityInIncludes(QString id) const
{
	const Entity *res;
	const EditorFile *f;

	Q_FOREACH(f, includes)
	{
		res = f->findEntityInCategories(id);
		if (res) return res;
		res = f->findEntityInIncludes(id);
		if (res) return res;
	}
	return NULL;
}

const Entity* EditorFile::findEntityInCategories(QString id) const
{
	const Entity *res;
	const Category *c;

	Q_FOREACH(c, categories)
	{
		res = c->findEntity(id);
		if (res) return res;
	}
	return NULL;
}

const Category* EditorFile::findCategory(QString name) const
{
	Category *c;

	Q_FOREACH(c, categories)
		if (c->get_name() == name)
			return c;
	return NULL;
}

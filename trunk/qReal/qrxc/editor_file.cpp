#include <QDomDocument>
#include <QDomNode>
#include <QFile>
#include "generator.h"
#include "editor_file.h"

EditorFile::EditorFile(QFileInfo filename, Generator *gen)
{
	generator = gen;
	fileinfo = filename;
	loading_done = false;
}

EditorFile::~EditorFile()
{
	while (!editors.isEmpty())
		delete editors.takeFirst();
}

bool EditorFile::load(void)
{
	QFile file(fullPath());
	QDomDocument *doc;
	QDomElement child, metamodel;
	Editor *cat = NULL;
	QString error = "";
	int errorLine = 0;
	int errorCol = 0;

	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "cannot open file" << fullPath();
		return false;
	}

	doc = new QDomDocument("+1");
	if( !doc->setContent(&file, false, &error, &errorLine, &errorCol))
	{
		qDebug() << "parse error in " << fullPath()
				 << " at (" << errorLine << "," << errorCol
				 << "): " << error;
		delete doc;
		file.close();
		return false;
	}
	file.close();

	metamodel = doc->firstChildElement("metamodel");
	if (metamodel.isNull())
	{
		qDebug() << "metamodel tag not found in"
				 << fullPath() << "file";
		delete doc;
		return false;
	}


	// I. Load include dependencies
	for (child = metamodel.firstChildElement("include"); !child.isNull();
		 child = child.nextSiblingElement("include"))
	{
		QString inc = child.text();
		const EditorFile *edfile = 0;

		inc.append(".xml");
		if (!generator->loadFile(inc, &edfile))
		{
			qDebug() << "Cannot include file" << inc
					 << "to file " << fullPath();
			delete doc;
			return false;
		}
		includes << edfile;
	}


	// II. Load diagrams part one:
	// Do not process inherited properties.
	for (child = metamodel.firstChildElement("editor"); !child.isNull();
		 child = child.nextSiblingElement("editor"))
	{
		QString cat_name = child.attribute("name");
		Editor const *old_cat = generator->findEditor(cat_name);
		if (old_cat)
		{
			qDebug() << "Error processing file"
					 << fullPath() << "Editor"
					 << cat_name << "already loaded from file"
					 << old_cat->get_editor()->fullPath();
			return false;
		}
		cat = new Editor(cat_name, this);
		if (!cat->init(child))
		{
			delete cat;
			delete doc;
			return false;
		}
		editors << cat;
	}
	delete doc;

	// III. Load diagram part two:
	// Resolve all dependencies.
	Q_FOREACH(cat, editors)
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
		if( !f )
			continue;
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
	const Editor *c;

	Q_FOREACH(c, editors)
	{
		res = c->findEntity(id);
		if (res) return res;
	}
	return NULL;
}

const Editor* EditorFile::findEditor(QString name) const
{
	Editor *c;

	Q_FOREACH(c, editors)
		if (c->get_name() == name)
			return c;
	return NULL;
}

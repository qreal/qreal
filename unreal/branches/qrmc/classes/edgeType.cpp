#include "edgeType.h"
#include "../utils/outFile.h"
#include "metaCompiler.h"
#include "diagram.h"
#include "editor.h"
#include "utils/nameNormalizer.h"

#include <QDebug>

using namespace utils;

EdgeType::EdgeType(Diagram *diagram, qrRepo::RepoApi *api, const qReal::Id &id) : GraphicType(diagram, api, id)
{
	mLineType = mApi->stringProperty(id, "lineType");
	initLabels();
}

EdgeType::~EdgeType()
{
}

Type* EdgeType::clone() const
{
	EdgeType *result = new EdgeType(mDiagram, mApi, mId);
	GraphicType::copyFields(result);
	result->mBeginType = mBeginType;
	result->mEndType = mEndType;
	result->mLineType = mLineType;
	return result;
}

void EdgeType::print()
{
	qDebug() << "edge" << mName;
}

bool EdgeType::isGraphicalType() const
{
	return !mLineType.isEmpty();
}

QString EdgeType::generateIsNodeOrEdge(QString const &lineTemplate) const
{
	QString line = lineTemplate;
	line.replace(elementNameTag, name()).replace(isNodeOrEdgeTag, "-1");
	return line;
}

QString EdgeType::generateNodeClass(const QString &classTemplate) const
{
	Q_UNUSED(classTemplate)
	return "";
}

QString EdgeType::generateEdgeClass(const QString &classTemplate) const
{
	QString edgeClass = classTemplate;
	MetaCompiler *compiler = diagram()->editor()->metaCompiler();

	QString labelsInitLine;
	QString labelsUpdateLine;
	QString labelsDefinitionLine;

	generateSdf();

	foreach(Label *label, mLabels) {
		labelsInitLine += label->generateInit(compiler, false) + endline;
		labelsUpdateLine += label->generateUpdate(compiler) + endline;
		labelsDefinitionLine += label->generateDefinition(compiler) + endline;
	}
	if (mLabels.isEmpty()) // no labels
		labelsUpdateLine = nodeIndent + "Q_UNUSED(repo)" + endline;

	edgeClass.replace(edgeInitTag, labelsInitLine)
			.replace(updateDataTag, labelsUpdateLine)
			.replace(labelDefinitionTag, labelsDefinitionLine)
			.replace(elementNameTag, name())
			.replace("\\n", "\n");
	return edgeClass + endline;
}

void EdgeType::generateSdf() const
{
	QDir dir;
	if (!dir.exists(generatedDir))
		dir.mkdir(generatedDir);
	dir.cd(generatedDir);
	QString editorName = NameNormalizer::normalize(diagram()->editor()->name());
	if (!dir.exists(editorName))
		dir.mkdir(editorName);
	dir.cd(editorName);
	if (!dir.exists(shapesDir))
		dir.mkdir(shapesDir);
	dir.cd(shapesDir);

	QString const fileName = dir.absoluteFilePath(name() + "Class.sdf");
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "cannot open \"" << fileName << "\"";
		return;
	}
	MetaCompiler *compiler = diagram()->editor()->metaCompiler();

	QString result = compiler->getTemplateUtils(lineSdfTag);
	result.replace(lineTypeTag, mApi->stringProperty(mId, "lineType"))
			.replace("\\n", "\n");

	QTextStream out(&file);
	out << result;
	file.close();
}

// copy-pasted from Shape, quick workaround for #349
void EdgeType::initLabels()
{
	QString xml = mApi->stringProperty(mId, "labels");
	QString error = "";
	int errorLine = 0;
	int errorCol = 0;
	QDomDocument graphics;
	if (!graphics.setContent(xml, false, &error, &errorLine, &errorCol))
		return;

	int count = 1;
	for (QDomElement element = graphics.firstChildElement("labels").firstChildElement("label");
		!element.isNull();
		element = element.nextSiblingElement("label"))
	{
		Label *label = new Label();
		if (!label->init(element, count, true))
			delete label;
		else {
			mLabels.append(label);
			++count;
		}
	}
	qDebug() << mApi->name(mId) << mLabels.size();
	return;

}

QString EdgeType::generateResourceLine(const QString &resourceTemplate) const
{
	QString line = resourceTemplate;
	return line.replace(fileNameTag, name() + "Class.sdf") + endline;
}

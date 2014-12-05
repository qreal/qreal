#include "edgeType.h"
#include "../metaCompiler.h"
#include "../diagram.h"
#include "../editor.h"
#include "../utils/nameNormalizer.h"

#include <QDebug>

using namespace qrmc;
using namespace qReal;

EdgeType::EdgeType(Diagram *diagram, qrRepo::LogicalRepoApi *api, const qReal::Id &id) : GraphicType(diagram, api, id)
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
	result->mFromPorts = mFromPorts;

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

QString EdgeType::generateNodeClass(QString const &classTemplate)
{
	Q_UNUSED(classTemplate)
	return "";
}

QString EdgeType::generateEdgeClass(QString const &classTemplate) const
{
	QString edgeClass = classTemplate;
	MetaCompiler *compiler = diagram()->editor()->metaCompiler();

	QString labelsInitLine;
	QString labelsUpdateLine;
	QString labelsDefinitionLine;

	generateSdf();
	generateArrows(edgeClass);

	foreach(Label *label, mLabels) {
		labelsInitLine += label->generateInit(compiler, false) + endline;
		labelsUpdateLine += label->generateUpdate(compiler) + endline;
		labelsDefinitionLine += label->generateDefinition(compiler) + endline;
	}
	if (mLabels.isEmpty()) { // no labels
		labelsUpdateLine = nodeIndent + "Q_UNUSED(repo)" + endline;
		labelsInitLine = nodeIndent + "Q_UNUSED(factory)" + endline +
						 nodeIndent + "Q_UNUSED(titles)" + endline;
	}

	QString lineType = mApi->stringProperty(mId, "lineType");
	if (lineType.isEmpty())
		lineType = "solidLine";
	lineType = "Qt::" + NameNormalizer::normalize(lineType);

	QString portsForFromPortTypes = generatePorts(mFromPorts);

	edgeClass.replace(edgeInitTag, labelsInitLine)
			.replace(updateDataTag, labelsUpdateLine)
			.replace(labelDefinitionTag, labelsDefinitionLine)
			.replace(lineTypeTag, lineType)

			.replace(portsForFromPortTypesTag, portsForFromPortTypes)

			.replace(elementNameTag, name())
			.replace("\\n", "\n");
	return edgeClass + endline;
}

void EdgeType::generateArrows(QString &edgeClass) const
{
	QString beginType;
	QString endType;
	IdList children = mApi->children(mId);

	foreach (Id child, children){
		if (child.element() == metaEntityAssociation) {
			beginType = mApi->stringProperty(child, "beginType");
			endType = mApi->stringProperty(child, "endType");
		}
	}
	generateArrowEnd(edgeClass, beginType, beginArrowCustomizationTag, beginArrowBrushColorTag);
	generateArrowEnd(edgeClass, endType, endArrowCustomizationTag, endArrowBrushColorTag);

}

void EdgeType::generateArrowEnd(QString &edgeClass, QString const &arrowEnd,
								QString const &customTag, QString const &brushTag) const
{
	MetaCompiler *compiler = diagram()->editor()->metaCompiler();
	if (arrowEnd.isEmpty() || arrowEnd == "no_arrow") {
		edgeClass.replace(customTag, "").replace(brushTag, "");
		return;
	}
	if (arrowEnd == "empty_arrow") {
		edgeClass.replace(customTag, compiler->getTemplateUtils(arrowTemplateTag))
				.replace(brushTag, compiler->getTemplateUtils(emptyArrowColorTag));
	} else if (arrowEnd == "filled_arrow") {
		edgeClass.replace(customTag, compiler->getTemplateUtils(arrowTemplateTag))
				.replace(brushTag, compiler->getTemplateUtils(filledArrowColorTag));
	} else if (arrowEnd == "open_arrow") {
		edgeClass.replace(customTag, compiler->getTemplateUtils(openArrowTemplateTag))
				.replace(brushTag, "");
	} else if (arrowEnd == "complex_arrow") {
		edgeClass.replace(customTag, compiler->getTemplateUtils(complexArrowTemplateTag))
				.replace(brushTag, compiler->getTemplateUtils(emptyArrowColorTag));
	} else if (arrowEnd == "empty_rhomb") {
		edgeClass.replace(customTag, compiler->getTemplateUtils(rhombTemplateTag))
				.replace(brushTag, compiler->getTemplateUtils(emptyArrowColorTag));
	} else if (arrowEnd == "filled_rhomb") {
		edgeClass.replace(customTag, compiler->getTemplateUtils(rhombTemplateTag))
				.replace(brushTag, compiler->getTemplateUtils(filledArrowColorTag));
	} else if (arrowEnd == "crossed_line") {
		edgeClass.replace(customTag, compiler->getTemplateUtils(crossedLineTemplateTag))
				.replace(brushTag, compiler->getTemplateUtils(emptyArrowColorTag));
	} else if (arrowEnd == "empty_circle") {
		edgeClass.replace(customTag, compiler->getTemplateUtils(emptyCircleTemplateTag))
				.replace(brushTag, compiler->getTemplateUtils(emptyArrowColorTag));
	} else if (arrowEnd == "signal") {
		edgeClass.replace(customTag, compiler->getTemplateUtils(signalTemplateTag))
				.replace(brushTag, compiler->getTemplateUtils(emptyArrowColorTag));
	} else if (arrowEnd == "timer") {
		edgeClass.replace(customTag, compiler->getTemplateUtils(timerTemplateTag))
				.replace(brushTag, compiler->getTemplateUtils(emptyArrowColorTag));
	}
}

void EdgeType::generateSdf() const
{
	QDir dir;
	if (!dir.exists(generatedDir)) {
		dir.mkdir(generatedDir);
	}
	dir.cd(generatedDir);
	QString editorName = diagram()->editor()->name();
	if (!dir.exists(editorName)) {
		dir.mkdir(editorName);
	}
	dir.cd(editorName);
	if (!dir.exists(generatedShapesDir)) {
		dir.mkdir(generatedShapesDir);
	}
	dir.cd(generatedShapesDir);
	if (!dir.exists(shapesDir)) {
		dir.mkdir(shapesDir);
	}
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
	QString xml = mApi->hasProperty(mId, "labels") ? mApi->stringProperty(mId, "labels") : "";
	QString error = "";
	int errorLine = 0;
	int errorCol = 0;
	QDomDocument graphics;
	if (!graphics.setContent(xml, false, &error, &errorLine, &errorCol)) {
		return;
	}

	int count = 1;
	for (QDomElement element = graphics.firstChildElement("labels").firstChildElement("label");
		!element.isNull();
		element = element.nextSiblingElement("label"))
	{
		Label *label = new Label();
		if (!label->init(element, count, true, mWidth, mHeight))
			delete label;
		else {
			mLabels.append(label);
			++count;
		}
	}
	return;

}

QString EdgeType::generateResourceLine(QString const &resourceTemplate) const
{
	QString line = resourceTemplate;
	return line.replace(fileNameTag, name() + "Class.sdf") + endline;
}

QString EdgeType::generatePorts(QStringList const &portTypes) const
{
	QString typeForReturning = "";
	foreach (QString const &type, portTypes) {
		//out() << " << \"" << type << "\"";
		typeForReturning += type;
	}
	if (typeForReturning == "")
		typeForReturning = "NonTyped";

	return typeForReturning;
}

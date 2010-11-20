#include "hascolParser.h"

#include "../../editorManager/editorManager.h"

#include <QtCore/QDebug>
#include <QtCore/QUuid>
#include <QtXml/QDomDocument>
#include <QtCore/QProcess>

#include "math.h"

#include "../../../qrrepo/repoApi.h"
#include "../../../utils/xmlUtils.h"

using namespace qReal;
using namespace parsers;
using gui::ErrorReporter;

HascolParser::HascolParser(qrRepo::RepoApi &api, EditorManager const &editorManager)
	: mApi(api), mEditorManager(editorManager), mErrorReporter()
{
}

ErrorReporter &HascolParser::parse(QStringList const &files)
{
	mImportedPortMappingDiagramId = initDiagram("Imported port mapping", "HascolPortMapping_HascolPortMappingDiagram");
	mImportedStructureDiagramId = initDiagram("Imported structure", "HascolStructure_HascolStructureDiagram");
	mApi.setProperty(mImportedStructureDiagramId, "output directory", "");

	foreach (QString file, files) {
		preprocessFile(file);
		parseFile(file + ".xml");
	}

	doPortMappingLayout();
	doStructureLayout();

	return mErrorReporter;
}

void HascolParser::preprocessFile(QString const &fileName)
{
	QProcess preprocessor;
	QStringList args;

	QString const coolRoot = QProcessEnvironment::systemEnvironment().value("COOL_ROOT", ".");
	args << "-I" << coolRoot + "/signature";
	args << fileName;
	args << "-o" << fileName + ".xml";

	preprocessor.start("hascolStructur2xml.byte.exe", args);
	preprocessor.waitForFinished();

	if (preprocessor.exitStatus() == QProcess::CrashExit)
		mErrorReporter.addError("Hascol preprocessor crashed.");
	else if (preprocessor.exitCode() != 0)
		mErrorReporter.addError(QString("Hascol preprocessor finished with error code %1").arg(preprocessor.exitCode()));

	mErrorReporter.addInformation(QString("Preprocessed file %1").arg(fileName));
	QByteArray standardOutput = preprocessor.readAllStandardOutput();
	if (!standardOutput.isEmpty())
		mErrorReporter.addInformation(QString("Output stream: %1").arg(standardOutput.data()));
	QByteArray standardError = preprocessor.readAllStandardError();
	if (!standardError.isEmpty())
		mErrorReporter.addInformation(QString("Error stream: %1").arg(standardError.data()));
}

Id HascolParser::initDiagram(QString const &diagramName, QString const &diagramType)
{
	Id result;
	Id const diagramTypeId = Id("HascolMetamodel", "HascolPortMapping", diagramType);

	foreach(Id element, mApi.children(Id::rootId())) {
		if (element.type() == diagramTypeId && mApi.name(element) == diagramName) {
			result = element;
			// full exterminatus
			// should track changed elements, but it's tricky
			mApi.removeChildren(result);
		}
	}

	if (result == Id())
		result = addElement(Id::rootId(), diagramTypeId, diagramName);
	return result;
}

Id HascolParser::addElement(Id const &parent, Id const &elementType, QString const &name)
{
	Id element(elementType, QUuid::createUuid().toString());

	mApi.addChild(parent, element);
	mApi.setProperty(element, "name", name);
	mApi.setProperty(element, "from", Id::rootId().toVariant());
	mApi.setProperty(element, "to", Id::rootId().toVariant());
	mApi.setProperty(element, "fromPort", 0.0);
	mApi.setProperty(element, "toPort", 0.0);
	mApi.setProperty(element, "links", IdListHelper::toVariant(IdList()));

	mApi.setProperty(element, "position", QPointF(0,0));
	mApi.setProperty(element, "configuration", QVariant(QPolygon()));

	return element;
}

void HascolParser::parseFile(QString const& fileName)
{
	QDomDocument doc = utils::xmlUtils::loadDocument(fileName);

	QDomNodeList list = doc.elementsByTagName("md");
	for (unsigned i = 0; i < list.length(); ++i) {
		QDomElement md = list.at(i).toElement();
		QDomNodeList children = md.childNodes();
		for(unsigned j = 0; j < children.length(); ++j) {
			QDomElement child = children.at(j).toElement();
			if (child.isElement()) {
				parseProcess(child.toElement());
			}
		}
	}
}

void HascolParser::initClassifierFields(Id const &classifier)
{
	// TODO: remove from here. make something like auto-completion of fields
	mApi.setProperty(classifier, "clientDependency", "");
	mApi.setProperty(classifier, "elementImport", "");
	mApi.setProperty(classifier, "generalization", "");
	mApi.setProperty(classifier, "implementation", "");
	mApi.setProperty(classifier, "isAbstract", "");
	mApi.setProperty(classifier, "redefinedClassifier", "");
	mApi.setProperty(classifier, "substitution", "");
	mApi.setProperty(classifier, "powertypeExtent", "");
	mApi.setProperty(classifier, "isLeaf", "");
	mApi.setProperty(classifier, "ownedRule", "");
	mApi.setProperty(classifier, "packageImport", "");
	mApi.setProperty(classifier, "visibility", "");
	mApi.setProperty(classifier, "ownedComment", "");
}

void HascolParser::parseProcess(QDomElement const &element)
{
	QString const name = element.nodeName();
	bool const functor = element.attribute("isFunctor", "0") == "1";

	Id const portMappingBaseId = Id("HascolMetamodel", "HascolPortMapping");
	Id const structureBaseId = Id("HascolMetamodel", "HascolPortMapping");

	Id portMappingElementType = functor ? Id(portMappingBaseId, "HascolPortMapping_FunctorInstance")
		: Id(portMappingBaseId, "HascolPortMapping_ProcessInstance");

	Id structureElementType = functor ? Id(structureBaseId, "HascolStructure_Functor")
		: Id(structureBaseId, "HascolStructure_Process");

	Id processOnAPortMap = addElement(mImportedPortMappingDiagramId, portMappingElementType, "a" + name + " : " + name);
	Id processOnAStructure = addElement(mImportedStructureDiagramId, structureElementType, name);
	initClassifierFields(processOnAStructure);

	QDomNodeList insList = element.elementsByTagName("ins");
	for (unsigned i = 0; i < insList.length(); ++i) {
		QDomElement ins = insList.at(i).toElement();
		parsePorts(ins.childNodes(), "in", processOnAPortMap, processOnAStructure);
	}

	QDomNodeList outsList = element.elementsByTagName("outs");
	for (unsigned i = 0; i < outsList.length(); ++i) {
		QDomElement outs = outsList.at(i).toElement();
		parsePorts(outs.childNodes(), "out", processOnAPortMap, processOnAStructure);
	}
}

void HascolParser::parsePorts(QDomNodeList const &ports, QString const &direction
	, Id const &parentOnAPortMap, Id const &parentOnAStructure)
{
	for (unsigned i = 0; i < ports.length(); ++i) {
		Id const portMappingBaseId = Id("HascolMetamodel", "HascolPortMapping");
		Id const structureBaseId = Id("HascolMetamodel", "HascolPortMapping");

		QDomElement port =  ports.at(i).toElement();
		QString portName = port.nodeName();
		QString parameters;
		QDomNamedNodeMap attrs = port.attributes();
		for (int i = 0; i < attrs.count(); ++i) {
			QDomAttr param = attrs.item(i).toAttr();
			QString paramValue = param.value();
			// removing explicit qualification for bincompl since it's included automatically
			paramValue = paramValue.remove("bincompl::");
			parameters += paramValue + ", ";
		}
		parameters.chop(2);

		// ports should be without arguments here
		Id attrType = Id(portMappingBaseId, "HascolPortMapping_Port");
		Id portId = addElement(parentOnAPortMap, attrType, portName);
		mApi.setProperty(portId, "direction", direction);

		portName += "(" + parameters + ")";

		Id structureAttrType = Id(structureBaseId, "HascolStructure_ProcessOperation");
		Id plugId = addElement(parentOnAStructure, structureAttrType, portName);
		mApi.setProperty(plugId, "direction", direction);
		initClassifierFields(plugId);
	}
}

void HascolParser::doLayout(Id const &diagram, unsigned cellWidth, unsigned cellHeight)
{
	unsigned rowWidth = ceil(sqrt(static_cast<qreal>(mApi.children(diagram).count())));
	unsigned currentRow = 0;
	unsigned currentColumn = 0;
	foreach(Id element, mApi.children(diagram)) {
		mApi.setProperty(element, "position", QPointF(currentColumn * cellWidth, currentRow * cellHeight));
		++currentColumn;
		if (currentColumn >= rowWidth) {
			currentColumn = 0;
			++currentRow;
		}
	}
}

void HascolParser::doPortMappingLayout()
{
	doLayout(mImportedPortMappingDiagramId, 300, 150);
	foreach(Id element, mApi.children(mImportedPortMappingDiagramId))
		doPortsLayout(element);
}

void HascolParser::doStructureLayout()
{
	doLayout(mImportedStructureDiagramId, 300, 250);
	foreach(Id element, mApi.children(mImportedStructureDiagramId))
		doPlugsLayout(element);
}

void HascolParser::doPlugsLayout(Id const &parent)
{
	unsigned const startY = 50;

	double step = 25;
	unsigned current = 1;

	foreach(Id element, mApi.children(parent)) {
		mApi.setProperty(element, "position", QPointF(10, startY + step * current));
		++current;
	}
}

void HascolParser::doPortsLayout(Id const &parent)
{
	unsigned inputPorts = 0;
	unsigned outputPorts = 0;
	foreach(Id element, mApi.children(parent)) {
		if (mApi.hasProperty(element, "direction")) {
			if (mApi.stringProperty(element, "direction") == "in")
				++inputPorts;
			else if (mApi.stringProperty(element, "direction") == "out")
				++outputPorts;
		}
	}

	doLayoutForPortsType(parent, 15, "in", inputPorts);
	doLayoutForPortsType(parent, 175, "out", outputPorts);
}

void HascolParser::doLayoutForPortsType(Id const &parent, unsigned margin, QString const &direction, unsigned count)
{
	unsigned const startY = 0;
	unsigned const endY = 100;

	double step = (endY - startY) / (count + 1);
	unsigned current = 1;

	foreach(Id element, mApi.children(parent)) {
		if (mApi.hasProperty(element, "direction")) {
			if (mApi.stringProperty(element, "direction") == direction) {
				mApi.setProperty(element, "position", QPointF(margin, startY + step * current));
				++current;
			}
		}
	}
}

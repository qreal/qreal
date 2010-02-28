#include "hascolParser.h"

#include "../../mainwindow/editormanager.h"

#include <QtCore/QDebug>
#include <QtCore/QUuid>
#include <QtXml/QDomDocument>

#include "math.h"

#include "../../../qrrepo/repoApi.h"
#include "../../../utils/xmlUtils.h"

using namespace qReal;
using namespace parsers;

HascolParser::HascolParser(qrRepo::RepoApi &api, EditorManager const &editorManager)
	: mApi(api), mEditorManager(editorManager)
{
}

void HascolParser::parse(QStringList const &files)
{
	Id const importedDiagramType = Id("HascolMetamodel", "HascolPortMapping",
		"HascolPortMapping_HascolPortMappingDiagram");

	foreach(Id element, mApi.children(ROOT_ID)) {
		if (element.type() == importedDiagramType && mApi.name(element) == "Imported diagram") {
			mImportedDiagramId = element;
			mApi.removeChildren(mImportedDiagramId);  // Пока что устраиваем локальный экстерминатус.
			// По идее, надо бы следить за изменёнными элементами, но это хитро.
		}
	}

	if (mImportedDiagramId == Id())
		mImportedDiagramId = addElement(ROOT_ID, importedDiagramType, "Imported diagram");

	foreach (QString file, files)
		parseFile(file);

	doLayout();
}

Id HascolParser::addElement(Id const &parent, Id const &elementType, QString const &name)
{
	Id element(elementType, QUuid::createUuid().toString());

	mApi.addChild(parent, element);
	mApi.setProperty(element, "name", name);
	mApi.setProperty(element, "from", ROOT_ID.toVariant());
	mApi.setProperty(element, "to", ROOT_ID.toVariant());
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

void HascolParser::parseProcess(QDomElement const &element)
{
	QString const name = element.nodeName();
	bool const functor = element.attribute("isFunctor", "0") == "1";

	Id const baseId = Id("HascolMetamodel", "HascolPortMapping");

	Id elementType = functor ? Id(baseId, "HascolPortMapping_FunctorInstance")
		: Id(baseId, "HascolPortMapping_ProcessInstance");

	Id process = addElement(mImportedDiagramId, elementType, name);

	QDomNodeList insList = element.elementsByTagName("ins");
	for (unsigned i = 0; i < insList.length(); ++i) {
		QDomElement ins = insList.at(i).toElement();
		parsePorts(ins.childNodes(), "in", process);
	}

	QDomNodeList outsList = element.elementsByTagName("outs");
	for (unsigned i = 0; i < outsList.length(); ++i) {
		QDomElement outs = outsList.at(i).toElement();
		parsePorts(outs.childNodes(), "out", process);
	}
}

void HascolParser::parsePorts(QDomNodeList const &ports, QString const &direction, Id const &parent)
{
	for (unsigned i = 0; i < ports.length(); ++i) {
		Id const baseId = Id("HascolMetamodel", "HascolPortMapping");

		QDomElement port =  ports.at(i).toElement();
		QString portName = port.nodeName();
		QString parameters;
		QDomNamedNodeMap attrs = port.attributes();
		for (int i = 0; i < attrs.count(); ++i) {
			QDomAttr param = attrs.item(i).toAttr();
			QString paramValue = param.value();
			paramValue = paramValue.remove("bincompl::");  // Уберём явную квалификацию для сигнатуры bincompl, она всё равно подключается по умолчанию.
			parameters += paramValue + ", ";
		}
		parameters.chop(2);
		portName += "(" + parameters + ")";

		Id attrType = Id(baseId, "HascolPortMapping_Port");
		Id portId = addElement(parent, attrType, portName);
		mApi.setProperty(portId, "direction", direction);
	}
}

void HascolParser::doLayout() {
	unsigned count = mApi.children(mImportedDiagramId).count();
	unsigned rowWidth = ceil(sqrt(count));
	unsigned currentRow = 0;
	unsigned currentColumn = 0;
	foreach(Id element, mApi.children(mImportedDiagramId)) {
		mApi.setProperty(element, "position", QPointF(currentColumn * 300, currentRow * 150));
		doPortsLayout(element);
		++currentColumn;
		if (currentColumn >= rowWidth) {
			currentColumn = 0;
			++currentRow;
		}
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

#include "pluginInterface.h"

#include "elements.h"


Q_EXPORT_PLUGIN2(qreal_editors, TestPlugin)

TestPlugin::TestPlugin()
{
	initPlugin();
}

void TestPlugin::initPlugin()
{
	initNameMap();
	initMouseGestureMap();
	initPropertyMap();
	initPropertyDefaultsMap();
	initDescriptionMap();
	initParentsMap();
}

void TestPlugin::initNameMap()
{
	diagramNameMap["TestEditor"] = QString::fromUtf8("testEditor");
	diagramNodeNameMap["TestEditor"] = "Diagram";

	elementsNameMap["TestEditor"]["Diagram"] = QString::fromUtf8("Diagram");
	elementsNameMap["TestEditor"]["Link"] = QString::fromUtf8("Link");
	elementsNameMap["TestEditor"]["Node"] = QString::fromUtf8("Node");
}

void TestPlugin::initMouseGestureMap()
{
}

void TestPlugin::initPropertyMap()
{
}

void TestPlugin::initPropertyDefaultsMap()
{
}

void TestPlugin::initDescriptionMap()
{
}

void TestPlugin::initParentsMap()
{
}

QStringList TestPlugin::diagrams() const
{
	return diagramNameMap.keys();
}

QStringList TestPlugin::elements(QString const &diagram) const
{
	return elementsNameMap[diagram].keys();
}

QStringList TestPlugin::getPropertiesWithDefaultValues(QString const &element) const
{
	return propertyDefault[element].keys();
}

QIcon TestPlugin::getIcon(SdfIconEngineV2Interface *engine) const
{
	return QIcon(engine);
}

QString TestPlugin::editorName() const
{
	 return "Test Editor";
}

QString TestPlugin::diagramName(QString const &diagram) const
{
	return diagramNameMap[diagram];
}

QString TestPlugin::diagramNodeName(QString const &diagram) const
{
	return diagramNodeNameMap[diagram];
}

QString TestPlugin::elementName(QString const &diagram, QString const &element) const
{
	return elementsNameMap[diagram][element];
}

QString TestPlugin::elementDescription(QString const &diagram, QString const &element) const
{
	return elementsDescriptionMap[diagram][element];
}

QString TestPlugin::propertyDescription(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDescriptionMap[diagram][element][property];
}

QString TestPlugin::propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDisplayedNamesMap[diagram][element][property];
}

QString TestPlugin::elementMouseGesture(QString const &diagram, QString const &element) const
{
	return elementMouseGesturesMap[diagram][element];
}

ElementImpl* TestPlugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const
{
	if (element == "Diagram")
		return new Diagram();
	else if (element == "Link")
		return new Link();
	else if (element == "Node")
		return new Node();
	else {
		Q_ASSERT(!"Request for creation of an element with unknown name");
		return NULL;
	}
}

bool TestPlugin::isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	if (parentsMap[childDiagram][childElement].contains(qMakePair(parentDiagram, parentElement)))
		return true;
	typedef QPair<QString, QString> StringPair;
	foreach (StringPair const &pair, parentsMap[childDiagram][childElement])
		if (isParentOf(parentDiagram, parentElement, pair.first, pair.second))
			return true;
	return false;
}
QList<QPair<QString, QString> > TestPlugin::getParentsOf(QString const &diagram, QString const &element) const
{
	return parentsMap[diagram][element];
}
QStringList TestPlugin::getPropertyNames(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "Diagram")
;
	else if (element == "Link")
;
	else if (element == "Node")
;
	return result;
}

QStringList TestPlugin::getTypesContainedBy(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QStringList TestPlugin::getConnectedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QStringList TestPlugin::getUsedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > TestPlugin::getPossibleEdges(QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	Q_UNUSED(element);
	return result;
}

//(-1) means "edge", (+1) means "node"
int TestPlugin::isNodeOrEdge(QString const &element) const
{
	if (element == "Diagram")
		return 1;
	else if (element == "Link")
		return -1;
	else if (element == "Node")
		return 1;
	return 0;
}
QStringList TestPlugin::getEnumValues(QString name) const 
{
	QStringList result;
	Q_UNUSED(name);
	return result;
}

QString TestPlugin::getPropertyType(QString const &element, QString const &property) const
{
	return propertyTypes[element][property];
}

QString TestPlugin::getPropertyDefaultValue(QString const &element, QString const &property) const
{
	return propertyDefault[element][property];
}

QList<qReal::ListenerInterface*> TestPlugin::listeners() const
{
	QList<qReal::ListenerInterface*> result;
	return result;
}

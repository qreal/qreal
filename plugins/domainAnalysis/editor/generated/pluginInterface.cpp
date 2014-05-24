#include "pluginInterface.h"

#include "elements.h"


EditorPlugin::EditorPlugin()
{
	initPlugin();
}

void EditorPlugin::initPlugin()
{
	initNameMap();
	initMouseGestureMap();
	initPropertyMap();
	initPropertyDefaultsMap();
	initDescriptionMap();
	initParentsMap();
	initPaletteGroupsMap();
	initPaletteGroupsDescriptionMap();
	initShallPaletteBeSortedMap();
	initExplosionsMap();
}

void EditorPlugin::initNameMap()
{
	mDiagramNameMap["Diagram"] = QString::fromUtf8("Diagram");
	mDiagramNodeNameMap["Diagram"] = "DiagramNode";

	mElementsNameMap["Diagram"]["Connection"] = QString::fromUtf8("Стрелка следования");
	mElementsNameMap["Diagram"]["DiagramNode"] = QString::fromUtf8("Diagram");
	mElementsNameMap["Diagram"]["FinalNode"] = QString::fromUtf8("Final Node");
	mElementsNameMap["Diagram"]["GameChoice"] = QString::fromUtf8("Выбор игры");
	mElementsNameMap["Diagram"]["GoalKeeperOrPlayer"] = QString::fromUtf8("Выбор позиции вратаря или полевого игрока");
	mElementsNameMap["Diagram"]["InitialNode"] = QString::fromUtf8("Initial Node");
	mElementsNameMap["Diagram"]["Place"] = QString::fromUtf8("Задать место");
	mElementsNameMap["Diagram"]["TeamChoice"] = QString::fromUtf8("Выбор команды");
	mElementsNameMap["Diagram"]["TeamName"] = QString::fromUtf8("Ввести имя команды");
	mElementsNameMap["Diagram"]["Time"] = QString::fromUtf8("Задать время");
}

void EditorPlugin::initPaletteGroupsMap()
{
	{
		QStringList groupElements;
		groupElements << QString::fromUtf8("Time");
		groupElements << QString::fromUtf8("Place");
		groupElements << QString::fromUtf8("TeamName");
		groupElements << QString::fromUtf8("TeamChoice");
		groupElements << QString::fromUtf8("GameChoice");
		groupElements << QString::fromUtf8("GoalKeeperOrPlayer");
		groupElements << QString::fromUtf8("Connection");
		groupElements << QString::fromUtf8("InitialNode");
		groupElements << QString::fromUtf8("FinalNode");
		mPaletteGroupsMap[QString::fromUtf8("Diagram")].append(qMakePair(QString::fromUtf8("Элементы"), groupElements));
	}
}

void EditorPlugin::initPaletteGroupsDescriptionMap()
{
}

void EditorPlugin::initMouseGestureMap()
{
}

void EditorPlugin::initPropertyMap()
{
	mPropertyTypes["DiagramNode"]["isView"] = "Bool";
}

void EditorPlugin::initPropertyDefaultsMap()
{
}

void EditorPlugin::initDescriptionMap()
{
}

void EditorPlugin::initParentsMap()
{
	mParentsMap["Diagram"]["DiagramNode"]
		<< qMakePair(QString("Diagram"), QString("Diagram"))
	;
}

void EditorPlugin::initShallPaletteBeSortedMap()
{
	mShallPaletteBeSortedMap[QString::fromUtf8("Diagram")] = true;
}

void EditorPlugin::initExplosionsMap()
{
}

QStringList EditorPlugin::diagrams() const
{
	return mDiagramNameMap.keys();
}

QStringList EditorPlugin::diagramPaletteGroups(QString const &diagram) const
{
	QStringList result;
	for (QPair<QString, QStringList> const &group : mPaletteGroupsMap[diagram]) {
		result << group.first;
	}

	return result;
}

QStringList EditorPlugin::diagramPaletteGroupList(QString const &diagram, QString const &group) const
{
	for (QPair<QString, QStringList> const &ourGroup : mPaletteGroupsMap[diagram]) {
		if (ourGroup.first == group) {
			return ourGroup.second;
		}
	}

	return QStringList();
}

QString EditorPlugin::diagramPaletteGroupDescription(QString const &diagram, QString const &group) const
{
	return mPaletteGroupsDescriptionMap[diagram][group];
}

bool EditorPlugin::shallPaletteBeSorted(QString const &diagram) const
{
	return mShallPaletteBeSortedMap[diagram];
}

QStringList EditorPlugin::elements(QString const &diagram) const
{
	return mElementsNameMap[diagram].keys();
}

QStringList EditorPlugin::getPropertiesWithDefaultValues(QString const &element) const
{
	return mPropertyDefault[element].keys();
}

QString EditorPlugin::editorName() const
{
	 return "Editor Editor";
}

QString EditorPlugin::diagramName(QString const &diagram) const
{
	return mDiagramNameMap[diagram];
}

QString EditorPlugin::diagramNodeName(QString const &diagram) const
{
	return mDiagramNodeNameMap[diagram];
}

QString EditorPlugin::elementName(QString const &diagram, QString const &element) const
{
	return mElementsNameMap[diagram][element];
}

QString EditorPlugin::elementDescription(QString const &diagram, QString const &element)const
{
	return mElementsDescriptionMap[diagram][element];
}

QString EditorPlugin::propertyDescription(QString const &diagram, QString const &element, QString const &property) const
{
	return mPropertiesDescriptionMap[diagram][element][property];
}

QString EditorPlugin::propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const
{
	return mPropertiesDisplayedNamesMap[diagram][element][property];
}

QString EditorPlugin::elementMouseGesture(QString const &diagram, QString const &element) const
{
	return mElementMouseGesturesMap[diagram][element];
}

QList<qReal::EditorInterface::ExplosionData>EditorPlugin::explosions(QString const &diagram, QString const &element) const 
{
	return mExplosionsMap[diagram][element];
}

qReal::ElementImpl* EditorPlugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const
{
	if (element == "Connection") {
		return new Connection();
	}
	else if (element == "DiagramNode") {
		return new DiagramNode();
	}
	else if (element == "FinalNode") {
		return new FinalNode();
	}
	else if (element == "GameChoice") {
		return new GameChoice();
	}
	else if (element == "GoalKeeperOrPlayer") {
		return new GoalKeeperOrPlayer();
	}
	else if (element == "InitialNode") {
		return new InitialNode();
	}
	else if (element == "Place") {
		return new Place();
	}
	else if (element == "TeamChoice") {
		return new TeamChoice();
	}
	else if (element == "TeamName") {
		return new TeamName();
	}
	else if (element == "Time") {
		return new Time();
	}
	else {
		Q_ASSERT(!"Request for creation of an element with unknown name");
		return NULL;
	}
}

bool EditorPlugin::isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	if (childDiagram == parentDiagram && childElement == parentElement)
		return true;
	if (mParentsMap[childDiagram][childElement].contains(qMakePair(parentDiagram, parentElement)))
		return true;
	typedef QPair<QString, QString> StringPair;
	foreach (StringPair const &pair, mParentsMap[childDiagram][childElement])
		if (isParentOf(parentDiagram, parentElement, pair.first, pair.second))
			return true;
	return false;
}
QList<QPair<QString, QString> > EditorPlugin::getParentsOf(QString const &diagram, QString const &element) const
{
	return mParentsMap[diagram][element];
}
QStringList EditorPlugin::getPropertyNames(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "Connection") {
	}
	else if (element == "DiagramNode") {
		result  << "isView" << "language";
	}
	else if (element == "FinalNode") {
	}
	else if (element == "GameChoice") {
	}
	else if (element == "GoalKeeperOrPlayer") {
	}
	else if (element == "InitialNode") {
	}
	else if (element == "Place") {
	}
	else if (element == "TeamChoice") {
	}
	else if (element == "TeamName") {
	}
	else if (element == "Time") {
	}
	return result;
}

QStringList EditorPlugin::getPortTypes(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "DiagramNode") {
		result << "NonTyped";
	}
	else if (element == "FinalNode") {
		result << "NonTyped";
	}
	else if (element == "GameChoice") {
		result << "NonTyped";
	}
	else if (element == "GoalKeeperOrPlayer") {
		result << "NonTyped";
	}
	else if (element == "InitialNode") {
		result << "NonTyped";
	}
	else if (element == "Place") {
		result << "NonTyped";
	}
	else if (element == "TeamChoice") {
		result << "NonTyped";
	}
	else if (element == "TeamName") {
		result << "NonTyped";
	}
	else if (element == "Time") {
		result << "NonTyped";
	}
	return result;
}

QStringList EditorPlugin::getReferenceProperties(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "Connection") {
	}
	else if (element == "DiagramNode") {
	}
	else if (element == "FinalNode") {
	}
	else if (element == "GameChoice") {
	}
	else if (element == "GoalKeeperOrPlayer") {
	}
	else if (element == "InitialNode") {
	}
	else if (element == "Place") {
	}
	else if (element == "TeamChoice") {
	}
	else if (element == "TeamName") {
	}
	else if (element == "Time") {
	}
	return result;
}

QStringList EditorPlugin::getTypesContainedBy(QString const &element) const
{
	QStringList result;
	if (element == "DiagramNode") {
		result << "Time" << "Place" << "TeamName" << "TeamChoice" << "GameChoice" << "GoalKeeperOrPlayer" << "Connection" ;
	}
	return result;
}

QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > EditorPlugin::getPossibleEdges(QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	Q_UNUSED(element);
	return result;
}

//(-1) means "edge", (+1) means "node"
int EditorPlugin::isNodeOrEdge(QString const &element) const
{
	if (element == "Connection")
		return -1;
	else if (element == "DiagramNode")
		return 1;
	else if (element == "FinalNode")
		return 1;
	else if (element == "GameChoice")
		return 1;
	else if (element == "GoalKeeperOrPlayer")
		return 1;
	else if (element == "InitialNode")
		return 1;
	else if (element == "Place")
		return 1;
	else if (element == "TeamChoice")
		return 1;
	else if (element == "TeamName")
		return 1;
	else if (element == "Time")
		return 1;
	return 0;
}
QString EditorPlugin::getGroupsXML() const 
{
	return QString::fromUtf8("");
}

QStringList EditorPlugin::getEnumValues(QString name) const 
{
	QStringList result;
	if (name == "AggregationKind")
		result << QString::fromUtf8("none") << QString::fromUtf8("shared") << QString::fromUtf8("composite");
	else if (name == "Bool")
		result << QString::fromUtf8("false") << QString::fromUtf8("true");
	else if (name == "CallConcurrencyKind")
		result << QString::fromUtf8("sequential") << QString::fromUtf8("guarded") << QString::fromUtf8("concurrent");
	else if (name == "VisibilityKind")
		result << QString::fromUtf8("private") << QString::fromUtf8("protected") << QString::fromUtf8("public") << QString::fromUtf8("package");
	else if (name == "ParameterDirectionKind")
		result << QString::fromUtf8("in") << QString::fromUtf8("inout") << QString::fromUtf8("out") << QString::fromUtf8("return");
	return result;
}

QString EditorPlugin::getPropertyType(QString const &element, QString const &property) const
{
	return mPropertyTypes[element][property];
}

QString EditorPlugin::getPropertyDefaultValue(QString const &element, QString const &property) const
{
	return mPropertyDefault[element][property];
}

QList<qReal::ListenerInterface*> EditorPlugin::listeners() const
{
	QList<qReal::ListenerInterface*> result;
	return result;
}

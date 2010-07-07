#include <QtCore/QCoreApplication>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QMap>
#include <QFileInfo>

void createNewChildForRepo(QDomNode node, QDomDocument &doc, QList<QString> &list, QString const &str)
{
	QDomElement eNewChildRepo = doc.createElement("label");
	eNewChildRepo.setAttribute("x", 0);
	eNewChildRepo.setAttribute("y", 0);
	eNewChildRepo.setAttribute(str, list.takeFirst());
	node.appendChild(eNewChildRepo);
}

void changeRepo(QDomNode &node, QDomElement &enode, QDomDocument &doc)
{
	enode.setTagName("labels");
	QDomNode childRepo = node.firstChild();
	QList<QString> listAttrRepo;
	QList<QString> listAttrText;
	while (!childRepo.isNull()) {
		QDomNode childChildRepo = childRepo.firstChild();
		while (!childChildRepo.isNull()) {
			QDomElement echildChildRepo = childChildRepo.toElement();
			if (echildChildRepo.tagName() == "html:text_from_repo") {
				listAttrRepo.append(echildChildRepo.attribute("name"));
			}
			if (echildChildRepo.tagName() == "html:text") {
				QDomNode childChildChildRepo = childChildRepo.firstChild();
				listAttrText.append(childChildChildRepo.nodeValue());
			}
			childChildRepo = childChildRepo.firstChild();
		}
		childRepo = childRepo.nextSibling();
	}
	childRepo = node.firstChild();
	while (!childRepo.isNull()) {
		QDomNode childRepoSibling = childRepo.nextSibling();
		node.removeChild(childRepo);
		childRepo = childRepoSibling;
	}
	while (!listAttrRepo.isEmpty()) {
		createNewChildForRepo(node, doc, listAttrRepo, "textBinded");
	}
	while (!listAttrText.isEmpty()) {
		createNewChildForRepo(node, doc, listAttrText, "text");
	}
}

void changeView(QDomNode &node)
{
	QDomNode nodeParent = node.parentNode();
	QDomNode nodeClone = node.cloneNode(true);
	nodeParent.removeChild(node);
	QDomNode i = nodeClone.firstChild();
	while (!i.isNull()) {
		QDomNode iSibling = i.nextSibling();
		nodeParent.appendChild(i);
		i = iSibling;
	}
	node = nodeParent;
}

void changeContainers(QDomNode &node, QDomElement &enode, QDomDocument &doc)
{
	enode.setTagName("container");
	QDomNode childCont = node.firstChild();
	QList<QDomElement> list;
	bool signBy = false;
	while (!childCont.isNull()) {
		QDomNode childChildCont = childCont.firstChild();
		QDomElement eChildChildCont = childChildCont.toElement();
		if (eChildChildCont.tagName() == "contained_by") {
			childCont.removeChild(childChildCont);
			signBy = true;
		}else{
			QString attr = eChildChildCont.attribute("idref");
			childCont.removeChild(childChildCont);
			QDomElement enewChildChildCont = doc.createElement("contains");
			enewChildChildCont.setAttribute("type", attr);
			list.append(enewChildChildCont);
			childCont = childCont.nextSibling();}
	}
	childCont = node.firstChild();
	while (!childCont.isNull()) {
		QDomNode childContSibling = childCont.nextSibling();
		node.removeChild(childCont);
		childCont = childContSibling;
	}
	while (!list.isEmpty() && !signBy) {
		node.appendChild(list.takeFirst());
	}
	if (!enode.hasChildNodes()) {
		QDomNode nodeSibling = node.nextSibling();
		QDomNode nodeParent = node.parentNode();
		nodeParent.removeChild(node);
		node = nodeSibling;
	}
}

void changeGeneralization(QDomNode &node)
{
	QDomNode childGen = node.firstChild();
	while (!childGen.isNull()) {
		QDomElement echildGen = childGen.toElement();
		QString attr = echildGen.attribute("parent_id");
		echildGen.removeAttribute("parent_id");
		echildGen.setAttribute("parentName", attr);
		childGen = childGen.nextSibling();
	}
	QDomNode nParent = node.parentNode();
	QDomNode nodeClone = node.cloneNode(true);
	nParent.removeChild(node);
	QDomNode i = nodeClone.firstChild();
	while (!i.isNull()) {
		QDomNode iSibling = i.nextSibling();
		nParent.appendChild(i);
		i = iSibling;
	}
	node = nParent;
}

void changeEdgeNodeAndEnum(QDomNode &node)
{
	QDomElement enode = node.toElement();
	QString attrId = enode.attribute("id");
	QString attrName = enode.attribute("name");
	enode.removeAttribute("id");
	enode.removeAttribute("name");
	enode.setAttribute("name", attrId);
	enode.setAttribute("displayedName", attrName);
}

void changeInclude(QDomNode &node)
{
	QDomNode childInclude = node.firstChild();
	if (childInclude.isText()) {
		QString existText = childInclude.nodeValue();
		QString newText = "../commonXml/" + existText + ".xml";
		childInclude.setNodeValue(newText);
	}
}

void changeAssociations(QDomNode &node)
{
	QDomNode childAsso = node.firstChild();
	while (!childAsso.isNull()) {
		QDomNode childAssoSibling = childAsso.nextSibling();
		node.removeChild(childAsso);
		childAsso = childAssoSibling;
	}
}

void changeEndBeginType(QDomNode &node, QDomElement &enode, QString const &str)
{
	QString attrType = enode.attribute("type");
	QDomNode nodeParent = node.parentNode();
	QDomNode nextSibling = node.nextSibling();
	nodeParent.removeChild(node);
	node = nextSibling;
	QDomNode nParentParent = nodeParent.parentNode();
	QDomElement enParentParent = nParentParent.toElement();
	QDomNodeList listLog = enParentParent.elementsByTagName("logic");
	QDomElement eParentLog = listLog.item(0).toElement();
	QDomNodeList listAsso = eParentLog.elementsByTagName("associations");
	listAsso.item(0).toElement().setAttribute(str, attrType);
}

void bypass(QDomNode node, QDomDocument &doc)
{
	QDomElement enode = node.toElement();
	if (enode.isNull())
		return;
	else if (enode.tagName() == "graph_types")
		enode.setTagName("graphicTypes");
	else if (enode.tagName() == "editor")
		enode.setTagName("diagram");
	else if (enode.tagName() == "line_type")
		enode.setTagName("lineType");
	else if (enode.tagName() == "repo_info")
		changeRepo(node, enode, doc);
	else if (enode.tagName() == "line_port")
		enode.setTagName("linePort");
	else if (enode.tagName() == "view" )
		changeView(node);
	else if (enode.tagName() == "generalization")
		changeGeneralization(node);
	else if (enode.tagName() == "edge" || enode.tagName() == "node" || enode.tagName() == "enum")
		changeEdgeNodeAndEnum(node);
	else if (enode.tagName() == "include")
		changeInclude(node);
	else if (enode.tagName() == "containers")
		changeContainers(node, enode, doc);
	else if (enode.tagName() == "associations")
		changeAssociations(node);
	else if (enode.tagName() == "end_type")
		changeEndBeginType(node, enode, "endType");
	else if (enode.tagName() == "begin_type")
		changeEndBeginType(node, enode, "beginType");

	QDomNode child = node.firstChild();
	while(!child.isNull()) {
		QDomNode childSibling = child.nextSibling();
		bypass(child, doc);
		child = childSibling;
	}
}

void getId(QDomNode node, QDomDocument &doc, QDomDocument &docId, QList<QString> &listId, QDomNode &rootId)
{
	QDomElement enode = node.toElement();
	if (enode.isNull())
		return;
	else if (enode.tagName() == "parent") {
		QString attrForId = enode.attribute("parent_id");
		if (!listId.contains(attrForId)) {
			listId.append(attrForId);
			QDomElement newId = docId.createElement("id");
			rootId.appendChild(newId);
			newId.setAttribute("was", attrForId);
			newId.setAttribute("now", "");
		}
	}

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		QDomNode childSibling = child.nextSibling();
		getId(child, doc, docId, listId, rootId);
		child = childSibling;
	}
}

void map(QDomNode node, QMap<QString, QString> &mapId)
{
	QDomElement e = node.toElement();
	mapId.insert(e.attribute("was"), e.attribute("now"));
	QDomNode neighbor = node.nextSibling();
	while (!neighbor.isNull()) {
		QDomNode neighborSibling = neighbor.nextSibling();
		map(neighbor, mapId);
		neighbor = neighborSibling;
	}
}

void changeId(QDomNode node, QMap<QString, QString> &mapId)
{
	QDomElement e = node.toElement();
	if (e.isNull())
		return;
	else if (e.tagName() == "parent") {
		e.setAttribute("parentName", mapId[e.attribute("parentName")]);
	}
	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		QDomNode childSibling = child.nextSibling();
		changeId(child, mapId);
		child = childSibling;
	}
}
QDomDocument fileOpenForRead(QFile &file){
	QDomDocument doc;
	if (!file.open(QIODevice::ReadOnly)) {
		throw "openFileException";
	}
	if (!doc.setContent(&file)) {
		file.close();
		throw "xmlFormatException";
	}
	file.close();
	return doc;
}
void fileOpenForWrite(QFile &file, QDomDocument &doc)
{
	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream stream( &file );
		stream << doc.toString(4) << endl;
	}
	file.close();
}
void fileOpenForReadWrite(QFile &file)
{
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
		throw "openFileException";

	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		if (line == "\n") {
			QTextStream stream( &file );
			QFileInfo fileIn(file);
			QString str = "QREAL_EDITOR_NAME = " + fileIn.baseName();
			stream << str << endl;
			stream << "include (../editorsCommon.pri)" << endl;
			break;
		}
	}
	file.close();
}
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QDomDocument docId;
	if (a.arguments().length() == 3) {
		QFile file(a.arguments().last());
		QDomDocument doc = fileOpenForRead(file);
		QFile filePro(a.arguments().at(1));
		fileOpenForReadWrite(filePro);
		QDomElement docRoot = doc.documentElement();
		QList<QString> listId;
		QDomElement rootId = docId.createElement("metamodel");
		docId.appendChild(rootId);
		getId(docRoot, doc, docId, listId, rootId);
		QFile fileId( "idCollector" );
		fileOpenForWrite(fileId, docId);
	}
	else {
		QFile file(a.arguments().at(1));
		QDomDocument doc = fileOpenForRead(file);
		QFileInfo fileIn(file);
		file.rename(fileIn.baseName() + "_old.xml");
		QDomElement docRoot = doc.documentElement();
		QMap<QString, QString> mapId;
		bypass(docRoot, doc);
		QFile fileId("idCollector");
		QDomDocument docId = fileOpenForRead(fileId);
		QDomElement docRootId = docId.documentElement();
		map(docRootId, mapId);
		docRoot = doc.documentElement();
		changeId(docRoot, mapId);
		QString filename = fileIn.fileName();
		QFile fileOut( filename );
		fileOpenForWrite(fileOut, doc);
		return 0;
	}
}

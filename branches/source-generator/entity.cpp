/** @file entity.cpp
 * 	@brief Описания вспомогательных классов сущностей
 * */
#include <QDebug>
#include "entity.h"
#include "generator.h"

void Entity::addProperty( QString name, QString type ){
	if( !name.isEmpty())
		if( !properties.contains(QPair<QString, QString>(name, type)))
			properties << QPair<QString, QString>(name, type);
}

void Entity::addProperties( const QList< QPair<QString, QString> > &list){
	for (int i = 0; i < (int) list.size(); i++)
		addProperty(list.at(i).first, list.at(i).second);
}

void Edge::addAssociation( Association* ass){
	bool found = false;

	for( int i=0; i< associations.size(); i++)
		if( ( associations.at(i)->from == ass->from ) && ( associations.at(i)->to == ass->to ) )
			found = true;
	if( !found )
		associations.push_back(ass);
}

bool Entity::parseGeneralizations(QDomElement &xml_element)
{
	QDomElement gen, par;
	QDomElement gens;

	gens = xml_element.firstChildElement("generalizations");
	if (gens.isNull())   // Not an error
		return true;

	for (gen = gens.firstChildElement("generalization"); !gen.isNull();
	     gen = gen.nextSiblingElement("generalization"))
	{
		for (par = gen.firstChildElement("parent"); !par.isNull();
		     par = par.nextSiblingElement("parent"))
		{
			QString par_id = par.attribute("parent_id");
			if (par_id == "")
			{
				qDebug() << "Anonymous parent of node \"" << id << "\"";
				return false;
			}
			parents << par_id;
		}
	}
	return true;
}

bool Entity::resolve(void)
{
	const Entity *parent;
	QString p;

	if (resolving_done) // Already resolved
		return true;

	Q_FOREACH(p, parents)
	{
		parent = cat->findEntityInTree(p);
		if (!parent)
		{
			qDebug() << "Cannot find" << p << "as a parent of" << id;
			return false;
		}
		// Parent may be unresolved if it is in the same category
		if (!parent->isResolved())
		{
			// Force resolving
			if (!const_cast<Entity*>(parent)->resolve())
				return false;
		}
		if (!applyParent(parent))
		{
			qDebug() << "Failed to apply parent" << p << "to" << id;
			return false;
		}
	}
	resolving_done = true;
	return true;
}

bool Entity::parseProperties(QDomElement &xml_element)
{
	QDomElement props, prop;

	props = xml_element.firstChildElement("properties");
	if (props.isNull())   // Not an error
		return true;

	for (prop = props.firstChildElement("property"); !prop.isNull();
	     prop = prop.nextSiblingElement("property"))
	{
		QDomNamedNodeMap attrs = prop.attributes();
		QString name;
		QString type;

		name = attrs.namedItem("name").toAttr().value();
		if (name == "") name = prop.firstChildElement("name").text();
		if (name == "")
		{
			qDebug() << "Anonymous prop of " << id << " entity";
			return false;
		}

		type = attrs.namedItem("type").toAttr().value();
		if (type == "") type = prop.firstChildElement("type").text();
		if (type == "")
		{
			qDebug() << "Unspecified type for prop" << name
			         << "of" << id << "entity";
			return false;
		}

		if (type == "enum")
			type = "enum " + prop.firstChildElement("enum").attribute("idref");
		else if (type == "ref")
			type = prop.firstChildElement("ref").attribute("idref");
		else if (type != "int" && type != "string" && type != "bool" &&
		         type != "positiveInt" && type != "nonNegativeInt" && type != "text")
		{
			qDebug() << "Unknown type for prop " << name << " of "
			         << id << " entity";
			return false;
		}
		addProperty(name, type);
	}
	return true;
}

bool Entity::parseAssociations(QDomElement &xml_element)
{
	QDomElement child, assocs;
	Edge* edge;

	assocs = xml_element.firstChildElement("associations");
	if (assocs.isNull()) // Not an error
		return true;

	if(type == NODE){
		return true; // WTF???
#if 0
		edge = new Edge(cat);
		edge->id = QString("untitledEdge_%1").arg(untitled);
		edge->name = QString("embedded edge #%1").arg(untitled);
		edge->height = -1;
		edge->width = -1;
#endif
	}
	else
		edge = dynamic_cast<Edge*>(this);
	//TODO: multiple associations support
	Association* ass = new Association();
	for (child = assocs.firstChildElement("association"); !child.isNull();
	     child = child.nextSiblingElement("association"))
	{
		QString role = child.attribute("id");
		QString arrowType = child.attribute("end_type");
		QDomElement begin = child.firstChildElement("begin");
		QDomElement end = child.firstChildElement("end");
		if (!begin.isNull()){
			ass->from = begin.attribute("idref");
			ass->fromID = role;
			ass->fromArrow = arrowType;
		}
		if (!end.isNull()){
			ass->to = end.attribute("idref");
			ass->toID = role;
			ass->toArrow = arrowType;
		}
	}
	for (child = assocs.firstChildElement("assoc_ref"); !child.isNull();
	     child = child.nextSiblingElement("assoc_ref"))
	{
		// sanity is for the weak!
	}
	edge->associations << ass;
	return true;
}

bool Entity::applyParent(const Entity *parent)
{
	addProperties(parent->properties);
	return true;
}

bool Node::init(QDomElement &xml_element)
{
	id = xml_element.attribute("id");
	name = xml_element.attribute("name");
	if (id == "" || name == "")
	{
		qDebug() << "anonymous node found";
		return false;
	}

	QDomElement logic = xml_element.firstChildElement("logic");
	if (logic.isNull())
	{
		// FIXME: statemachines_metamodel.xml/smTimeEvent
		qDebug() << "WARNING: cannot find <logic> tag of" << id << "entity";
		return true;
	}
	// WTF????
	addProperty("name", "string");

	if (!parseGeneralizations(logic)) return false;
	if (!parseProperties(logic)) return false;
	if (!parseAssociations(logic)) return false;

	if (!parseSdf(xml_element)) return false;
	if (!parsePorts(xml_element)) return false;
	if (!parseLabels(xml_element)) return false;

	return true;
}

bool Edge::init(QDomElement &xml_element)
{
	id = xml_element.attribute("id");
	name = xml_element.attribute("name");
	if (id == "" || name == "")
	{
		qDebug() << "anonymous node found";
		return false;
	}

	QDomElement logic = xml_element.firstChildElement("logic");
	if (logic.isNull())
	{
		qDebug() << "cannot find <logic> tag of \"" << id << "\" entity";
		return false;
	}
	// WTF????
	addProperty("name", "string");
	addProperty("from", "string");
	addProperty("to", "string");
	addProperty("fromPort", "string");
	addProperty("toPort", "string");

	if (!parseEdgeGraphics(xml_element)) return false;
	if (!parseGeneralizations(logic)) return false;
	if (!parseProperties(logic)) return false;
	if (!parseAssociations(logic)) return false;
	if (!parseLabels(logic)) return false;

	width = height = -1;
	return true;
}

bool Node::parseSdf(QDomElement &xml_element)
{
	QDir dir;

	dir.mkdir("generated");
	dir.cd("generated");
	QDomNodeList sdf = xml_element.elementsByTagName("picture");
	if( !dir.exists("shapes") )
		dir.mkdir("shapes");

	if (!sdf.isEmpty())
	{
		height = sdf.at(0).toElement().attribute("sizey").toInt();
		width = sdf.at(0).toElement().attribute("sizex").toInt();

		resources += res.arg("shapes/" + id + "Class.sdf");

		QFile file("generated/shapes/" + id + "Class.sdf");
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qDebug() << "Sdf file creation error";
			dir.cd("..");
			return false;
		}
		QTextStream stream(&file);
		sdf.at(0).save(stream, 1);
		file.close();

		visible = true;
	}
	else
	{
		height = -1;
		width = -1;
		visible = (id == "krnnNamedElement");
	}
	dir.cd("..");
	return true;
}

bool Node::parsePorts(QDomElement &xml_element)
{
	QDomNodeList point_ports_list = xml_element.elementsByTagName("point_port");
	for( int i=0; i<point_ports_list.size(); i++ ){
		Port port;
		port.type = "point";
		port.vals << (qreal) point_ports_list.at(i).toElement().attribute("x").toInt()/width;
		port.vals << (qreal) point_ports_list.at(i).toElement().attribute("y").toInt()/height;
		ports << port;
	}

	QDomNodeList line_ports_list = xml_element.elementsByTagName("line_port");
	for( int i=0; i<line_ports_list.size(); i++ ){
		QDomElement start = line_ports_list.at(i).firstChildElement("start");
		QDomElement end   = line_ports_list.at(i).firstChildElement("end");
		Port port;
		port.type = "line";
		port.vals << (qreal) start.attribute("startx").toInt()/width;
		port.vals << (qreal) start.attribute("starty").toInt()/height;
		port.vals << (qreal) end.attribute("endx").toInt()/width;
		port.vals << (qreal) end.attribute("endy").toInt()/height;
		ports << port;
	}
	return true;
}

bool Edge::parseEdgeGraphics(QDomElement &xml_element)
{
	QDomNode lineTypeNode = xml_element.elementsByTagName("line_type").at(0);
	if(!lineTypeNode.isNull()){
		QString type = lineTypeNode.toElement().attribute("type");
		if( type == "noPan"){ // temporary kludge
			type = "solidLine";
		}
		lineType = "Qt::" + type.replace(0,1,type.at(0).toUpper());
	}
	return true;
}

bool Entity::parseLabels(QDomElement &xml_element)
{
	QDomNodeList htmls = xml_element.elementsByTagName("html:html");
	for( int i=0; i < htmls.size(); i++ ){
		Label l;
		QString txt;
		QTextStream stream(&txt);
		l.x = (qreal)htmls.at(i).toElement().attribute("x").toInt()/width;
		l.y = (qreal)htmls.at(i).toElement().attribute("y").toInt()/height;

		QDomNodeList tfrs = htmls.at(i).toElement().elementsByTagName("html:text_from_repo");
		int x = tfrs.size();
		if( x > 0){
			for( int j=0; j<x; j++){
				QDomNode par = tfrs.at(0).parentNode();
				QString role = tfrs.at(0).toElement().attribute("name");

				const QDomText data = xml_element.ownerDocument().createTextNode(" %" + QString::number(j+1));
				if( role == "name" )
					l.args << "Qt::DisplayRole";
				else
					l.args << "Unreal::" + id + "::" + role + "Role";
				par.replaceChild(data, tfrs.at(0));
			}
		}
		QDomNodeList texts = htmls.at(i).toElement().elementsByTagName("html:text");
		int y = texts.size();
		if( y > 0 ){
			for( int j=0; j<y; j++){
				QDomNode par = texts.at(0).parentNode();
				QString role = texts.at(0).toElement().attribute("text");
				par.removeChild(texts.at(0));
				const QDomText data = xml_element.ownerDocument().createTextNode(role);
				par.appendChild(data);
			}


		}
		for( int j=0; j<htmls.at(0).childNodes().size(); j++)
			htmls.at(0).childNodes().at(j).save(stream, 1);
		txt.replace(QString("\n"), QString(" "));
		txt.replace(QString("\""), QString("\\\""));
		txt.replace(QString("html:"), QString(""));
		l.text = txt.simplified();
		labels << l;
	}
	return true;
}

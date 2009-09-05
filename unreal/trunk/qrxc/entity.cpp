/** @file entity.cpp
 * 	@brief Описания вспомогательных классов сущностей
 * */
#include <QDebug>
#include "entity.h"
#include "generator.h"

Entity::~Entity()
{
	RealProperty *p;

	while (!own_properties.isEmpty())
	{
		p = own_properties.takeFirst();
		delete p;
	}
}

bool Entity::addProperty(RealProperty *prop){
	FOR_ALL_PROPERTIES(this, p)
		if ((*p)->getName() == prop->getName()) // duplicate!
		{
			// Different objects?
			if ((*p)->getOwner() != prop->getOwner())
			{
				qDebug() << "Two properties with the same name"
						 << (*p)->getName() << "of objects"
						 << prop->getOwner()->id << "and"
						 << (*p)->getOwner()->id;
				return false;
			}

			// The same (this) object
			if ((*p)->getOwner() == this)
			{
				qDebug() << "Two properties with the same name"
						 << (*p)->getName() << "of object" << id;
				return false;
			}

			// These pointers are the same property.
			// Beware multiple inheritance!
			if ((*p) == prop)
				return true;

			qDebug() << "Should not happed, please, report!";
			return false;
		}
	if (prop->getName() == "name")
		properties.insert(0, prop);
	else
		properties << prop;
	if (prop->getOwner() == this)
		own_properties << prop;
	return true;
}

bool Entity::addParentProperties(const Entity *parent){
	FOR_ALL_PROPERTIES(parent, p)
		if (!addProperty((*p)))
			return false;
	return true;
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
				qDebug() << "Anonymous parent of node" << id;
				return false;
			}

			// Not a big error if duplicated
			if (!parents.contains(par_id))
				parents << par_id;
			else
				qDebug() << "WARNING: parent" << par_id
						 << "is already specified for" << id;
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
		RealProperty *p = new RealProperty(this);

		if (!p->init(prop))
		{
			delete p;
			return false;
		}
		if (!addProperty(p))
		{
			delete p;
			return false;
		}
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
		return true; // анонимные линки не обратабываем :(
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
	return addParentProperties(parent);
}

bool Node::init(QDomElement &xml_element)
{
	elem = xml_element;
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
		qDebug() << "cannot find <logic> tag of" << id << "entity"
				 << "in category" << cat->get_name();
		return false;
	}

	// Baptizo te in nomine Patris et Filii et Spiritus Sancti
	if (id != "krnnNamedElement" && id != "krnnElement")
		parents << "krnnNamedElement";

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
		qDebug() << "cannot find <logic> tag of" << id << "edge"
				 << "in category" << cat->get_name();
		return false;
	}

	// Baptizo te in nomine Patris et Filii et Spiritus Sancti
	if (id != "krneRelationship")
		parents << "krneRelationship";
	else
		parents << "krnnNamedElement";

	if (!parseEdgeGraphics(xml_element) || !parseGeneralizations(logic) || !parseProperties(logic) ||
		!parseAssociations(logic) || !parseLabels(logic) 
	) 
		return false;

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

		Generator *generator = cat->get_editor()->getGenerator();
		generator->addResource(res.arg(id + "Ports.sdf"));
		generator->addResource(res.arg(id + "Class.sdf"));

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
		visible = (id == "krnnNamedElement");

	dir.cd("..");
	return true;
}

bool Node::parsePorts(QDomElement &xml_element)
{
	QDir dir;
	dir.cd("generated");
	QDomNodeList sdf = xml_element.elementsByTagName("ports");
	if (!dir.exists("shapes"))
		dir.mkdir("shapes");

	if (!sdf.isEmpty())
	{
		QFile file("generated/shapes/" + id + "Ports.sdf");
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qDebug() << "Sdf file creation error";
			dir.cd("..");
			return false;
		}

		QTextStream stream(&file);

		stream << "<picture ";
		stream << "sizex=\"" << xml_element.elementsByTagName("picture").at(0).toElement().attribute("sizex").toInt() << "\" ";
		stream << "sizey=\"" << xml_element.elementsByTagName("picture").at(0).toElement().attribute("sizey").toInt() << "\" ";
		stream << ">\n";

		QDomNodeList pointPorts = xml_element.elementsByTagName("point_port");
		for (int i = 0; i < pointPorts.size(); ++i)
		{
			QDomElement portelem = pointPorts.at(i).toElement();
			stream << "\t<point stroke-width=\"11\" stroke-style=\"solid\" stroke=\"#c3dcc4\" ";
			stream << "x1=\""<<portelem.attribute("x") << "\" y1=\""<<portelem.attribute("y") << "\" ";
			stream << "/>\n";

			stream << "\t<point stroke-width=\"3\" stroke-style=\"solid\" stroke=\"#465945\" ";
			stream << "x1=\"" << portelem.attribute("x") << "\" y1=\"" << portelem.attribute("y") << "\" ";
			stream << "/>\n";

			Port port;
			port.type = "point";

						port.vals << (qreal) portelem.attribute("x").toInt()/width;
						port.vals << (qreal) portelem.attribute("y").toInt()/height;

			ports << port;
		}

		QDomNodeList linePorts = xml_element.elementsByTagName("line_port");
		for (int i = 0; i < linePorts.size(); ++i)
		{
			QDomElement portelem_start = linePorts.at(i).firstChild().toElement();
			QDomElement portelem_end = linePorts.at(i).lastChild().toElement();
			stream << "\t<line x1=\"" << portelem_start.attribute("startx") << "\" y1=\"" << portelem_start.attribute("starty") << "\" ";
			stream << "x2=\"" << portelem_end.attribute("endx") << "\" y2=\"" << portelem_end.attribute("endy") << "\" ";
			stream << "stroke-width=\"7\" stroke-style=\"solid\" stroke=\"#c3dcc4\" ";
			stream << "/>\n";

			stream << "\t<line x1=\"" << portelem_start.attribute("startx") << "\" y1=\""<<portelem_start.attribute("starty") << "\" ";
			stream << "x2=\""<<portelem_end.attribute("endx") << "\" y2=\"" << portelem_end.attribute("endy") << "\" ";
			stream << "stroke-width=\"1\" stroke-style=\"solid\" stroke=\"#465945\" ";
			stream << "/>\n";

			Port port;
			port.type = "line";

						QString startx = portelem_start.attribute("startx");
						QString starty = portelem_start.attribute("starty");
						QString endx = portelem_end.attribute("endx");
						QString endy = portelem_end.attribute("endy");

						if (startx.endsWith("a"))
						{
							startx.remove(startx.length() - 1, 1);
							port.vals << (qreal)startx.toInt() / width;
							port.props << true;
						} else if (startx.endsWith("%"))
						{
							startx.remove(startx.length() - 1, 1);
							port.vals << (qreal)startx.toInt() / 100;
							port.props << false;
						}  else
						{
							 port.vals << (qreal)startx.toInt()/width;
							 port.props << false;
						}


						if (starty.endsWith("a"))
						{
							starty.remove(starty.length() - 1, 1);
							port.vals << (qreal)starty.toInt() / height;
							port.props << true;
						} else if (starty.endsWith("%"))
						{
							starty.remove(starty.length() - 1, 1);
							port.vals << (qreal)starty.toInt() / 100;
							port.props << false;
						}  else
						{
							 port.vals << (qreal)starty.toInt() / height;
							 port.props << false;
						}


						if (endx.endsWith("a"))
						{
							endx.remove(endx.length() - 1, 1);
							port.vals << (qreal)endx.toInt() / width;
							port.props << true;
						} else if (endx.endsWith("%"))
						{
							endx.remove(endx.length() - 1, 1);
							port.vals << (qreal)endx.toInt() / 100;
							port.props << false;
						}  else
						{
							 port.vals << (qreal)endx.toInt() / width;
							 port.props << false;
						}


						if (endy.endsWith("a"))
						{
							endy.remove(endy.length() - 1, 1);
							port.vals << (qreal)endy.toInt() / height;
							port.props << true;
						} else if (endy.endsWith("%"))
						{
							endy.remove(endy.length() - 1, 1);
							port.vals << (qreal)endy.toInt() / 100;
							port.props << false;
						}  else
						{
							 port.vals << (qreal)endy.toInt() / height;
							 port.props << false;
						}


						ports << port;
		}
				stream << "</picture>\n";
		file.close();

	} else
	{
		QFile file("generated/shapes/" + id + "Ports.sdf");
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qDebug() << "Sdf file creation error";
			dir.cd("..");
			return false;
		}

		QTextStream stream(&file);
		stream << "<picture ";
		stream << "sizex=\"" << xml_element.elementsByTagName("picture").at(0).toElement().attribute("sizex").toInt() << "\" ";
		stream << "sizey=\"" << xml_element.elementsByTagName("picture").at(0).toElement().attribute("sizey").toInt() << "\" ";
		stream << "\\>\n";
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
		QString sdfType = type;
		sdfType.remove("Line").toLower();
		qDebug() << name << type << sdfType;

		OutFile out("generated/shapes/" + id + "Class.sdf");
		out() << "<picture sizex=\"100\" sizey=\"60\" >\n" <<
				"\t<line fill=\"#000000\" stroke-style=\"" << sdfType << "\" stroke=\"#000000\" y1=\"0\" " <<
				"x1=\"0\" y2=\"60\" stroke-width=\"2\" x2=\"100\" fill-style=\"solid\" />\n" <<
				"</picture>";

		Generator *generator = cat->get_editor()->getGenerator();
		generator->addResource(res.arg(id + "Class.sdf"));

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

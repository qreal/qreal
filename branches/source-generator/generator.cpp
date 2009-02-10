/** @file generator.cpp
 * 	@brief Генератор классов используемых на диаграммах элементов
*/
#include <QtGui/QPainter>

#include "generator.h"

Generator::Generator()
{
	resources = "<!DOCTYPE RCC><RCC version=\"1.0\">\n<qresource>\n";
	res = "\t<file>%1</file>\n";

	untitled = 0;
	objectsCount = 0;
}

bool Generator::work( QStringList files ){

	// creating directory for generated stuff
	dir.cd(".");
	dir.mkdir("generated");
	if (!dir.cd("generated"))
	{
		qDebug() << "cannot chdir() to 'generated' directory";
		return false;
	}

	// parse all files
	for (int i=0; i<files.size(); i++){
		qDebug() << "processing file " << files.at(i);
		if (!parseFile("editors/" + files.at(i)))
			return false;
	}
	// propagate properties and other useful things
	propagateAll();

	// generate all the stuff needed
	genEnums();
	genTypes();
	genClasses();
	genFactory();
	genRealRepoInfo();
	genEdgesFunction();

	// write the resource file
	QFile file("generated/real_dynamic.qrc");
	if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
		return false;
	QTextStream out(&file);
	resources += "</qresource>\n</RCC>";

	out << resources;

	file.close();

	qDebug() << "done";
	return true;
}

Generator::~Generator(){
	// doc is already deleted.

	// delete each category in the list
	while (!categories.isEmpty())
	   delete categories.takeFirst();

	// array of references to objects elements, so just clear
	edges.clear();

	// delete each object in the list
	while (!objects.isEmpty())
	   delete objects.takeFirst();
}

bool Generator::parseFile( QString filename ){

	doc = new QDomDocument("+1"); // :)
	QFile file(filename);
	if( !file.open(QIODevice::ReadOnly)){
		qDebug() << "incorrect filename " << filename;
		return false;
	}
	QString error = "";
	int errorLine = 0;
	int errorCol = 0;
	if( !doc->setContent(&file, false, &error, &errorLine, &errorCol)){
		file.close();
		qDebug() << "parse error in " << filename << ", error is " << error
			<< ". error line is " << errorLine << ", column is " << errorCol;
		return false;
	}
	file.close();

	Category* cat = new Category();
	cat->name = doc->elementsByTagName("diagram").at(0).toElement().attribute("name");
	categories << cat;

	// I. parsing enums
	QDomNodeList enumList = doc->elementsByTagName("enumType");
	for( int i=0; i < (int) enumList.length(); i++){
		parseEnum( enumList.at(i) );
	}

	// II. parsing nodes
	QDomNodeList nodeList = doc->elementsByTagName("node");
	for( int i=0; i < (int) nodeList.length(); i++ ){
		parseNode( nodeList.at(i) );
	}

	// III. parsing edges
	QDomNodeList edgeList = doc->elementsByTagName("edge");
	for( int i=0; i < (int) edgeList.length(); i++ ){
		parseEdge( edgeList.at(i) );
	}

	if( categories.last()->objects.size() == 0 )
		categories.removeLast();

	delete doc;
	return true;
}

bool Generator::parseEnum( QDomNode dnode ){

	QDomElement cur = dnode.toElement();
	QStringList values;
	QString name  = cur.attribute("id");
	QString qname = cur.attribute("name");
	QDomNodeList vals = cur.elementsByTagName("enumValue");
	values << qname;
	for( int k = 0; k < (int) vals.length(); k++ )
		values << vals.at(k).toElement().text();
	enumerations.insert(name, values);
	return true;
}

bool Generator::parseNode( QDomNode dnode ){

	Node *cur = new Node();
	cur->id  = dnode.toElement().attribute("id");
	cur->name = dnode.toElement().attribute("name");
	QDomElement logic = dnode.firstChildElement("logic");
	bool isNode = true;

	cur->addProperty("name", "string");

	parseGeneralizations( cur, logic );
	parseProperties( cur, logic );
	parseAssociations( cur, logic, isNode);
	parseSdf( cur, dnode );
	parsePorts( cur, dnode );
	parseLabels( cur, dnode );

	objects << cur;
	categories.at(categories.size()-1)->objects << objectsCount;
	objectsCount++;
	return true;
}

bool Generator::parseEdge( QDomNode dnode ){

	Edge *cur = new Edge();
	cur->id  = dnode.toElement().attribute("id");
	cur->name = dnode.toElement().attribute("name");
	QDomElement logic = dnode.firstChildElement("logic");
	bool isNode = false;

	// quick hack to make these props be on top
	cur->addProperty("name", "string");
	cur->addProperty("from", "string");
	cur->addProperty("to", "string");
	cur->addProperty("fromPort", "string");
	cur->addProperty("toPort", "string");

	parseEdgeGraphics( cur, dnode );
	parseGeneralizations( cur, logic );
	parseProperties( cur, logic );
	parseAssociations( cur, logic, isNode );
	parseLabels( cur, dnode );

	cur->height = -1;
	cur->width = -1;
	edges << cur;
	objects << cur;

	categories.at(categories.size()-1)->objects << objectsCount;
	objectsCount++;
	return true;
}

bool Generator::parseGeneralizations( Entity* cur, QDomNode logic ){

	QDomNodeList gens = logic.firstChildElement("generalizations").toElement().elementsByTagName("generalization");
	// for each <generalization> tag
	for( int i=0; i < (int) gens.size(); i++ ){
		QString parentID = gens.at(i).firstChildElement("parent").attribute("parent_id");
		cur->addParent( parentID );
	}
	return true;
}

bool Generator::parseProperties( Entity* cur, QDomNode logic ){

	QDomNodeList props = logic.toElement().elementsByTagName("properties");
	for( int i=0; i<props.size(); i++){  // for each <properties>...</properties> section
		QDomNodeList property = props.at(i).toElement().elementsByTagName("property");
		for( int j=0; j<property.size(); j++ ){   // for each <property ... > tag
				QDomNamedNodeMap attrs = property.at(j).attributes();
				QString name;
				QString type;
				if( !attrs.isEmpty() ){
					name = attrs.namedItem("name").toAttr().value();
					type = attrs.namedItem("type").toAttr().value();
				}
				else{
					name = property.at(j).firstChildElement("name").text();
					type = property.at(j).firstChildElement("type").text();
				}
				if( type == "enum" ){
					type = "enum " + property.at(j).firstChildElement("enum").attribute("idref");
				}
				if( type == "ref" ){
					type = property.at(j).firstChildElement("ref").attribute("idref");
				}
				cur->addProperty( name, type );
				// TODO: defaults and other missing property stuff support
		}
	}
	return true;
}

bool Generator::parsePorts( Node* cur, QDomNode dnode ){

	QDomNodeList ports = dnode.toElement().elementsByTagName("point_port");
	for( int i=0; i<ports.size(); i++ ){
		Port port;
		port.type = "point";
		port.vals << (qreal) ports.at(i).toElement().attribute("x").toInt()/cur->width;
		port.vals << (qreal) ports.at(i).toElement().attribute("y").toInt()/cur->height;
		cur->ports << port;
	}

	QDomNodeList lines = dnode.toElement().elementsByTagName("line_port");
	for( int i=0; i<lines.size(); i++ ){
		QDomElement start = lines.at(i).firstChildElement("start");
		QDomElement end   = lines.at(i).firstChildElement("end");
		Port port;
		port.type = "line";
		port.vals << (qreal) start.attribute("startx").toInt()/cur->width;
		port.vals << (qreal) start.attribute("starty").toInt()/cur->height;
		port.vals << (qreal) end.attribute("endx").toInt()/cur->width;
		port.vals << (qreal) end.attribute("endy").toInt()/cur->height;
		cur->ports << port;
	}
	return true;
}

bool Generator::parseEdgeGraphics( Edge* cur, QDomNode dnode ){

	QDomNode lineType = dnode.toElement().elementsByTagName("line_type").at(0);
	if( lineType != QDomNode() ){
		QString type = lineType.toElement().attribute("type");
		if( type == "noPan"){ // temporary kludge
			type = "solidLine";
		}
		cur->lineType = "Qt::" + type.replace(0,1,type.at(0).toUpper());
	}
	return true;
}

bool Generator::parseLabels( Entity* cur, QDomNode dnode ){
	QDomNodeList htmls = dnode.toElement().elementsByTagName("html:html");
	for( int i=0; i < htmls.size(); i++ ){
		Label l;
		QString txt;
		QTextStream stream(&txt);
		l.x = (qreal)htmls.at(i).toElement().attribute("x").toInt()/cur->width;
		l.y = (qreal)htmls.at(i).toElement().attribute("y").toInt()/cur->height;

		QDomNodeList tfrs = htmls.at(i).toElement().elementsByTagName("html:text_from_repo");
		int x = tfrs.size();
		if( x > 0){
			for( int j=0; j<x; j++){
				QDomNode par = tfrs.at(0).parentNode();
				QString role = tfrs.at(0).toElement().attribute("name");

				const QDomText data = doc->createTextNode(" %" + QString::number(j+1));
				if( role == "name" )
					l.args << "Qt::DisplayRole";
				else
					l.args << "Unreal::" + cur->id + "::" + role + "Role";
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
				const QDomText data = doc->createTextNode(role);
				par.appendChild(data);
			}


		}
		for( int j=0; j<htmls.at(0).childNodes().size(); j++)
			htmls.at(0).childNodes().at(j).save(stream, 1);
		txt.replace(QString("\n"), QString(" "));
		txt.replace(QString("\""), QString("\\\""));
		txt.replace(QString("html:"), QString(""));
		l.text = txt.simplified();
		cur->labels << l;
	}
	return true;
}

bool Generator::parseSdf( Entity* cur, QDomNode dnode ){

	QDomNodeList sdf = dnode.toElement().elementsByTagName("picture");
	if( !dir.exists("shapes") )
		dir.mkdir("shapes");

	if (!sdf.isEmpty())
	{
		cur->height = sdf.at(0).toElement().attribute("sizey").toInt();
		cur->width = sdf.at(0).toElement().attribute("sizex").toInt();

		resources += res.arg("shapes/" + cur->id + "Class.sdf");

		QFile file("generated/shapes/" + cur->id + "Class.sdf");
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qDebug() << "Sdf file creation error";
			return false;
		}
		QTextStream stream(&file);
		sdf.at(0).save(stream, 1);
		file.close();

		cur->visible = true;
	}
	else
	{
		cur->height = -1;
		cur->width = -1;
		cur->visible = ( cur->id == "krnnNamedElement" );
	}
	return true;
}

bool Generator::parseAssociations( Entity *cur, QDomNode logic, bool isNode ){

	Edge* edge;
	QDomNodeList assocs = logic.toElement().elementsByTagName("association");
	if( assocs.size() == 0 )
		return true;
	if( isNode ){
		return true;
		edge = new Edge();
		edge->id = QString("untitledEdge_%1").arg(untitled);
		edge->name = QString("embedded edge #%1").arg(untitled);
		untitled++;
		edge->height = -1;
		edge->width = -1;
	}
	else
		edge = (Edge*) cur;
	//TODO: multiple associations support
	Association* ass = new Association();
	for( int i=0; i < assocs.size(); i++){
		QString role = assocs.at(i).toElement().attribute("id");
		QString arrowType = assocs.at(i).toElement().attribute("end_type");
		QDomElement begin = assocs.at(i).firstChildElement("begin");
		QDomElement end = assocs.at(i).firstChildElement("end");
		if( begin != QDomElement() ){
			ass->from = begin.toElement().attribute("idref");
			ass->fromID = role;
			ass->fromArrow = arrowType;
		}
		if( end != QDomElement() ){
			ass->to = end.toElement().attribute("idref");
			ass->toID = role;
			ass->toArrow = arrowType;
		}
	}
	edge->associations << ass;
	if( isNode ){
		objects << edge;
		edges << edge;
		categories.at(categories.size()-1)->objects << objectsCount;
		objectsCount++;
	}
	return true;
}

void Generator::genEnums()
{
	if( !dir.exists("repo") )
		dir.mkdir("repo");
	QFile file("generated/repo/realreporoles.h");
	if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
		return;
	QTextStream out(&file);

	int id = NUM;

	out << "#ifndef REALREPOROLES_H\n#define REALREPOROLES_H\n\n";

	QString tmp2 = "#include \"%1\"\n";

	out << "namespace UML {\n";
	out << "\tenum ElementTypes{\n";
	for( int i=0; i < objects.size(); i++ ){
		out << "\t\t" +  objects.at(i)->id +
						"=" + QString::number(id++);
		if( i!=objects.size()-1 )
			out << ",";
		out << "\n";
	}
	out << "\t};\n};\n\n";

	out << "namespace Unreal {\n";
	out << "\tenum Roles {\n"
		   "\t\tPositionRole = Qt::UserRole + 64,   // Position on a diagram\n"
		   "\t\tConfigurationRole,          // Configuration (e.g. for link)\n"
		   "\t\tIdRole,\n"
		   "\t\tTypeRole,\n"
		   "\t\tUserRole = Qt::UserRole + 96        // First role available for other types\n\t};\n\n";

	for( int i=0; i < objects.size(); i++ ){
		out << "\tnamespace " + objects.at(i)->id + " {\n";
		out << "\t\tenum Roles {\n";
		for (int j=0; j<objects.at(i)->properties.size(); j++){
			out << "\t\t\t" + objects.at(i)->properties.at(j).first + "Role";
			if( !j )
				out << " = UserRole + 1";
			if( j != objects.at(i)->properties.size()-1)
				out << ",";
			out << "\n";
		}
	out << "\t\t};\n";
		out << "\t};\n\n";
   }

	out << "};\n\n";

	out <<
		   "namespace Unreal {\n\t"
		   "enum ClassRoles {\n\t\tFieldsRole = UserRole + 1,\n\t\tMethodsRole\n\t};\n\n\t"
		   "enum LinkRoles {\n\t\tFromRole = UserRole + 1,\n\t\tToRole,\n\t\tFromPortRole,\n\t\tToPortRole\n\t};\n};\n";


	out << "#endif\n";

	file.close();
	return;
}

void Generator::genTypes()
{
	if( !dir.exists("reposerver") )
		dir.mkdir("reposerver");
	dir.cd("reposerver");
	if( !dir.exists("generated") )
		dir.mkdir("generated");

	QFile file("generated/reposerver/generated/repotypesinfo.h");
	if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
		return;
	QTextStream out(&file);

	out << "#ifndef __REPO_TYPES_INFO_H__\n#define __REPO_TYPES_INFO_H__\n\n";
	out << "#include <QMap>\n#include <QString>\n#include \"../../common/classes.h\" //to be removed soon\n"
		"#include \"../../common/realrepoapiclasses.h\"\n"
		"#include \"../../common/defs.h\"\n\n";

	out << "\nusing namespace QRealTypes;\n\n";

	out << "class RepoTypesInfo\n{\n"
		"public:\n"
		"\tRepoTypesInfo();\n"
		"\t~RepoTypesInfo();\n"
		"\tint getTypesCount();\n"
		"\tRealType getTypeInfo( int );\n"
		"\tRealType getTypeInfo( QString );\n"
		"\tQString getTypesByMetatype( const MetaType );\n"
		"\tint analyseType( int );\n"
		"\tvoid elementCreated( int, int );\n"
		"\tvoid elementDeleted( int, int );\n"
		"private:\n"
		"};\n\n";

	out << "#endif\n\n";

	file.close();

	QFile file2("generated/reposerver/generated/repotypesinfo.cpp");
	if( !file2.open(QIODevice::WriteOnly | QIODevice::Text) )
		return;
	QTextStream out2(&file2);

	// static inits
	out2 << "#include \"repotypesinfo.h\"\n\n"
		"static bool initCompleted = false;\n\n"
		"static QMap<int, RealType> map;\n\n";


	out2 << "static void initStaticData()\n{\n"
		"\tif ( initCompleted )\n"
		"\t\treturn;\n\n"
		"\tRealType info;\n\n";

	out2 << "\n\t//metatype will be replaced with real values"
					" as soon as we start to use it in our XML descriptions\n\n";

	for (int i=0; i<objects.size(); i++){
		int j = i+NUM;
		out2 << "\tinfo.setId(" << j << ");\n"
			<< "\tinfo.setName(\"" << objects.at(i)->id << "\");\n"
			<< "\tinfo.setDescription(\"" << objects.at(i)->name << "\");\n"
			<< "\tinfo.setMetaType(QRealTypes::object);\n"
			<< QString("\tmap[%1] = info;\n\n").arg(j);

	}

	out2 << "\tinitCompleted = true;\n"
		"}\n\n";

	// constructor
	out2 << "RepoTypesInfo::RepoTypesInfo()\n"
		"{\n"
		"\tinitStaticData();\n"
		"}\n\n";

	// destructor
	out2 << "RepoTypesInfo::~RepoTypesInfo()\n{\n}\n\n";

	// getTypeInfo
	out2 << "RealType RepoTypesInfo::getTypeInfo( int id )\n{\n"
		"\treturn map[id];\n}\n\n";

	out2 << "RealType RepoTypesInfo::getTypeInfo( QString name )\n{\n"
		"\tforeach( RealType type, map.values() )\n"
		"\t\tif( type.getName() == name )\n"
		"\t\t\treturn type;\n"
		"\tqDebug() << \"wrong type requested!\";\n"
		"\treturn RealType();\n}\n\n";

	// getTypesCount
	out2 << "int RepoTypesInfo::getTypesCount()\n{\n"
		"\treturn map.size();\n}\n\n";

	// getTypesByMetatype
	out2 << "QString RepoTypesInfo::getTypesByMetatype( const MetaType id )\n{\n"
		"\tQString res = \"\";\n"
		"\tforeach( RealType type, map.values() )\n"
		"\t\tif( type.getMetaType() == id )\n"
		"\t\t\tres += QString(\"%1\\t\").arg(type.getId());\n"
		"\treturn res;\n}\n\n";

	// analyzeType
	out2 << "int RepoTypesInfo::analyseType( int type )\n{\n"
		"\tswitch (type)\n\t{\n";
	for( int i=0; i<objects.size(); i++ ){
		if( objects[i]->type == EDGE )
			out2 << "\t\tcase " << i + NUM << ":\n";
	}
	out2 << "\t\t\treturn TYPE_LINK;\n"
		"\t\tdefault:\n\t\t\treturn TYPE_OBJECT;\n\t}\n}\n\n";

	// elementCreated
	out2 << "void RepoTypesInfo::elementCreated( int type, int id )\n{\n"
		"\tmap[type].addObject(id);\n}\n\n";

	// elementDeleted
	out2 << "void RepoTypesInfo::elementDeleted( int type, int id )\n{\n"
		"\tmap[type].deleteObject(id);\n}\n\n";

	file2.close();
	dir.cdUp();
}

void Generator::genClasses(){

	//
	// I. elements
	//
	if( !dir.exists("umllib") )
		dir.mkdir("umllib");
	dir.cd("umllib");
	if( !dir.exists("generated") )
		dir.mkdir("generated");

	QFile file("generated/umllib/generated/objects.cpp");
		if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
			return;
	QTextStream out(&file);

	QFile file2("generated/umllib/generated/objects.h");
		if( !file2.open(QIODevice::WriteOnly | QIODevice::Text) )
			return;
	QTextStream out2(&file2);


	out <<  "#include <QtGui>\n"
			"#include \"objects.h\"\n\n"
			"using namespace Unreal;\n"
			"using namespace UML;\n\n";

	out2 << "#include <QWidget>\n#include <QList>\n\n"
			"#include \"uml_nodeelement.h\"\n"
			"#include \"uml_edgeelement.h\"\n"
			"#include \"sdfrenderer.h\"\n"
			"#include \"realreporoles.h\"\n"
			"namespace UML {\n";


	for (int i=0; i < objects.size(); i++){

		QString classname = objects.at(i)->id + "Class";

		if( objects.at(i)->type == EDGE )
			continue;

		//
		// 1. CPP-file
		//

		out << "// " << classname << "\n";

		//constructor
		out << classname << "::" << classname << "()\n";
		out <<   "{\n";

		out << "\tupdatePorts();\n"
			<< QString("\trenderer.load(QString(\"%1\"));\n").arg(":/shapes/" + classname + ".sdf")
			<< "\ttext = \"\";\n";
		if( objects.at(i)->parents.size() > 0)
			out << "\tparentsList";
		for( int j=0;j<objects.at(i)->parents.size(); j++ ){
			out << QString("  << %1").arg(position(objects.at(i)->parents.at(j)) + NUM);
		}
		out << ";\n";
		out << QString("\theight = %1;\n").arg(objects.at(i)->height)
			<< QString("\twidth = %1;\n").arg(objects.at(i)->width)
		<< "\tm_contents.setWidth(width);\n"
		<< "\tm_contents.setHeight(height);\n";

		if( objects.at(i)->type == NODE){
			Node* node = (Node*) objects.at(i);
			for( int j=0; j<node->ports.size(); j++ ){
				if( node->ports.at(j).type == "point" ){
						out << QString("\tpointPorts << QPointF(%1, %2);\n")
															.arg(node->ports.at(j).vals.at(0))
															.arg(node->ports.at(j).vals.at(1));
				}
				else if (node->ports.at(j).type == "line" ){
					out << QString("\tlinePorts << QLineF(%1, %2, %3, %4);\n")
											.arg(node->ports.at(j).vals.at(0))
											.arg(node->ports.at(j).vals.at(1))
											.arg(node->ports.at(j).vals.at(2))
											.arg(node->ports.at(j).vals.at(3));
				}
			}
		}
		out  << "}\n\n";

		//destructor
		out << classname << "::~" << classname << "()\n";
		out <<   "{\n}\n\n";


		//paint
		out << "void " << classname << "::paint(QPainter *painter, const QStyleOptionGraphicsItem *style,"
																				"QWidget *widget)\n{\n";
		out << "\tupdatePorts();\n"
			<< QString("\trenderer.render(painter, m_contents);\n\n")
			<< "\tQTextDocument d;\n"
			<< "\td.setHtml(text);\n";
		if( objects.at(i)->labels.size() > 0){
			out << "\tpainter->save();\n";
			if( objects.at(i)->id == "cnClass"){     // yeah, hate me. but no coordinates for labels allowed :/
				out << QString("\tpainter->translate(QPointF(0, 0));\n")
					<< "\td.setTextWidth(m_contents.width());\n"
					<< "\td.drawContents(painter, m_contents);\n";
			}
			else if( objects.at(i)->id == "krnnDiagram"){
				out << QString("\tpainter->translate(QPointF(0, 2*m_contents.height()/3 ));\n")
					<< "\tQRectF conts = m_contents;\n"
					<< "\tconts.setHeight(m_contents.height()/3);\n"
					<< "\td.setTextWidth(m_contents.width());\n"
					<< "\td.drawContents(painter, conts);\n";

			}
			else{
				if( objects.at(i)->labels.at(0).x != 0  || objects.at(i)->labels.at(0).y != 0)
					out << QString("\tpainter->translate(QPointF(%1 * m_contents.width(), %2 * m_contents.height()));\n")
							.arg(objects.at(i)->labels.at(0).x).arg(objects.at(i)->labels.at(0).y)
						<< "\tQRectF conts = m_contents;\n"
						<< QString("\tconts.setHeight(m_contents.height() * (1 - %1));\n")
							.arg(objects.at(i)->labels.at(0).y);
				else
					out << QString("\tpainter->translate(QPointF(0, m_contents.height()-15));\n")
						<< "\tQRectF conts = m_contents;\n"
						<< QString("\tconts.setHeight(20);\n");
				out << "\td.setTextWidth(m_contents.width());\n"
					<< "\td.drawContents(painter, conts);\n";
			}
			out << "\tpainter->restore();\n";
		}
		else
			out << "\td.drawContents(painter, m_contents);\n";
		out << "\tNodeElement::paint(painter, style, widget);\n"
			<< "}\n\n";

		//updateData
		out << "void " << classname << "::updateData()\n{\n"
			<< "\tNodeElement::updateData();\n";
		if( objects.at(i)->labels.size() > 0){
			out << QString("\ttext = QString(\"%1\")").arg(objects.at(i)->labels.at(0).text);
			if( objects.at(i)->labels.at(0).args.size() > 0)
				for( int k=0; k<objects.at(i)->labels.at(0).args.size(); k++)
					out << QString("\n\t\t\t.arg(dataIndex.data(%2).toString())")
								.arg(objects.at(i)->labels.at(0).args.at(k));
			out << ";\n";
		}
		else
			out << "\ttext = \"\";\n";
		out << QString("")
			<< "\tupdate();\n"
			<< "}\n\n";

		// updatePorts
		out << "void " + classname + "::updatePorts(){\n";
			   //"\tports.clear();\n";
		out << "}\n";

		out << "// " + classname  << "\n";

		//
		//2. H-files
		//

		out2 << "#ifndef " << classname.toUpper() << "_H\n#define " << classname.toUpper() << "_H\n\n";

		out2 << "class " << classname << " : public NodeElement{\n";
		out2 << "\tpublic:\n\t\t" << classname << "();\n";
		out2 << "\t\t~" << classname << "();\n";
		out2 <<  "\tvirtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);\n"
				"\tvirtual void updateData();\n\n"
		"private:\n"
				"\tvoid updatePorts();\n\n"
				"\tQString text;\n"
				"\t/* int textSize; */\n"
				"\tint width;\n"
				"\tint height;\n"
		"\tSdfRenderer renderer;\n";

		out2 << "};\n\n#endif\n";
	}
	//
	// II. edges
	//

	for ( int i=0; i<edges.size(); i++ ){

		//
		// 1. H-files
		//

		QString classname = edges.at(i)->id + "Class";
		out2 << "#ifndef " << classname.toUpper() << "_H\n#define " << classname.toUpper() << "_H\n\n";

		out2 << "\tclass " << classname << " : public EdgeElement{\n";
		out2 << "\tpublic:\n\t\t" << classname << "();\n";
		out2 << "\t\t~" << classname << "();\n";
		out2 <<  "\t\tvirtual void drawStartArrow(QPainter *) const;\n"
				"\t\tvirtual void drawEndArrow(QPainter *) const;\n";
		out2 << "};\n\n#endif\n";

		//
		// CPP-files
		//

		// constructor

		out << classname << "::" << classname << "()\n";
		out << QString("{\n\tm_penStyle = %1;\n").arg(edges.at(i)->lineType);
		if( edges.at(i)->labels.size() > 0){
			out << QString("\tm_text = QString(\"%1\")").arg(edges.at(i)->labels.at(0).text);
			if( edges.at(i)->labels.at(0).args.size() > 0 )
				out << QString(".arg(dataIndex.data(%1).toString());\n").arg(edges.at(i)->labels.at(0).args.at(0));
			else
				out << ";\n";
		}
		out << "}\n\n";

		// destructor

		out << classname << "::~" << classname << "()\n";
		out <<   "{\n}\n\n";

		// drawStartArrow

	QString style;
	if( edges.at(i)->associations.size() != 0 )
			style = edges.at(i)->associations.at(0)->fromArrow;
	else
		style = "";


		if( !style.isEmpty() && style != "no_arrow" ){
			out << "void " << classname << "::drawStartArrow(QPainter *painter) const\n";
			out <<   "{\n"
				"\tQBrush old = painter->brush();\n"
				"\tQBrush brush;\n"
				"\tbrush.setStyle(Qt::SolidPattern);\n";
			if( style.isEmpty() )
				style = "filled_arrow";
			if( style == "empty_arrow" || style == "empty_rhomb" || style == "complex_arrow" )
				out << "\tbrush.setColor(Qt::white);\n";
			if( style == "filled_arrow" || style == "filled_rhomb" )
				out << "\tbrush.setColor(Qt::black);\n";
			out << "\tpainter->setBrush(brush);\n";

			if( style == "empty_arrow" || style == "filled_arrow" )
				out << "\tstatic const QPointF points[] = {\n"
					"\t\tQPointF(0,0),\n\t\tQPointF(-5,10),\n\t\tQPointF(5,10)\n\t};\n"
					"\tpainter->drawPolygon(points, 3);\n";
			if( style == "empty_rhomb" || style == "filled_rhomb" )
				out << "\tstatic const QPointF points[] = {\n"
					"\t\tQPointF(0,0),\n\t\tQPointF(-5,10),\n\t\tQPointF(0,20),\n\t\tQPointF(5,10)\n\t};\n"
					"\tpainter->drawPolygon(points, 4);\n\t";
			if( style == "open_arrow" )
				out << "\tstatic const QPointF points[] = {\n"
					"\t\tQPointF(-5,10),\n\t\tQPointF(0,0),\n\t\tQPointF(5,10)\n\t};\n"
					"\tpainter->drawPolyline(points, 3);\n\t";
			if( style == "complex_arrow" )
				out << "\tstatic const QPointF points[] = {"
					"\n\t\tQPointF(-15,30),\n\t\tQPointF(-10,10),"
					"\n\t\tQPointF(0,0),\n\t\tQPointF(10,10),"
					"\n\t\tQPointF(15,30),\n\t\tQPointF(0,23),\n\t\tQPointF(-15,30)\n\t};\n"
					"\tpainter->drawPolyline(points, 7);\n\t";
			out << "painter->setBrush(old);\n}\n\n";

		}
		else
			out << "void " << classname << "::drawStartArrow(QPainter *) const\n"
				   "{\n}\n\n";

		// drawEndArrow
	if( edges.at(i)->associations.size() != 0 )
		style = edges.at(i)->associations.at(0)->toArrow;
	else
		style = "";
		if( !style.isEmpty() && style != "no_arrow" ){
			out << "void " << classname << "::drawEndArrow(QPainter * painter) const\n{\n"
				"\tQBrush old = painter->brush();\n"
				"\tQBrush brush;\n"
				"\tbrush.setStyle(Qt::SolidPattern);\n";
			if( style.isEmpty() )
				style = "filled_arrow";
			if( style == "empty_arrow" || style == "empty_rhomb" || style == "complex_arrow" )
				out << "\tbrush.setColor(Qt::white);\n";
			if( style == "filled_arrow" || style == "filled_rhomb" )
				out << "\tbrush.setColor(Qt::black);\n";
			out << "\tpainter->setBrush(brush);\n";

			if( style == "empty_arrow" || style == "filled_arrow" )
				out << "\tstatic const QPointF points[] = {\n"
					"\t\tQPointF(0,0),\n\t\tQPointF(-5,10),\n\t\tQPointF(5,10)\n\t};\n"
					"\tpainter->drawPolygon(points, 3);\n";
			if( style == "empty_rhomb" || style == "filled_rhomb" )
				out << "\tstatic const QPointF points[] = {\n"
					"\t\tQPointF(0,0),\n\t\tQPointF(-5,10),\n\t\tQPointF(0,20),\n\t\tQPointF(5,10)\n\t};\n"
					"\tpainter->drawPolygon(points, 4);\n\t";
			if( style == "open_arrow" )
				out << "\tstatic const QPointF points[] = {\n"
					"\t\tQPointF(-5,10),\n\t\tQPointF(0,0),\n\t\tQPointF(5,10)\n\t};\n"
					"\tpainter->drawPolyline(points, 3);\n\t";
			if( style == "complex_arrow" )
				out << "\tstatic const QPointF points[] = {"
					"\n\t\tQPointF(-15,30),\n\t\tQPointF(-10,10),"
					"\n\t\tQPointF(0,0),\n\t\tQPointF(10,10),"
					"\n\t\tQPointF(15,30),\n\t\tQPointF(0,23),\n\t\tQPointF(-15,30)\n\t};\n"
					"\tpainter->drawPolyline(points, 7);\n\t";
			out << "\tpainter->setBrush(old);\n}\n\n";
		}
		else
			out << "void " << classname << "::drawEndArrow(QPainter *) const\n"
				   "{\n}\n\n";

	}
	out2 << "}\n";
	file.close();
	file2.close();
	dir.cdUp();

	//
	// IV. pri-file
	//

	QFile file3("generated/umllib/generated/umllib.pri");
	if( !file3.open(QIODevice::WriteOnly | QIODevice::Text) )
		return;
	QTextStream out3(&file3);
	QString headers = "HEADERS += umllib/generated/objects.h\n";
	QString sources = "SOURCES += umllib/generated/objects.cpp\n";
	/*QString prefix = "umllib/generated/";

	for (int i=0; i < objects.size(); i++){

		headers += " \\ \n\t" + prefix + objects.at(i)->id + "Class.h";
		sources += " \\ \n\t" + prefix + objects.at(i)->id + "Class.cpp";

	}
	*/
	out3 << headers << "\n\n";
	out3 << sources << "\n";
	file3.close();
}



void Generator::genFactory()
{

	if( !dir.exists("umllib"))
		dir.mkdir("umllib");
	QFile file("generated/umllib/uml_guiobjectfactory.cpp");
	if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
		return;
	QTextStream out(&file);
	QString includes = "";
	QString classes = "";
	QString tmp = "\t\tcase UML::%1:\n\t\t\treturn new %2();\n";

	out <<  "#include <QtGui>\n\n"
			"#include \"realreporoles.h\"\n"
			"#include \"objects.h\"\n"
			"#include \"uml_guiobjectfactory.h\"\n\n";


	for (int i=0; i<objects.size(); i++){
		int height = objects.at(i)->height;
		int width  = objects.at(i)->width;

		if ( height == -1 && width == -1 )
			continue;
		classes += tmp.arg(objects.at(i)->id).arg(objects.at(i)->id + "Class") ;
	}

	for (int i=0; i<edges.size(); i++){

		classes += tmp.arg(edges.at(i)->id).arg(edges.at(i)->id + "Class");

	}

  //  out << includes << "\n\n";
	out <<  "using namespace UML;\n\n"
			"Element * UML::GUIObjectFactory(int type){\n"
			"\tswitch ( type ) {\n";
	out << classes;

	out << "\t\tdefault: return 0;//\t qDebug() << \"not creating\" << type; \n\t}\n";
	out << "\treturn 0;\n}\n";

	file.close();
}

int Generator::position( QString id ){

	int result = -1;
	for( int i=0; i<(int) objects.size(); i++ )
		if( objects.at(i)->id == id ){
			result = i;
			break;
		}
   return result;

}

void Generator::genRealRepoInfo(){

	QFile file("generated/repo/realrepoinfo.h");
	if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
		return;
	QTextStream out(&file);

	out << "#ifndef REALREPOINFO_H\n#define REALREPOINFO_H\n\n";

	out << "#include <QStringList>\n#include <QMap>\n#include <QString>\n#include <QIcon>\n#include \"sdfrenderer.h\"\n\n";

	out <<  "class RealRepoInfo\n{\n"
			"public:\n"
			"\tRealRepoInfo();\n"
			"\t~RealRepoInfo();\n"
			"\tQStringList getObjectCategories() const;\n"
			"\tQList<int> getObjects(int category) const;\n"
			"\tQString objectDesc(int id) const;\n"
			"\tQString objectName(int id) const;\n\n"
			"\tQStringList getColumnNames(int type) const;\n"
			"\tQString getColumnName(int type, int role) const;\n\n"
			"\tint roleByIndex(int index) const\n"
			"\t\t{ return index+129; };\n"
			"\tint indexByRole(int role) const\n"
			"\t\t{ return role-129; };\n\n"
			"\tQIcon objectIcon( int id ) const; \n\n"
			"private:\n"
			"};\n\n";

	out << "#endif\n\n";

	file.close();

	QFile file2("generated/repo/realrepoinfo.cpp");
	if( !file2.open(QIODevice::WriteOnly | QIODevice::Text) )
		return;
	QTextStream out2(&file2);

	// static inits
	out2 << "#include \"realrepoinfo.h\"\n\n"
			"static bool initCompleted = false;\n\n"

		"class Category\n{\n"
			"public:\n"
			"\tQString name;\n"
			"\tQList<int> objects;\n};\n\n"

			"static QList< Category > categories;\n"
			"static QStringList objects;\n"
			"static QStringList descriptions;\n"
			"static QList< QIcon > icons;\n"
			"static QMap<int, QStringList> map;\n\n";

	out2 << "static void initStaticData()\n{\n"
		"\tif ( initCompleted )\n"
		"\t\treturn;\n\n"
			"\tCategory cat;\n\n";
	for( int i=0; i<categories.size(); i++){

	bool isEmpty = true;
	for( int j=0; j<categories.at(i)->objects.size(); j++)
		if( objects.at(categories.at(i)->objects.at(j))->visible )
			isEmpty = false;
	//qDebug() << "cat " << categories.at(i)->name << ", empty " << isEmpty;
	if( isEmpty )
		continue;

	out2 << QString("\tcat.objects.clear();\n\tcat.name = \"%1\";\n").arg(categories.at(i)->name);

	if( categories.at(i)->objects.size() > 0 )
			out2 << QString("\tcat.objects ");

	if( i )
		out2 << "<< 2 << 18";

		for( int j=0; j<categories.at(i)->objects.size(); j++){
//		qDebug() << categories.at(i)->objects.at(j)+NUM << objects.at(categories.at(i)->objects.at(j))->name
//		<< objects.at(categories.at(i)->objects.at(j))->visible;
		if( objects.at(categories.at(i)->objects.at(j))->visible || objects.at(categories.at(i)->objects.at(j))->type == EDGE )
					out2 << QString(" << %1").arg(categories.at(i)->objects.at(j)+NUM);
	}

		out2 << "\t;\n";
		out2 << "\tcategories << cat;\n\n";

	}

	if( objects.size() > 0 )
		out2 << "objects ";
	for( int i=0; i<objects.size(); i++ )
		out2 << QString(" << \"%1\"").arg(objects.at(i)->id);
	out2 << ";\n\n";
	if( objects.size() > 0 )
		out2 << "descriptions ";
	for( int i=0; i<objects.size(); i++ ){
		out2 << QString(" << \"%1\"").arg(objects.at(i)->name);
	}

	out2 << ";\n\n";


	// from former realreporoles

	out2 << "// from realreporoles.cpp\n\n";
	out2 << "\tQStringList l;\n";

	for (int i=0; i<objects.size(); i++){
		out2 << "\tl.clear();\n";
		for( int j=0; j<objects.at(i)->properties.size(); j++)
			out2 << QString("\t\tl << \"%1\";\n").arg(objects.at(i)->properties.at(j).first);
		out2 << QString("\tmap.insert(%1, l);\n").arg(NUM + i);
		out2 << "\n";
	}

	// initializing icons

	out2 << "//initializing icons\n\n";
	out2 << "\ticons";

	for( int k=0; k<objects.size(); k++ ){
		if( objects.at(k)->height == -1 && objects.at(k)->width == -1 )
			out2 << "\n\t\t<< QIcon()";
		else
			out2 << QString("\n\t\t<< QIcon(new SdfIconEngineV2(\":/shapes/" + objects.at(k)->id + "Class.sdf\"))");
	}
	out2 << ";\n\n";

	out2 << "\tinitCompleted = true;\n"
		"}\n\n";

	// constructor

	out2 << "RealRepoInfo::RealRepoInfo()\n"
			"{\n"
			"\tinitStaticData();\n"
			"}\n\n";

	// destructor
	out2 << "RealRepoInfo::~RealRepoInfo()\n{\n}\n\n";

	// getObjectCategories
	out2 << "QStringList RealRepoInfo::getObjectCategories() const\n{\n"
			"\tQStringList l;\n"
			"\tfor( int i=0; i<categories.size(); i++)\n"
			"\t\tl << categories.at(i).name;\n"
			"\treturn l;\n}\n\n";

	// getObjects
	out2 << "QList<int> RealRepoInfo::getObjects(int category) const\n{\n"
			"\treturn categories.at(category).objects;\n}\n\n";

	// objectName
	out2 << "QString RealRepoInfo::objectName( int id ) const\n{\n"
			"\treturn objects.at(id-1);\n}\n\n";

	out2 << "QString RealRepoInfo::objectDesc( int id ) const\n{\n"
			"\treturn descriptions.at(id-1);\n}\n\n";

	// getColumnName

	out2 << "QString RealRepoInfo::getColumnName(int type, int role) const\n{\n"
			"\treturn map.value(type).at(indexByRole(role));\n}\n\n";

	// getColumnNames

	out2 << "QStringList RealRepoInfo::getColumnNames(int type) const\n{\n"
			"\treturn map.value(type);\n}\n\n";

	out2 << "QIcon RealRepoInfo::objectIcon( int id ) const\n"
			"{\n\tif ( id > 0 && id <= icons.size() )\n"
			"\t\treturn icons.at(id-1);\n"
			"\telse\n"
			"\t\treturn QIcon();\n}\n";


	file2.close();
}


void Generator::genEdgesFunction(){

	QFile file("generated/repo/edges_stuff.cpp");
	if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
		return;
	QTextStream out(&file);

	out <<  "#include <QMap>\n#include <QHash>\n#include <QDebug>\n\n"
			"#include \"uml_nodeelement.h\"\n"
			"#include \"objects.h\"\n";
	QString cases = "";
	QString singleCase = "\t\tcase %1: return check(pars, new %2()); break;\n";

	for( int i=0; i<objects.size(); i++)
		if( objects.at(i)->type == NODE){
			cases += singleCase.arg(i+NUM).arg(objects.at(i)->id + "Class");
		}
	out << "\n";
	out <<  "const int ANY = -1;\n"
			"const int NONE = 0;\n\n";

	out << "using namespace UML;\n";

	out << "int check(QList<int> pars, NodeElement* el){\n"
		   "\tfor( int i=0; i<pars.size(); i++)\n"
		   "\t\tif( el->isChildOf(pars.at(i)) ){\n"
		   "\t\t\treturn pars.at(i);\t\t\n\t}\n\n"
		   "\treturn NONE;\n}\n\n";

	out << "int checkParents(QList<int> pars, int id){\n"
		   "\tswitch(id){\n"
		   "\t\tcase -1: return -1; break;\n";
	out << cases;
	out << "\t\tdefault: return NONE; break;\n\t}\n\n";
	out << "}\n\n";


	out << "bool canBeConnected( int linkID, int from, int to ){\n"
		   "\tQMap< int, QHash< int, int > > edges;\n"
		   "\tQHash< int, int> hash;\n\n";

	for( int i=0; i<(int) objects.size(); i++){
		if( objects.at(i)->type == EDGE ){
			Edge* edge = (Edge*) objects.at(i);
			for( int j=0; j<(int) edge->associations.size(); j++ ){
				out << "\thash.clear();\n";
				int from = position(edge->associations.at(j)->from);
				int to = position(edge->associations.at(j)->to);
				if( from != -1)
					from += 12;
				 if( to != -1)
					to += 12;
				out << QString("\thash.insertMulti(%1, %2);\n").arg(from).arg(to);
				out << QString("\tedges.insert(%1, hash);\n\n").arg(i+NUM);
			}
		}
	}

	out << "\tif( from == ANY && to == ANY )\n"
		   "\t\treturn true; // (-1, -1)\n\n"
		   "\tQHash<int, int> h;\n"
		   "\tbool res;\n"
		   "\tint fromID = NONE;\n"
		   "\tint toID = NONE;\n"
		   "\th = edges.value(linkID);\n\n"
		   "\tif( h.keys().contains(from) )\n"
		   "\t\tfromID = from;\n"
		   "\telse\n"
		   "\t\tfromID = checkParents(h.keys(), from);\n\n"
		   "\tif( h.values().contains(to) )\n"
		   "\t\ttoID = to;\n"
		   "\telse\n"
		   "\t\ttoID = checkParents(h.values(), to);\n\n"
		   "\tif( fromID == ANY && toID != NONE)\n"
		   "\t\treturn h.values().contains(toID);\n\n"
		   "\tif( toID == ANY && fromID != NONE)\n"
		   "\t\treturn h.keys().contains(fromID);\n\n"
		   "\tres = h.values(fromID).contains(toID);\n\n"
		   "\treturn res;\n";

	out << "}\n";

	file.close();
}

Entity* Generator::find( QString id ){
	for ( int i=0; i < (int) objects.size(); i++ )
		if( objects.at(i)->id == id )
			return objects.at(i);
	return 0;
}

Edge* Generator::findEdge( QString id ){
	for ( int i=0; i < (int) edges.size(); i++ )
		if( edges.at(i)->id == id )
			return edges.at(i);
	return 0;
}

void Generator::propagateAll(){

	// propagating objects' properties
	for( int i=0; i < (int) objects.size(); i++ ){
		if( !objects.at(i)->propsPropagated )
			propagateProperties( objects.at(i) );
	}

	for( int i=0; i< (int) objects.size(); i++ ){
		if( !objects.at(i)->parentsPropagated )
			propagateParents(objects.at(i));
	}

	for( int i=0; i< (int) objects.size(); i++ ){
//        qDebug() << i + NUM << objects.at(i)->id << objects.at(i)->parents;
	}

	// is not needed right now
/*
	// propagating edges' stuff
	for( int i=0; i < (int) edges.size(); i++ ){
		if( !edges.at(i)->assocsPropagated )
			propagateAssocs( edges.at(i) );
	}
  */


}

void Generator::propagateParents( Entity* cur ){

	for( int j=0; j < cur->parents.size(); j++ ){
		Entity* par = find(cur->parents.at(j));
		if( !par ){
			cur->parentsPropagated = true;
			return;
		}
		cur->addParent(par->id);
		if ( !par->parentsPropagated )
			propagateParents( par );
		for( int i=0; i<par->parents.size(); i++)
			cur->addParent(par->parents.at(i));
	}
	cur->parentsPropagated = true;

}

/*
void Generator::findChildren(Entity* cur, QString id){
	for( int i=0; i<(int) cur->parents.size(); i++ ){
		Entity* par = find(cur->parents.at(i));
		par->addChild(cur->id);
		for( int ch=0; ch<cur->children.size(); ch++ )
			par->addChild(cur->children.at(ch));
		findParents(par, id);

	}
}
*/
void Generator::propagateProperties( Entity* cur ){

	for( int j=0; j < cur->parents.size(); j++ ){
		Entity* par = find(cur->parents.at(j));
		if( !par){
			cur->propsPropagated = true;
			return;
		}
		if ( !par->propsPropagated )
			propagateProperties( par );
		for( int k=0; k < par->properties.size(); k++ )
			cur->addProperty( par->properties.at(k).first, par->properties.at(k).second );
	}
	cur->propsPropagated = true;
}

void Generator::propagateAssocs( Edge* cur ){

	for( int j=0; j < cur->parents.size(); j++ ){
		Edge* par = findEdge(cur->parents.at(j));
		if( !par ){
			cur->assocsPropagated = true;
			return;
		}
		if ( !par->assocsPropagated )
			propagateAssocs( par );
		for( int k=0; k < par->associations.size(); k++ )
			cur->addAssociation( par->associations.at(k) );
	}
	cur->assocsPropagated = true;
}

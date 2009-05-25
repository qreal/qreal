#include "xmiHandler.h"
#include "realrepoclient.h"
#include "dbg.h"

#include <QtGui>

XmiHandler::XmiHandler (QString const &addr, const int port)
{
	client = new RealRepoClient(addr, port, 0);
}

void XmiHandler::exportToXmi(QString const &pathToFile)
{
	QFile file("test.xmi");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);

	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\n";
	out << "<xmi:XMI xmlns:xmi=\"http://schema.omg.org/spec/XMI/2.1\" xmlns:uml=\"http://schema.omg.org/spec/UML/2.0\" xmi:version=\"2.1\">" << "\n";

	out << "\n";
	out << "\n";

	//  --------------  header --------------- //
	out << "<xmi:Documentation exporter=\"QReal\" />" << "\n";

	out << "\n";
	out << "\n";

	//  --------------  end of header --------------- //

	//  --------------  content --------------- //
	out << "<uml:Package xmi:id=\"1\" xmi:uuid=\"1\" name=\"RootDiagram\">" << "\n";

	qRealTypes::RealObject root = client->getObjectById("repoRoot");
	qRealTypes::IdTypeList rootDiagrams = root.getChildElements();

	foreach (const QString typeDiagram, rootDiagrams) {
		out << this->serializeChildren(typeDiagram);
	}

	out << "</uml:Package>" << "\n";
	//  --------------  end of content  --------------- //

	out << "</xmi:XMI>" << "\n";
	qDebug() << "Done.";
}

QString XmiHandler::serializeChildren(QString const &idParent)
{
	QString result = "";
	qRealTypes::RealObject object = client->getObjectById(idParent);
	qRealTypes::IdTypeList childElems = object.getChildElements();

	if (childElems.size() > 0) {
		foreach (const QString id, childElems) {
			result = result + this->serializeObject(id);
		}
	}

	qRealTypes::IdTypeList linksOut = object.getOutcomingLinks();

	if (linksOut.size() > 0) {
		foreach (const QString id, linksOut) {
			result = result + this->serializeOutcomingLink(id);
		}
	}

	qRealTypes::IdTypeList linksIn = object.getIncomingLinks();

	if (linksIn.size() > 0) {
		foreach (const QString id, linksIn) {
			result = result + this->serializeIncomingLink(id);
		}
	}

	return result;
}

QString XmiHandler::serializeObject(QString const &id)
{
	QString result = "";
	qRealTypes::RealObject object = client->getObjectById(id);

	QString typeOfElem = "";
	QString typeOfTag = "";
	QString additionalParams = "";

	if (object.getTypeId() == "krnnDiagram") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Package";
	}

	// class diagramm

	else if (object.getTypeId() == "cnClass") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Class";
	} else if (object.getTypeId() == "cnClassView") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Package";
	}else if (object.getTypeId() == "cnClassMethod") {
		typeOfTag = "ownedOperation";
		typeOfElem = "uml:Operation";
	} else if (object.getTypeId() == "cnClassField") {
		typeOfTag = "ownedAttribute";
		typeOfElem = "uml:Property";
	}

	//use case diagram

	else if (object.getTypeId() == "uscnActor") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Actor";
	} else if (object.getTypeId() == "uscnUseCase") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:UseCase";
	}

	// sequence diagramm

	else if (object.getTypeId() == "sqnnInteraction") {
		typeOfTag = "ownedMember";
		typeOfElem = "uml:Interaction";
	} else if (object.getTypeId() == "sqnnSLifeline") {
		typeOfTag = "lifeline";
		typeOfElem = "uml:Lifeline";
	}

	// additional params
	if (object.getProperty("visibility").size() > 0) {
		QString visibility = object.getProperty("visibility");

		if (visibility == "public" || visibility == "private" || visibility == "protected" || visibility == "package") {
			additionalParams = additionalParams + "visibility=\"" + object.getProperty("visibility") + "\"";
		}
	}

	if (typeOfElem.size() != 0 && typeOfTag.size() != 0) {
		result = result + "<" + typeOfTag + " xmi:type=\"" + typeOfElem + "\" xmi:id=\""
			+ id + "\" xmi:uuid=\"" + id + "\" name=\"" + object.getName()
			+ "\" " + additionalParams + ">" + "\n";
		result = result + this->serializeChildren(id);
		result = result + "</" + typeOfTag + ">" + "\n";
		result = result + this->serializeLinkBodies(id);
	}
	return result;
}

QString XmiHandler::serializeOutcomingLink(QString const &id)
{
	QString result = "";
	qRealTypes::RealLink link = client->getLinkById(id);

	// kernel diagram
	if (link.getTypeId() == "krnePackageImport") {
		result =
			result + "<packageImport xmi:type=\"uml:PackageImport\" xmi:id=\""
			+ link.getId() + "\" xmi:uuid=\"" + link.getId()
			+ "\"  importedPackage=\"" + link.getToId() + "\" />";
	} else if (link.getTypeId() == "krneElementImport") {
		result =
			result + "<elementImport xmi:type=\"uml:ElementImport\" xmi:id=\""
			+ link.getId() + "\" xmi:uuid=\"" + link.getId()
			+ "\"  importedElement=\"" + link.getToId() + "\" />";
	} else if (link.getTypeId() == "krneGeneralization") {
		result =
			result + "<generalization xmi:type=\"uml:Generalization\" xmi:id=\""
			+ link.getId() + "\" xmi:uuid=\"" + link.getId()
			+ "\" general=\"" + link.getToId() +  "\"/>";
	} else if (link.getTypeId() == "krneDirRelationship") {
		result = result + "<ownedAttribute xmi:type=\"uml:Property\" xmi:id=\""
			+ "ToEnd" + id + "\" xmi:uuid=\"" + "ToEnd" + id
			+ "\" visibility=\"protected\" type=\"" + link.getToId() + "\">" + "\n";

		QString toMult = link.getProperty("toMultiplicity");
		if (toMult.size() > 0) {
			QPair<QString, QString> multValues = multiplicityValues(toMult);

			QString valueLover = multValues.first;
			QString valueUpper = multValues.second;

			if (valueLover.size() > 0 && valueUpper.size() > 0) {
				result = result + "<lowerValue xmi:type=\"uml:LiteralString\" xmi:id=\""
					+ "loverValueTo" + id + "\" xmi:uuid=\"" + "loverValueTo"
					+ id + "\" visibility=\"public\" value=\"" + valueLover + "\"/>" + "\n";
				result = result + "<upperValue xmi:type=\"uml:LiteralString\" xmi:id=\""
					+ "upperValueTo" + id + "\" xmi:uuid=\"" + "upperValueTo"
					+ id + "\" visibility=\"public\" value=\"" + valueUpper + "\"/>" + "\n";
			}
		}

		result = result + "<association xmi:idref=\"" + id +  "\"/>" + "\n";
		result = result + "</ownedAttribute>" + "\n";
	}

	// class diagram

	else if (link.getTypeId() == "ceDependency") {
		result = result +
			"<clientDependency xmi:idref=\"" + link.getId()  + "\"/>" + "\n";
	}

	// use case diagram

	else if (link.getTypeId() == "uscaExtend") {
		result = result + "<extend xmi:type=\"uml:Extend\" xmi:id=\"" + id
			+ "\" xmi:uuid=\"" + id + "\" extendedCase=\"" + link.getToId() + "\">" + "\n";
		result = result +  "<extension xmi:idref=\"" + link.getFromId() + "\"/>" + "\n";
		result = result + "</extend>" + "\n";
	} else if (link.getTypeId() == "uscaInclude"){
		result =
			result + "<include xmi:type=\"uml:Include\" xmi:id=\"" + id +
			"\" xmi:uuid=\"" + id + "\" addition=\"" + link.getToId() + "\"/>" + "\n";
	}

	return result;
}

QString XmiHandler::serializeIncomingLink(QString const &id)
{
	QString result = "";
	qRealTypes::RealLink link = client->getLinkById(id);

	if (link.getTypeId() == "ceDependency") {
		result = result +
				 "<supplierDependency xmi:idref=\"" + link.getId()  + "\"/>";
	}

	return result;
}

QString XmiHandler::serializeLink(QString const &id)
{
	QString result = "";
	qRealTypes::RealLink link = client->getLinkById(id);
	QString additionalParams = "";

	if (link.getProperty("visibility").size() > 0) {
		additionalParams = additionalParams + "visibility=\"" + link.getProperty("visibility") + "\"";
	}

	if (link.getTypeId() == "ceComposition" || link.getTypeId() == "ceAggregation"
		|| link.getTypeId() == "krneRelationship" || link.getTypeId() == "ceRelation"
		|| link.getTypeId() == "krneDirRelationship")
	{
		QString aggregation = "";

		if (link.getTypeId() == "ceComposition") {
			aggregation = "aggregation=\"composite\"";
		} else if (link.getTypeId() == "ceAggregation") {
			aggregation = "aggregation=\"shared\"";
		}

		result = result + "<ownedMember xmi:type=\"uml:Association\" xmi:id=\""
			+ id + "\" xmi:uuid=\"" + id + "\" " + additionalParams + ">" + "\n";

		// FromEnd

		result = result + "<memberEnd xmi:idref=\"" + "FromEnd" + id + "\"/>"  + "\n";
		result = result + "<ownedEnd xmi:type=\"uml:Property\" xmi:id=\""
			+ "FromEnd" + id + "\" xmi:uuid=\"" + "FromEnd" + id
			+ "\" visibility=\"protected\" type=\"" + link.getFromId() + "\">" + "\n";
		result = result + "<association xmi:idref=\"" + id +  "\"/>" + "\n";

		QString fromMult = link.getProperty("fromMultiplicity");
		if (fromMult.size() > 0) {
			QPair<QString, QString> multValues = multiplicityValues(fromMult);

			QString valueLover = multValues.first;
			QString valueUpper = multValues.second;

			if (valueLover.size() > 0 && valueUpper.size() > 0){
				result = result + "<lowerValue xmi:type=\"uml:LiteralString\" xmi:id=\""
					+ "loverValueFrom" + id + "\" xmi:uuid=\"" + "loverValueFrom"
					+ id + "\" visibility=\"public\" value=\"" + valueLover + "\"/>" + "\n";
				result = result + "<upperValue xmi:type=\"uml:LiteralString\" xmi:id=\""
					+ "upperValueFrom" + id + "\" xmi:uuid=\"" + "upperValueFrom"
					+ id + "\" visibility=\"public\" value=\"" + valueUpper + "\"/>" + "\n";
			}
		}

		result = result + "<association xmi:idref=\"" + id +  "\"/>" + "\n";
		result = result + "</ownedEnd>" + "\n";

		// ToEnd

		result = result + "<memberEnd xmi:idref=\"" + "ToEnd" + id + "\"/>"  + "\n";

		if (link.getTypeId() != "krneDirRelationship") {

			result = result + "<ownedEnd xmi:type=\"uml:Property\" xmi:id=\"" + "ToEnd"
				+ id + "\" xmi:uuid=\"" + "ToEnd" + id + "\" visibility=\"protected\" "
				+ aggregation + " type=\"" + link.getToId() + "\">" + "\n";

			QString toMult = link.getProperty("toMultiplicity");
			if (toMult.size() > 0){
				QPair<QString, QString> multValues = multiplicityValues(toMult);

				QString valueLover = multValues.first;
				QString valueUpper = multValues.second;

				if (valueLover.size() > 0 && valueUpper.size() > 0){
					result = result + "<lowerValue xmi:type=\"uml:LiteralString\" xmi:id=\""
						+ "loverValueTo" + id + "\" xmi:uuid=\"" + "loverValueTo"
						+ id + "\" visibility=\"public\" value=\"" + valueLover + "\"/>" + "\n";
					result = result + "<upperValue xmi:type=\"uml:LiteralString\" xmi:id=\""
						+ "upperValueTo" + id + "\" xmi:uuid=\"" + "upperValueTo"
						+ id + "\" visibility=\"public\" value=\"" + valueUpper + "\"/>" + "\n";
				}
			}

			result = result + "<association xmi:idref=\"" + id +  "\"/>" + "\n";
			result = result + "</ownedEnd>" + "\n";
		}

		result = result + "</ownedMember>" + "\n";
	} else if (link.getTypeId() == "ceDependency"){
		result = result + "<ownedMember xmi:type=\"uml:Dependency\" xmi:id=\""
			+ id + "\" xmi:uuid=\"" + id + "\" " + additionalParams + ">" + "\n";
		result = result + "<supplier xmi:idref=\"" + link.getToId() + "\"/>" + "\n";
		result = result + "<client xmi:idref=\"" + link.getFromId() + "\"/>" + "\n";
		result = result + "</ownedMember>" + "\n";
	}

	return result;
}

QString XmiHandler::serializeLinkBodies(QString const &id){
	QString result = "";
	qRealTypes::RealObject object = client->getObjectById(id);

	qRealTypes::IdTypeList links = object.getIncomingLinks();

	if (links.size() > 0){
		foreach (const QString id, links) {
			result = result + this->serializeLink(id);
		}
	}
	return result;
}

QPair<QString, QString> XmiHandler::multiplicityValues(QString const &multiplicity) const
{
	if (multiplicity == "1..*"){
		return QPair<QString, QString>("1", "*");
	} else if (multiplicity == "0..1") {
		return QPair<QString, QString>("0", "1");
	} else if (multiplicity == "1") {
		return QPair<QString, QString>("1", "1");
	} else if (multiplicity == "*") {
		return QPair<QString, QString>("*", "*");
	} else {
		return QPair<QString, QString>();
	}
}

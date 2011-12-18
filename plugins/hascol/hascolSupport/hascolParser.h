#pragma once

#include <QtCore/QString>
#include <QtXml/QDomElement>

#include "../../../qrkernel/ids.h"
#include "../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "../../../qrrepo/logicalRepoApi.h"

namespace hascol {
namespace support {

class HascolParser
{
public:
	HascolParser(qrRepo::LogicalRepoApi &api, qReal::ErrorReporterInterface &errorReporter);

	void parse(QStringList const &files);

private:
	qReal::Id initDiagram(QString const &diagramName, QString const &diagramType);
	qReal::Id addElement(qReal::Id const &parent, qReal::Id const &elementType, QString const &name);
	void preprocessFile(QString const &fileName);
	void parseFile(QString const &fileName);
	void parseProcess(QDomElement const &element);
	void parsePorts(QDomNodeList const &ports, QString const &direction
					, qReal::Id const &parentOnAPortMap, qReal::Id const &parentOnAStructure);
	void initClassifierFields(qReal::Id const &classifier);

	void doLayout(qReal::Id const &diagram, unsigned cellWidth, unsigned cellHeight);
	void doPortMappingLayout();
	void doStructureLayout();
	void doPlugsLayout(qReal::Id const &parent);
	void doPortsLayout(qReal::Id const &parent);
	void doLayoutForPortsType(qReal::Id const &parent, unsigned margin, QString const &direction, unsigned count);

	qReal::Id mImportedPortMappingDiagramId;
	qReal::Id mImportedStructureDiagramId;

	qrRepo::LogicalRepoApi &mApi;
	qReal::ErrorReporterInterface &mErrorReporter;
};

}
}

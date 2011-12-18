#pragma once

#include <QtCore/QString>

#include "../../../qrkernel/ids.h"
#include "../../../qrutils/outFile.h"
#include "../../../qrrepo/logicalRepoApi.h"
#include "../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace hascol {
namespace support {

class HascolGenerator
{
public:
	explicit HascolGenerator(qrRepo::LogicalRepoApi const &api, qReal::ErrorReporterInterface &errorReporter);

	void generate();

private:
	void generateDiagram(qReal::Id const &id);
	void generateProcess(qReal::Id const &id, utils::OutFile &out);
	void generateProcessTypeBody(qReal::Id const &id, utils::OutFile &out);
	void generateProcessOperation(qReal::Id const &id, utils::OutFile &out);
	void generateLetBinding(qReal::Id const &id, utils::OutFile &out);
	void generateResource(qReal::Id const &id, bool first, utils::OutFile &out);
	void generateActivity(qReal::Id const &id, utils::OutFile &out);
	void generateHandler(qReal::Id const &id, utils::OutFile &out);
	void generateFunctor(qReal::Id const &id, utils::OutFile &out);
	void generateFunctorFormalParameter(qReal::Id const &id, utils::OutFile &out);
	void generatePortMap(qReal::Id const &id, utils::OutFile &out);
	qReal::Id generateChain(qReal::Id const &startNode, utils::OutFile &out);
	void generateActivityNode(qReal::Id const &id, utils::OutFile &out);
	qReal::Id generateIf(qReal::Id const &id, utils::OutFile &out);

	qrRepo::LogicalRepoApi const &mApi;
	qReal::ErrorReporterInterface &mErrorReporter;

	qReal::IdList mPortMappingDiagrams;
	qReal::IdList mActivityDiagrams;
};

}
}

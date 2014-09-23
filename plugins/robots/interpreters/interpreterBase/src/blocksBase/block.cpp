#include <interpreterBase/blocksBase/block.h>

#include <thirdparty/qslog/QsLog.h>

using namespace interpreterBase;
using namespace blocksBase;
using namespace qReal;

Block::Block()
	: mNextBlockId(Id())
	, mGraphicalModelApi(nullptr)
	, mLogicalModelApi(nullptr)
	, mGraphicalId(Id())
	, mState(idle)
	, mErrorReporter(nullptr)
	, mRobotModelManager(nullptr)
{
	connect(this, &BlockInterface::done, this, &Block::finishedRunning);
}

void Block::init(Id const &graphicalId
		, GraphicalModelAssistInterface const &graphicalModelApi
		, LogicalModelAssistInterface const &logicalModelApi
		, ErrorReporterInterface * const errorReporter
		, robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguageToolbox)
{
	mGraphicalId = graphicalId;
	mGraphicalModelApi = &graphicalModelApi;
	mLogicalModelApi = &logicalModelApi;
	mErrorReporter = errorReporter;
	mRobotModelManager = &robotModelManager;
	mParser = &textLanguageToolbox;
}

bool Block::initNextBlocks()
{
	if (id().isNull() || id() == Id::rootId()) {
		error(tr("Control flow break detected, stopping"));
		return false;
	}

	IdList const links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	if (links.count() > 1) {
		error(tr("Too many outgoing links"));
		return false;
	}

	if (links.count() == 0) {
		error(tr("No outgoing links, please connect this block to something or use Final Node to end program"));
		return false;
	}

	if (links.count() == 1) {
		Id const nextBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(links[0], id());
		if (nextBlockId.isNull() || nextBlockId == Id::rootId()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		mNextBlockId = nextBlockId;
	}

	return true;
}

Id const Block::id() const
{
	return mGraphicalId;
}

void Block::interpret()
{
	// mState == running is not filtered out due to recursions and forks
	if (mState == failed) {
		return;
	}

	mState = running;
	if (initNextBlocks()) {
		run();
	}
}

void Block::setFailedStatus()
{
	mState = failed;
}

void Block::finishedRunning()
{
	mState = idle;
}

QVariant Block::property(QString const &propertyName) const
{
	return property(id(), propertyName);
}

QString Block::stringProperty(QString const &propertyName) const
{
	return stringProperty(id(), propertyName);
}

int Block::intProperty(QString const &propertyName) const
{
	return intProperty(id(), propertyName);
}

bool Block::boolProperty(QString const &propertyName) const
{
	return boolProperty(id(), propertyName);
}

QVariant Block::property(Id const &id, QString const &propertyName) const
{
	Id const logicalId = mGraphicalModelApi->logicalId(id);
	return mLogicalModelApi->propertyByRoleName(logicalId, propertyName);
}

QString Block::stringProperty(Id const &id, QString const &propertyName) const
{
	return property(id, propertyName).toString();
}

int Block::intProperty(Id const &id, QString const &propertyName) const
{
	return property(id, propertyName).toInt();
}

bool Block::boolProperty(Id const &id, QString const &propertyName) const
{
	return property(id, propertyName).toBool();
}

QColor Block::propertyToColor(QString const &property) const
{
	// Qt does not support dark-yellow string color (see QColor::colorNames())
	return property == "darkYellow" ? QColor(Qt::darkYellow) : QColor(property);
}

void Block::error(QString const &message)
{
	mErrorReporter->addError(message, id());
	emit failure();
}

void Block::warning(QString const &message)
{
	mErrorReporter->addWarning(message, id());
}

QMap<robotModel::PortInfo, robotModel::DeviceInfo> Block::usedDevices()
{
	return QMap<robotModel::PortInfo, robotModel::DeviceInfo>();
}

void Block::evalCode(QString const &code)
{
	evalCode<int>(code);
}

void Block::eval(QString const &propertyName)
{
	eval<int>(propertyName);
}

bool Block::errorsOccured() const
{
	return !mParser->errors().isEmpty();
}

interpreterBase::robotModel::RobotModelInterface &Block::model()
{
	return mRobotModelManager->model();
}

void Block::finishedSteppingInto()
{
}

void Block::reportParserErrors()
{
	for (qrtext::core::Error const &error : mParser->errors()) {
		switch (error.severity()) {
		case qrtext::core::Severity::critical:
		case qrtext::core::Severity::error:
			mErrorReporter->addError((QString("%1:%2 %3")
					.arg(error.connection().line())
					.arg(error.connection().column())
					.arg(error.errorMessage()))
					, id());

			break;
		case qrtext::core::Severity::warning:
			mErrorReporter->addWarning(QString("%1:%2 %3")
					.arg(error.connection().line())
					.arg(error.connection().column())
					.arg(error.errorMessage())
					, id());

			break;
		case qrtext::core::Severity::internalError:
			break;
		}
	}
}

#include <qrutils/interpreter/block.h>

#include <thirdparty/qslog/QsLog.h>
#include <qrtext/languageToolboxInterface.h>

using namespace qReal;
using namespace interpretation;

Block::Block()
	: mNextBlockId(Id())
	, mGraphicalModelApi(nullptr)
	, mLogicalModelApi(nullptr)
	, mGraphicalId(Id())
	, mState(idle)
	, mErrorReporter(nullptr)
{
	connect(this, &BlockInterface::done, this, &Block::finishedRunning);
}

void Block::init(const Id &graphicalId
		, const GraphicalModelAssistInterface &graphicalModelApi
		, const LogicalModelAssistInterface &logicalModelApi
		, ErrorReporterInterface * const errorReporter
		, qrtext::LanguageToolboxInterface &textLanguageToolbox)
{
	mGraphicalId = graphicalId;
	mGraphicalModelApi = &graphicalModelApi;
	mLogicalModelApi = &logicalModelApi;
	mErrorReporter = errorReporter;
	mParser = &textLanguageToolbox;
	if (mLogicalModelApi) {
		mParserErrorReporter.reset(new utils::ParserErrorReporter(*mParser, *mErrorReporter
				, mLogicalModelApi->editorManagerInterface()));
	}
}

bool Block::initNextBlocks()
{
	if (id().isNull() || id() == Id::rootId()) {
		error(tr("Control flow break detected, stopping"));
		return false;
	}

	const IdList links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	if (links.count() > 1) {
		error(tr("Too many outgoing links"));
		return false;
	}

	if (links.count() == 0) {
		error(tr("No outgoing links, please connect this block to something or use Final Node to end program"));
		return false;
	}

	if (links.count() == 1) {
		const Id nextBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(links[0], id());
		if (nextBlockId.isNull() || nextBlockId == Id::rootId()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		mNextBlockId = nextBlockId;
	}

	return true;
}

const Id Block::id() const
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

QVariant Block::property(const QString &propertyName) const
{
	return property(id(), propertyName);
}

QString Block::stringProperty(const QString &propertyName) const
{
	return stringProperty(id(), propertyName);
}

int Block::intProperty(const QString &propertyName) const
{
	return intProperty(id(), propertyName);
}

bool Block::boolProperty(const QString &propertyName) const
{
	return boolProperty(id(), propertyName);
}

QVariant Block::property(const Id &id, const QString &propertyName) const
{
	const Id logicalId = mGraphicalModelApi->logicalId(id);
	return mLogicalModelApi->propertyByRoleName(logicalId, propertyName);
}

QString Block::stringProperty(const Id &id, const QString &propertyName) const
{
	return property(id, propertyName).toString();
}

int Block::intProperty(const Id &id, const QString &propertyName) const
{
	return property(id, propertyName).toInt();
}

bool Block::boolProperty(const Id &id, const QString &propertyName) const
{
	return property(id, propertyName).toBool();
}

QColor Block::propertyToColor(const QString &property) const
{
	// Qt does not support dark-yellow string color (see QColor::colorNames())
	return property == "darkYellow" ? QColor(Qt::darkYellow) : QColor(property);
}

void Block::error(const QString &message)
{
	mErrorReporter->addError(message, id());
	emit failure();
}

void Block::warning(const QString &message)
{
	mErrorReporter->addWarning(message, id());
}

void Block::evalCode(const QString &code)
{
	evalCode<int>(code);
}

void Block::eval(const QString &propertyName)
{
	eval<int>(propertyName);
}

bool Block::errorsOccured() const
{
	return !mParser->errors().isEmpty();
}

void Block::finishedSteppingInto()
{
}

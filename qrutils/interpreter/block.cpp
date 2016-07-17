/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include <qrutils/interpreter/block.h>

#include <thirdparty/qslog/QsLog.h>
#include <qrtext/languageToolboxInterface.h>
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

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

	if (!mGraphicalModelApi->graphicalRepoApi().exist(id())) {
		error(tr("Block has disappeared!"));
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

void Block::interpret(Thread *thread)
{
	// mState == running is not filtered out due to recursions and forks
	if (mState == failed) {
		return;
	}

	mState = running;
	mThread = thread;
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

QVariant Block::property(const QString &propertyName)
{
	return property(id(), propertyName);
}

QString Block::stringProperty(const QString &propertyName)
{
	return stringProperty(id(), propertyName);
}

int Block::intProperty(const QString &propertyName)
{
	return intProperty(id(), propertyName);
}

bool Block::boolProperty(const QString &propertyName)
{
	return boolProperty(id(), propertyName);
}

QVariant Block::property(const Id &id, const QString &propertyName)
{
	const Id logicalId = mGraphicalModelApi->logicalId(id);
	if (logicalId.isNull()) {
		// If we get here we definitely have such situation:
		// graphical id existed when this Block instance was constructed (or we just will not get here),
		// but now the logical instance has suddenly disppeared.
		error(tr("Block has disappeared!"));
		return QVariant();
	}

	return mLogicalModelApi->propertyByRoleName(logicalId, propertyName);
}

QString Block::stringProperty(const Id &id, const QString &propertyName)
{
	return property(id, propertyName).toString();
}

int Block::intProperty(const Id &id, const QString &propertyName)
{
	return property(id, propertyName).toInt();
}

bool Block::boolProperty(const Id &id, const QString &propertyName)
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

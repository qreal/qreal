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

#pragma once

#include <qrkernel/ids.h>
#include <qrkernel/definitions.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "qrgui/systemFacade/systemFacadeDeclSpec.h"

namespace qReal {

class QRGUI_SYSTEM_FACADE_EXPORT NullErrorReporter : public ErrorReporterInterface
{
public:
	void addInformation(const QString &message, const Id &position = Id::rootId()) override;
	void addWarning(const QString &message, const Id &position = Id::rootId()) override;
	void addError(const QString &message, const Id &position = Id::rootId()) override;
	void addCritical(const QString &message, const Id &position = Id::rootId()) override;

	void sendBubblingMessage(const QString &message, int duration, QWidget *parent = 0) override;

	bool wereErrors() override;
	void clear() override;
	void clearErrors() override;

private:
	bool mWereErrors = false;
};

}

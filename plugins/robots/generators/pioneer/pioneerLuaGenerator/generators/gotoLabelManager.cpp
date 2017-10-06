/* Copyright 2017 QReal Research Group
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

#include "gotoLabelManager.h"

#include <generatorBase/semanticTree/semanticNode.h>
#include <generatorBase/semanticTree/simpleNode.h>

using namespace pioneer::lua;

QString GotoLabelManager::labelFor(const qReal::Id &id)
{
	qReal::Id actualId = id;
	if (actualId.editor().startsWith("label_")) {
		actualId = qReal::Id(
				actualId.editor().mid(QString("label_").length())
				, actualId.diagram()
				, actualId.element()
				, actualId.id());
	}

	if (mLabels.contains(actualId)) {
		return mLabels.value(actualId);
	}

	const auto type = actualId.type();
	if (mNodeTypesCount.contains(type)) {
		++mNodeTypesCount[type];
	} else {
		mNodeTypesCount.insert(type, 1);
	}

	const auto label = beautify(QString("%1_%2").arg(actualId.element()).arg(mNodeTypesCount.value(type)));
	mLabels.insert(actualId, label);
	return label;
}

void GotoLabelManager::reinit()
{
	mLabels.clear();
	mNodeTypesCount.clear();
}

QString GotoLabelManager::beautify(const QString &label)
{
	QString result;
	for (QChar ch : label) {
		if (ch.isLower()) {
			result += ch.toUpper();
		} else if (ch.isUpper()) {
			result += '_' + ch;
		} else {
			result += ch;
		}
	}

	return result;
}

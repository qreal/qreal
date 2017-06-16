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

#include "receiveMailGenerator.h"
#include "generatorBase/generatorCustomizer.h"
#include "ev3GeneratorBase/ev3GeneratorFactory.h"

using namespace ev3::simple;
using namespace generatorBase::simple;
using namespace qReal;

const QMap<QString, QString> DEFAULT_VALUE =  {{"int", "0"}, {"float", "0.0"}, {"bool", "true"}, {"string", ""}};

ReceiveMailGenerator::ReceiveMailGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "", QList<Binding *>(), parent)
{
	auto mGeneratorFactory = dynamic_cast<Ev3GeneratorFactory *>(parent);
	const QString mailboxName = mRepo.property(mId, "MailBoxName").toString();
	const QString type = mRepo.property(mId, "MsgType").toString();
	const QString variable = customizer.factory()->nameNormalizerConverter()->convert(mRepo.property(mId
			 , "Variable").toString());
	if (!mGeneratorFactory->mailboxes().tryRegisterReadMailbox(mailboxName, type)) {
		mGeneratorFactory->reportError(tr("There is already mailbox with same name, but different msg type") , mId);
	}

	bool synch = mRepo.property(mId, "Synchronized").toBool();
	if (synch) {
		setPathToTemplate("mailboxes/readMailSynchronous.t");
	} else {
		setPathToTemplate("mailboxes/readMailAsynchronous.t");
	}

	// small trick to provide info about variable to system
	customizer.factory()->functionBlockConverter(id, "Variable")->convert(
			QString("%1 = %2").arg(variable).arg(DEFAULT_VALUE[type]));

	addBinding(Binding::createStatic("@@ID@@"
			, mGeneratorFactory->mailboxes().mailboxNameToId(mailboxName)));
	addBinding(Binding::createStatic("@@TYPE_LENGHT@@"
			, mGeneratorFactory->mailboxes().mailboxNameToTypeLength(mailboxName)));
	addBinding(Binding::createStatic("@@DATA_TYPE@@"
			, mGeneratorFactory->mailboxes().typePostfixDeclaration(type)));
	addBinding(Binding::createStatic("@@VARIABLE@@", variable));
}

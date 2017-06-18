/* Copyright 2017 CyberTech Labs Ltd.
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

#include "sendMailGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace ev3::simple;
using namespace generatorBase::simple;
using namespace qReal;

SendMailGenerator::SendMailGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "mailboxes/writeMail.t", QList<Binding *>(), parent)
{
	auto mGeneratorFactory = dynamic_cast<Ev3GeneratorFactory *>(parent);
	Binding::ConverterInterface *nameNormalizer = customizer.factory()->nameNormalizerConverter();
	const QString receiverName =mRepo.property(mId, "ReceiverName").toString();
	const QString mailboxName = nameNormalizer->convert(mRepo.property(mId, "ReceiverMailBoxName").toString());
	const QString type = mRepo.property(mId, "MsgType").toString();
	if (!mGeneratorFactory->mailboxes().tryRegisterWriteMailbox(mailboxName, type)) {
		mGeneratorFactory->reportError(tr("There is already mailbox with same name, but different msg type") , mId);
	}

	addBinding(Binding::createStatic("@@RECEIVER_NAME@@", receiverName));
	addBinding(Binding::createStatic("@@MAILBOX_NAME@@", mailboxName));
	addBinding(Binding::createStatic("@@TYPE@@", mGeneratorFactory->mailboxes().typeToEv3Type(type)));
	const Binding::ConverterInterface *valueConverter = nullptr;

	if (type == "int") {
		valueConverter = customizer.factory()->intPropertyConverter(mId, "Message");
	} else if (type == "bool") {
		valueConverter = customizer.factory()->boolPropertyConverter(mId, "Message", false);
	} else if (type == "string") {
		valueConverter = customizer.factory()->stringPropertyConverter(mId, "Message");
	} else { //float
		valueConverter = customizer.factory()->floatPropertyConverter(mId, "Message");
	}

	addBinding(Binding::createConverting("@@VALUE@@", "Message", valueConverter));
}

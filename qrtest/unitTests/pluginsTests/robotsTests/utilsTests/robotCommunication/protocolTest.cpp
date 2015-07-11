/* Copyright 2007-2015 QReal Research Group, Yurii Litvinov
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

#include "protocolTest.h"

#include <utils/robotCommunication/protocol.h>

using namespace qrTest::robotsTests::utilsTests;
using namespace utils::robotCommunication;

class CommunicatorInterface;
class ProtocolInterface;
class ErrorReporterInterface;

TEST_F(ProtocolTest, sanityTest)
{
	/*
	Protocol p;
	p << "version";
	p >> [](const QByteArray &data) { return true; };
	p.timeout(3000);
	p << "request config";
	p >> [](const QByteArray &data) { return true; };
	p.timeout(3000);

	p.run();
	p.waitForFinished();
	*/

	const QString versionMarker("version: ");
	const QString requiredVersion = "3.0.2";
	ErrorReporterInterface errorReporter;

	CommunicatorInterface tcpRobotCommunicator("127.0.0.1", 9000);

	ProtocolInterface versionProtocol = send("version")
			& receive(3000
				, [&](const QByteArray &data)
				{
					const QString currentVersion = data.mid(versionMarker.length());
					if (currentVersion != requiredVersion) {
						errorReporter->addError(tr("TRIK runtime version is too old, please update it by pressing "
								"'Upload Runtime' button on toolbar"));
					}
				}
				,
	);

	ProtocolInterface configurationProtocol = send("request config") & receive(3000, [](const QByteArray &data) { return true; });

	ProtocolInterface protocol = versionProtocol & configurationProtocol;

	protocol.setCommunicator(tcpRobotCommunicator);

	protocol.run();

	protocol.waitForFinished();



/*
	Protocol protocol;

	protocol.connect(

	[](){
		if (mSocket.state() == QTcpSocket::ConnectedState || mSocket.state() == QTcpSocket::ConnectingState) {
			return true;
		}

		mSocket.connectToHost(serverAddress, static_cast<quint16>(mPort));
		const bool result = mSocket.waitForConnected(3000);
		if (!result) {
			QLOG_ERROR() << mSocket.errorString();
		}

		mBuffer.clear();
		mExpectedBytes = 0;

		return result;
	}

	);

	protocol.send(

	[]() { mControlConnection->send("version");}

	, 3000

	);

	protocol.receive(

	[]() {
		const QString versionMarker("version: ");
		if (message.startsWith(versionMarker) && mErrorReporter) {
			mVersionTimer.stop();
			const QString currentVersion = message.mid(versionMarker.length());
			if (currentVersion != requiredVersion) {
				mErrorReporter->addError(tr("TRIK runtime version is too old, please update it by pressing "
						"'Upload Runtime' button on toolbar"));
			}
		}
	}

	);
	*/
}

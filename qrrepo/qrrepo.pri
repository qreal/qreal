# Copyright 2007-2015 QReal Research Group
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

links(qrkernel qrutils)

HEADERS += \
	$$PWD/private/repository.h \
	$$PWD/private/folderCompressor.h \
	$$PWD/private/qrRepoGlobal.h \
	$$PWD/private/serializer.h \
	$$PWD/private/singleXmlSerializer.h \
	$$PWD/private/valuesSerializer.h \
	$$PWD/private/classes/object.h \
	$$PWD/private/classes/logicalObject.h \
	$$PWD/private/classes/graphicalObject.h \
	$$PWD/private/classes/graphicalPart.h \

SOURCES += \
	$$PWD/private/repository.cpp \
	$$PWD/private/folderCompressor.cpp \
	$$PWD/private/repoApi.cpp \
	$$PWD/private/serializer.cpp \
	$$PWD/private/singleXmlSerializer.cpp \
	$$PWD/private/valuesSerializer.cpp \
	$$PWD/private/classes/object.cpp \
	$$PWD/private/classes/logicalObject.cpp \
	$$PWD/private/classes/graphicalObject.cpp \
	$$PWD/private/classes/graphicalPart.cpp \

# repo API
HEADERS += \
	$$PWD/repoApi.h \
	$$PWD/graphicalRepoApi.h \
	$$PWD/logicalRepoApi.h \
	$$PWD/repoControlInterface.h \
	$$PWD/commonRepoApi.h \
	$$PWD/exceptions/corruptSavefileException.h \
	$$PWD/exceptions/couldNotCreateDestinationFolderException.h \
	$$PWD/exceptions/couldNotCreateOutFileException.h \
	$$PWD/exceptions/qrrepoException.h \
	$$PWD/exceptions/saveFileNotFoundException.h \
	$$PWD/exceptions/saveFileNotReadableException.h \
	$$PWD/exceptions/sourceFolderNotFoundException.h \
	$$PWD/exceptions/couldNotOpenDestinationFileException.h \
	$$PWD/exceptions/couldNotOpenInputFileException.h \

DEFINES += QRREPO_LIBRARY

QT += xml

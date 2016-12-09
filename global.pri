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

win32 {
	PLATFORM = windows
}

unix:!macx {
	PLATFORM = linux
}

macx {
	PLATFORM = mac
}

CONFIG(debug, debug | release) {
	CONFIGURATION = debug
	CONFIGURATION_SUFFIX = -d
	unix {
		QMAKE_CXXFLAGS += -coverage
		QMAKE_LFLAGS += -coverage
	}
} else {
	CONFIGURATION = release
	CONFIGURATION_SUFFIX =
}

DESTDIR = $$PWD/bin/$$CONFIGURATION

PROJECT_BASENAME = $$basename(_PRO_FILE_)
PROJECT_NAME = $$section(PROJECT_BASENAME, ".", 0, 0)

isEmpty(TARGET) {
	TARGET = $$PROJECT_NAME$$CONFIGURATION_SUFFIX
} else {
	TARGET = $$TARGET$$CONFIGURATION_SUFFIX
}

equals(TEMPLATE, app) {
	unix:!macx {
		QMAKE_LFLAGS += -Wl,-rpath-link,$$DESTDIR
		!CONFIG(no_rpath) QMAKE_LFLAGS += -Wl,-O1,-rpath,.
	}
}

macx-clang {
	QMAKE_CXXFLAGS += -stdlib=libc++
	QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/../../../
}

unix {

	# seems like we want USan always, but are afraid of ....
	!CONFIG(sanitize_address):!CONFIG(sanitize_thread):!CONFIG(sanitize_memory):!CONFIG(sanitize_kernel_address) {
		# Ubsan is turned on by default into debug build
		CONFIG += sanitizer sanitize_undefined
		macx-clang {
			# sometimes runtime is missing in clang. this hack allows to avoid runtime dependency.
			QMAKE_SANITIZE_UNDEFINED_CFLAGS += -fsanitize-trap=undefined
			QMAKE_SANITIZE_UNDEFINED_CXXFLAGS += -fsanitize-trap=undefined
			QMAKE_SANITIZE_UNDEFINED_LFLAGS += -fsanitize-trap=undefined
		}
	}

	!CONFIG(sanitize_address):!macx-clang { CONFIG += sanitize_leak }

	CONFIG(sanitize_leak) {
		#LSan can be used without performance degrade even in release build
		QMAKE_CFLAGS += -fsanitize=leak
		QMAKE_CXXFLAGS += -fsanitize=leak
		QMAKE_LFLAGS += -fsanitize=leak
	}


	!clang:gcc:*-g++*:system($$QMAKE_CXX --version | grep -oe \'\\<5\\.[0-9]\\+\\.\' ){
		CONFIG(sanitize_undefined){
		# Ubsan has (had at least) known issues with false errors about calls of methods of the base class.
		# That must be disabled. Variables for confguring ubsan are taken from here:
		# https://codereview.qt-project.org/#/c/43420/17/mkspecs/common/sanitize.conf
		# They can change in some version of Qt, keep track of it.
		# By the way, simply setting QMAKE_CFLAGS, QMAKE_CXXFLAGS and QMAKE_LFLAGS instead of those used below
		# will not work due to arguments order ("-fsanitize=undefined" must be declared before "-fno-sanitize=vptr").
			QMAKE_SANITIZE_UNDEFINED_CFLAGS += -fno-sanitize=vptr
			QMAKE_SANITIZE_UNDEFINED_CXXFLAGS += -fno-sanitize=vptr
			QMAKE_SANITIZE_UNDEFINED_LFLAGS += -fno-sanitize=vptr
		}
	}

	CONFIG(release, debug | release){
		!clang:gcc:*-g++*:system($$QMAKE_CXX --version | grep -oe \'\\<4\\.[0-9]\\+\\.\' ){
			message("Too old compiler: $$QMAKE_CXX")
		} else {
			QMAKE_CFLAGS += -fsanitize-recover=all
			QMAKE_CXXFLAGS += -fsanitize-recover=all
		}
	}

}

OBJECTS_DIR = .build/$$CONFIGURATION/obj
MOC_DIR = .build/$$CONFIGURATION/moc
RCC_DIR = .build/$$CONFIGURATION/rcc
UI_DIR = .build/$$CONFIGURATION/ui

INCLUDEPATH += $$_PRO_FILE_PWD_ \
	$$_PRO_FILE_PWD_/include \
	$$PWD \

LIBS += -L$$DESTDIR

CONFIG += c++11
QMAKE_CXXFLAGS += -Wextra -Wcast-qual -Wwrite-strings -Wredundant-decls -Wunreachable-code -Wnon-virtual-dtor

GLOBAL_PWD = $$PWD

# Useful function to copy additional files to destination,
# from http://stackoverflow.com/questions/3984104/qmake-how-to-copy-a-file-to-the-output
defineTest(copyToDestdir) {
	FILES = $$1
	NOW = $$2

	for(FILE, FILES) {
		DESTDIR_SUFFIX =
		isEmpty(QMAKE_SH) {
		# This ugly code is needed because xcopy requires to add source directory name to target directory name when copying directories
			win32 {
				AFTER_SLASH = $$section(FILE, "/", -1, -1)
				BASE_NAME = $$section(FILE, "/", -2, -2)
				equals(AFTER_SLASH, ""):DESTDIR_SUFFIX = /$$BASE_NAME

				FILE ~= s,/$,,g

				FILE ~= s,/,\\,g
			}
			DDIR = $$DESTDIR$$DESTDIR_SUFFIX/$$3
			win32:DDIR ~= s,/,\\,g
		} else {
			DDIR = $$DESTDIR$$DESTDIR_SUFFIX/$$3
		}

		isEmpty(NOW) {
			# In case this is directory add "*" to copy contents of a directory instead of directory itself under linux.
			!win32:equals(AFTER_SLASH, ""):FILE = $$FILE*
			QMAKE_POST_LINK += $(COPY_DIR) $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
		} else {
			win32 {
				system("cmd /C "xcopy $$quote($$FILE) $$quote($$DDIR) /s /e /q /y /i"")
			}

			unix:!macx {
				system("cp -r -f $$FILE $$DDIR")
			}

			macx {
				system("cp -af $$FILE $$DDIR/")
			}
		}
	}

	export(QMAKE_POST_LINK)
}

defineTest(includes) {
	PROJECTS = $$1

	for(PROJECT, PROJECTS) {
		INCLUDEPATH += $$GLOBAL_PWD/$$PROJECT $$GLOBAL_PWD/$$PROJECT/include
	}

	export(INCLUDEPATH)
}

defineTest(links) {
	PROJECTS = $$1

	for(PROJECT, PROJECTS) {
		LIBS += -l$$PROJECT$$CONFIGURATION_SUFFIX
	}

	export(LIBS)
}

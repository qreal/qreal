# Copyright 2016 Iakov Kirilenko, 2007-2017 QReal Research Group
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#	  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

!isEmpty(CONFIG):isEmpty(GLOBAL_PRI_INCLUDED){
GLOBAL_PRI_INCLUDED = $$PWD

CONFIG+=qt

CONFIG(no-sanitizers):!CONFIG(nosanitizers): CONFIG += nosanitizers

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

DESTDIR = $$absolute_path(bin/$$CONFIGURATION)

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
	macx:!CONFIG(no_rpath) {
		QMAKE_LFLAGS += -rpath . -rpath @executable_path/../Lib -rpath @executable_path/../Frameworks -rpath @executable_path/../../../
	}
}

macx-clang {
	QMAKE_MACOSX_DEPLOYMENT_TARGET=10.9
	QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/
}

!gcc4:!gcc5:!clang:!win32:gcc:*-g++*:system($$QMAKE_CXX --version | grep -qEe '"\<5\.[0-9]+\."' ){ CONFIG += gcc5 }
!gcc4:!gcc5:!clang:!win32:gcc:*-g++*:system($$QMAKE_CXX --version | grep -qEe '"\<4\.[0-9]+\."' ){ CONFIG += gcc4 }


!CONFIG(nosanitizers):!clang:gcc:*-g++*:gcc4{
	warning("Disabled sanitizers, failed to detect compiler version or too old compiler: $$QMAKE_CXX")
	CONFIG += nosanitizers
}

unix:!CONFIG(nosanitizers) {

	# seems like we want USan always, but are afraid of ....
	!CONFIG(sanitize_address):!CONFIG(sanitize_thread):!CONFIG(sanitize_memory):!CONFIG(sanitize_kernel_address) {
		# Ubsan is turned on by default
		CONFIG += sanitizer sanitize_undefined
	}

	CONFIG(debug, debug | release):!CONFIG(sanitize_address):!macx-clang { CONFIG += sanitize_leak }

	CONFIG(sanitize_leak) {
		#LSan can be used without performance degrade even in release build
		QMAKE_CFLAGS += -fsanitize=leak
		QMAKE_CXXFLAGS += -fsanitize=leak
		QMAKE_LFLAGS += -fsanitize=leak
	}

	CONFIG(sanitize_undefined):macx-clang {
		# sometimes runtime is missing in clang. this hack allows to avoid runtime dependency.
		QMAKE_SANITIZE_UNDEFINED_CFLAGS += -fsanitize-trap=undefined
		QMAKE_SANITIZE_UNDEFINED_CXXFLAGS += -fsanitize-trap=undefined
		QMAKE_SANITIZE_UNDEFINED_LFLAGS += -fsanitize-trap=undefined
	}


	CONFIG(gcc5){
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
		CONFIG(gcc4) {
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

QMAKE_CXXFLAGS += -pedantic-errors -ansi -std=c++11 -Wextra

CONFIG(gcc5)|clang{
	QMAKE_CXXFLAGS +=-Werror=pedantic -Werror=delete-incomplete
}

QMAKE_CXXFLAGS += -Werror=cast-qual -Werror=write-strings -Werror=redundant-decls -Werror=unreachable-code \
			-Werror=non-virtual-dtor -Wno-error=overloaded-virtual \
			-Werror=uninitialized -Werror=init-self


# I want -Werror to be turned on, but Qt has problems
#QMAKE_CXXFLAGS += -Werror -Wno-error=inconsistent-missing-override -Wno-error=deprecated-declarations -Wno-error=unused-parameter

GLOBAL_PWD = $$absolute_path($$PWD)

# Simple function that checks if given argument is a file or directory.
# Returns false if argument 1 is a file or does not exist.
defineTest(isDir) {
	exists($$1/*):return(true)
	return(false)
}

# Useful function to copy additional files to destination,
# from http://stackoverflow.com/questions/3984104/qmake-how-to-copy-a-file-to-the-output
defineTest(copyToDestdir) {
	FILES = $$1
	NOW = $$2

	for(FILE, FILES) {
		DESTDIR_SUFFIX =
		AFTER_SLASH = $$section(FILE, "/", -1, -1)
		# This ugly code is needed because xcopy requires to add source directory name to target directory name when copying directories
		win32 {
			FILE = $$system_path($$FILE)
			isDir($$FILE) {
				ABSOLUTE_PATH = $$absolute_path($$FILE, $$GLOBAL_PWD)
				BASE_NAME = $$section(ABSOLUTE_PATH, "/", -1, -1)
				DESTDIR_SUFFIX = /$$BASE_NAME
			}
		}

		DDIR = $$DESTDIR/$$3$$DESTDIR_SUFFIX
		#win32:DDIR ~= s,/,\\,g ??? why not system_path?
		DDIR = $$system_path($$DDIR)
		mkpath($$DDIR)

		isEmpty(NOW) {
			# In case this is directory add "*" to copy contents of a directory instead of directory itself under linux.
			!win32:equals(AFTER_SLASH, ""):FILE = $$FILE* #looks like inconsistent behaviour
			win32:equals(AFTER_SLASH, "*"):FILE = $$section(FILE, "*", 0, -2)\\\*
			win32:QMAKE_POST_LINK += $$quote("xcopy /s /e /q /y /i") $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
			!win32:QMAKE_POST_LINK += $(COPY_DIR) $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
		} else {
			win32 {
				# Message here is very useful in diagnostics.
				message("Executing cmd /C \\\"xcopy $$quote($$FILE) $$quote($$DDIR) /s /e /q /y /i\\\"")
				system("cmd /C \\\"xcopy $$quote($$FILE) $$quote($$DDIR) /s /e /q /y /i\\\"")
			}

			unix:!macx {
				system("cp -r -f $$FILE $$DDIR/")
			}

			macx {
				system("rsync -avz $$FILE $$DDIR/")
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
message($$CONFIG)
} # GLOBAL_PRI_INCLUDED

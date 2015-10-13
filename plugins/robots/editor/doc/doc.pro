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

TEMPLATE = subdirs

include(../../../../global.pri)

copyToDestdir(help/ changelog.txt, now)

OTHER_FILES += \
	changelog.txt \
	help/contents.html.js \
	help/ev3.html \
	help/footer.html.js \
	help/header.html.js \
	help/index.html \
	help/interface.html \
	help/nxt.html \
	help/programming.html \
	help/style.css \
	help/trik.html \
	help/version.js \

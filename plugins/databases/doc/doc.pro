# Copyright 2016 Anastasia Semenova
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

include(../../../global.pri)

copyToDestdir(help/ changelog.txt, now)

OTHER_FILES += \
	changelog.txt \
	help/index.html \
	help/interface.html \
	help/lang.html \
	help/modelling.html \
	help/operations.html \
	help/style.css \
	help/version.js \

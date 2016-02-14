# Copyright 2015 Yurii Litvinov, QReal Research Group
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

copyToDestdir(deployment-scripts/, now)

OTHER_FILES += \
	$$PWD/deployment-scripts/uploading/update_site_scripts.sh \
	$$PWD/deployment-scripts/uploading/upload_to_google_drive.sh \
	$$PWD/deployment-scripts/uploading/upload_to_google_site.py \
	$$PWD/version_increment/generate_changelog.sh \
	$$PWD/version_increment/increment_trik_studio.sh \
	$$PWD/version_increment/replace.sh \
	$$PWD/virtualbox/build.py \
	$$PWD/virtualbox/build.sh \
	$$PWD/virtualbox/possible_machines.txt \
	$$PWD/virtualbox/guest/ubuntu_32/build.sh \
	$$PWD/virtualbox/guest/ubuntu_32/onBooted.sh \
	$$PWD/virtualbox/guest/ubuntu_32/sequence.txt \
	$$PWD/virtualbox/guest/ubuntu_64/build.sh \
	$$PWD/virtualbox/guest/ubuntu_64/onBooted.sh \
	$$PWD/virtualbox/guest/ubuntu_64/sequence.txt \
	$$PWD/virtualbox/guest/windows/build.sh \
	$$PWD/virtualbox/guest/windows/onBooted.bat \
	$$PWD/virtualbox/guest/windows/sequence.txt \

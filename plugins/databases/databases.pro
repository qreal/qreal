# Copyright 2014-2015 Anastasia Semenova
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

SUBDIRS = \
	databasesSupport/databasesSupport.pro \
	databasesEditor/databasesEditor.pro \
	databasesPhysicalModelMetamodel/sqlServer2008/sqlServer2008.pro \
	databasesPhysicalModelMetamodel/mySql5/mySql5.pro \
	databasesPhysicalModelMetamodel/sqlite/sqlite.pro \

/* Copyright 2015 QReal Research Group
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

// For autocompletion and syntax highlighting
var mainWindow, ui, utils, keyboard, palette, hints, scene, cursor;

var menuHelp = ui.getMenu("menuHelp");
assert(menuHelp != null);

utils.activateMenu(menuHelp);
api.wait(200);

var actionHelp = ui.getActionInMenu(menuHelp, "actionHelp");
checkAction(actionHelp, true, false, false);
expect(!ui.isSubMenuInMenu(menuHelp, actionHelp));

var actionAbout = ui.getActionInMenu(menuHelp, "actionAbout");
checkAction(actionAbout, true, false, false);
expect(!ui.isSubMenuInMenu(menuHelp, actionAbout));

var actionAboutQt = ui.getActionInMenu(menuHelp, "actionAboutQt");
checkAction(actionAboutQt, true, false, false);
expect(!ui.isSubMenuInMenu(menuHelp, actionAboutQt));

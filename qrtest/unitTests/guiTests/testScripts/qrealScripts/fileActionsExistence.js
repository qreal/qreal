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

var menuFile = ui.findMenu("menu_File");
assert(menuFile != null);

utils.activateMenu(menuFile);
api.wait(200);

function expectAction(actionName, enabled) {
    var action = ui.findActionInMenu(menuFile, actionName);
    checkAction(action, enabled, false, false);
    expect(!ui.isSubMenuInMenu(menuFile, action));
}

var actionRecentProjects = ui.findActionInMenu(menuFile, "Recent projects");
checkAction(actionRecentProjects, true, false, false);
assert(ui.isSubMenuInMenu(menuFile, actionRecentProjects));
var subMenuRecentProject = ui.findMenuContainedByAction(actionRecentProjects);
expect(subMenuRecentProject != null);

expectAction("actionNewProject", true);
expectAction("actionNew_Diagram", true);
expectAction("actionOpen", true);
expectAction("actionQuit", true);

expectAction("actionPrint", false);
expectAction("actionSave_diagram_as_a_picture", false);
expectAction("actionSave_as", false);
expectAction("actionSave", false);

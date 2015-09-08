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

var menuFile = ui.getMenu("menu_File");
assert(menuFile != null);

utils.activateMenu(menuFile);
api.wait(200);

var actionNewProject = ui.getActionInMenu(menuFile, "actionNewProject");
checkAction(actionNewProject, true, false, false);
expect(!ui.isSubMenuInMenu(menuFile, actionNewProject));

var actionNewDiagram = ui.getActionInMenu(menuFile, "actionNew_Diagram");
checkAction(actionNewDiagram, true, false, false);
expect(!ui.isSubMenuInMenu(menuFile, actionNewDiagram));

var actionRecentProjects = ui.getActionInMenu(menuFile, "Recent projects");
checkAction(actionRecentProjects, true, false, false);
assert(ui.isSubMenuInMenu(menuFile, actionRecentProjects));
var subMenuRecentProject = ui.getMenuContainedByAction(actionRecentProjects);
expect(subMenuRecentProject != null);

var actionOpen = ui.getActionInMenu(menuFile, "actionOpen");
checkAction(actionOpen, true, false, false);
expect(!ui.isSubMenuInMenu(menuFile, actionOpen));

var actionSave = ui.getActionInMenu(menuFile, "actionSave");
checkAction(actionSave, false, false, false);
expect(!ui.isSubMenuInMenu(menuFile, actionSave));

var actionSaveAs = ui.getActionInMenu(menuFile, "actionSave_as");
checkAction(actionSaveAs, false, false, false);
expect(!ui.isSubMenuInMenu(menuFile, actionSaveAs));

var actionPicture = ui.getActionInMenu(menuFile, "actionSave_diagram_as_a_picture");
checkAction(actionPicture, false, false, false);
expect(!ui.isSubMenuInMenu(menuFile, actionPicture));

var actionPrint = ui.getActionInMenu(menuFile, "actionPrint");
checkAction(actionPrint, false, false, false);
expect(!ui.isSubMenuInMenu(menuFile, actionPrint));

var actionQuit = ui.getActionInMenu(menuFile, "actionQuit");
checkAction(actionQuit, true, false, false);
expect(!ui.isSubMenuInMenu(menuFile, actionQuit));

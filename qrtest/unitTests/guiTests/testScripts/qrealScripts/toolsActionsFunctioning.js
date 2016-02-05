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

var menuTools = ui.getMenu("menuTools");
assert(menuTools != null);

utils.activateMenu(menuTools);
api.wait(200);

var actionGesturesShow = ui.getActionInMenu(menuTools, "actionGesturesShow");
checkAction(actionGesturesShow, true, false, false);
expect(!ui.isSubMenuInMenu(menuTools, actionGesturesShow));

// todo: add new checks

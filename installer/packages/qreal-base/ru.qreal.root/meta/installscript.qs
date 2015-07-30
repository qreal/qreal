// todo: everything below till the constructor is a horrible workarround for QtIFW`s qts localization bugs.
var langIndex = function() {
	return gui.pageWidgetByObjectName("IntroductionPage").MessageLabel.text[0] == "W" ? 0 : 1;
}

// cool escaping tool: http://www.mobilefish.com/services/unicode_escape_sequence_converter/unicode_escape_sequence_converter.php
var nonEmptyMessage = [
		"The installation path cannot be empty, please specify a valid folder.",
		// "Необходимо задать путь к каталогу установки. Выберите подходящий каталог."];
		"\u041d\u0435\u043e\u0431\u0445\u043e\u0434\u0438\u043c\u043e \u0437\u0430\u0434\u0430\u0442\u044c \u043f\u0443\u0442\u044c \u043a \u043a\u0430\u0442\u0430\u043b\u043e\u0433\u0443 \u0443\u0441\u0442\u0430\u043d\u043e\u0432\u043a\u0438. \u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u043f\u043e\u0434\u0445\u043e\u0434\u044f\u0449\u0438\u0439 \u043a\u0430\u0442\u0430\u043b\u043e\u0433."];
var noRelative = [
		"The installation path cannot be relative, please specify an absolute path.",
		// "Путь к каталогу установки не может быть относительным. Задайте абсолютный путь."];
		"\u041f\u0443\u0442\u044c \u043a \u043a\u0430\u0442\u0430\u043b\u043e\u0433\u0443 \u0443\u0441\u0442\u0430\u043d\u043e\u0432\u043a\u0438 \u043d\u0435 \u043c\u043e\u0436\u0435\u0442 \u0431\u044b\u0442\u044c \u043e\u0442\u043d\u043e\u0441\u0438\u0442\u0435\u043b\u044c\u043d\u044b\u043c. \u0417\u0430\u0434\u0430\u0439\u0442\u0435 \u0430\u0431\u0441\u043e\u043b\u044e\u0442\u043d\u044b\u0439 \u043f\u0443\u0442\u044c."];
var noAscii = [
		"The path or installation directory contains non ASCII characters. This is currently not supported! Please choose a different path or installation directory.",
		// "В строке пути или в имени каталога установки содержится символ, не относящийся к ASCII. В настоящее время такие символы не поддерживаются. Выберите другой путь или каталог установки."];
		"\u0412 \u0441\u0442\u0440\u043e\u043a\u0435 \u043f\u0443\u0442\u0438 \u0438\u043b\u0438 \u0432 \u0438\u043c\u0435\u043d\u0438 \u043a\u0430\u0442\u0430\u043b\u043e\u0433\u0430 \u0443\u0441\u0442\u0430\u043d\u043e\u0432\u043a\u0438 \u0441\u043e\u0434\u0435\u0440\u0436\u0438\u0442\u0441\u044f \u0441\u0438\u043c\u0432\u043e\u043b, \u043d\u0435 \u043e\u0442\u043d\u043e\u0441\u044f\u0449\u0438\u0439\u0441\u044f \u043a ASCII. \u0412 \u043d\u0430\u0441\u0442\u043e\u044f\u0449\u0435\u0435 \u0432\u0440\u0435\u043c\u044f \u0442\u0430\u043a\u0438\u0435 \u0441\u0438\u043c\u0432\u043e\u043b\u044b \u043d\u0435 \u043f\u043e\u0434\u0434\u0435\u0440\u0436\u0438\u0432\u0430\u044e\u0442\u0441\u044f. \u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u0434\u0440\u0443\u0433\u043e\u0439 \u043f\u0443\u0442\u044c \u0438\u043b\u0438 \u043a\u0430\u0442\u0430\u043b\u043e\u0433 \u0443\u0441\u0442\u0430\u043d\u043e\u0432\u043a\u0438."];
var noRootOrHome = [
		"As the install directory is completely deleted, installing in %1 is forbidden.",
		// "Так как при деинсталляции каталог установки будет полностью удалён, установка в %1 запрещена."];
		"\u0422\u0430\u043a \u043a\u0430\u043a \u043f\u0440\u0438 \u0434\u0435\u0438\u043d\u0441\u0442\u0430\u043b\u043b\u044f\u0446\u0438\u0438 \u043a\u0430\u0442\u0430\u043b\u043e\u0433 \u0443\u0441\u0442\u0430\u043d\u043e\u0432\u043a\u0438 \u0431\u0443\u0434\u0435\u0442 \u043f\u043e\u043b\u043d\u043e\u0441\u0442\u044c\u044e \u0443\u0434\u0430\u043b\u0451\u043d, \u0443\u0441\u0442\u0430\u043d\u043e\u0432\u043a\u0430 \u0432 %1 \u0437\u0430\u043f\u0440\u0435\u0449\u0435\u043d\u0430."];
var tooLong = [
		"The path you have entered is too long, please make sure to specify a valid path.",
		// "Введённый путь слишком длинный, введите более короткий путь."];
		"\u0412\u0432\u0435\u0434\u0451\u043d\u043d\u044b\u0439 \u043f\u0443\u0442\u044c \u0441\u043b\u0438\u0448\u043a\u043e\u043c \u0434\u043b\u0438\u043d\u043d\u044b\u0439, \u0432\u0432\u0435\u0434\u0438\u0442\u0435 \u0431\u043e\u043b\u0435\u0435 \u043a\u043e\u0440\u043e\u0442\u043a\u0438\u0439 \u043f\u0443\u0442\u044c."];
var endsWithPoint = [
		"The installation path must not end with '.', please specify a valid folder.",
		// "Путь к каталогу не должен заканчиваться точкой, введите корректный путь."];
		"\u041f\u0443\u0442\u044c \u043a \u043a\u0430\u0442\u0430\u043b\u043e\u0433\u0443 \u043d\u0435 \u0434\u043e\u043b\u0436\u0435\u043d \u0437\u0430\u043a\u0430\u043d\u0447\u0438\u0432\u0430\u0442\u044c\u0441\u044f \u0442\u043e\u0447\u043a\u043e\u0439, \u0432\u0432\u0435\u0434\u0438\u0442\u0435 \u043a\u043e\u0440\u0440\u0435\u043a\u0442\u043d\u044b\u0439 \u043f\u0443\u0442\u044c."];
var containsRestricted = [
		"The installation path must contains spaces or other restricted characters, please specify a valid folder.",
		// "Путь к каталогу установки содержит пробелы или другие запрещенные символы, пожалуйста, выберите другой каталог."];
		"\u041f\u0443\u0442\u044c \u043a \u043a\u0430\u0442\u0430\u043b\u043e\u0433\u0443 \u0443\u0441\u0442\u0430\u043d\u043e\u0432\u043a\u0438 \u0441\u043e\u0434\u0435\u0440\u0436\u0438\u0442 \u043f\u0440\u043e\u0431\u0435\u043b\u044b \u0438\u043b\u0438 \u0434\u0440\u0443\u0433\u0438\u0435 \u0437\u0430\u043f\u0440\u0435\u0449\u0435\u043d\u043d\u044b\u0435 \u0441\u0438\u043c\u0432\u043e\u043b\u044b, \u043f\u043e\u0436\u0430\u043b\u0443\u0439\u0441\u0442\u0430, \u0432\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u0434\u0440\u0443\u0433\u043e\u0439 \u043a\u0430\u0442\u0430\u043b\u043e\u0433."];
var preclaimer = [
		"Please specify the folder where %1 will be installed",
		// "Укажите путь к папке, куда будет произвдена установка %1"];
		"\u0423\u043a\u0430\u0436\u0438\u0442\u0435 \u043f\u0443\u0442\u044c \u043a \u043f\u0430\u043f\u043a\u0435, \u043a\u0443\u0434\u0430 \u0431\u0443\u0434\u0435\u0442 \u043f\u0440\u043e\u0438\u0437\u0432\u0434\u0435\u043d\u0430 \u0443\u0441\u0442\u0430\u043d\u043e\u0432\u043a\u0430 %1"];
var header = [
		"Installation Folder",
		// "Установочная директория"];
		"\u0423\u0441\u0442\u0430\u043d\u043e\u0432\u043e\u0447\u043d\u0430\u044f \u0434\u0438\u0440\u0435\u043a\u0442\u043e\u0440\u0438\u044f"];
var fileDialogHeader = [
		"Choose your target directory",
		// "Выберите директорию для установки"];
		"\u0412\u044b\u0431\u0435\u0440\u0438\u0442\u0435 \u0434\u0438\u0440\u0435\u043a\u0442\u043e\u0440\u0438\u044e \u0434\u043b\u044f \u0443\u0441\u0442\u0430\u043d\u043e\u0432\u043a\u0438"];
var overwrite = [
		"A previous installation exists in this folder. If you wish to continue, everything will be overwritten.",
		// "Другая версия программы уже существует в этой директории. Если Вы желаете продолжить, она будет удалена."];
		"\u0414\u0440\u0443\u0433\u0430\u044f \u0432\u0435\u0440\u0441\u0438\u044f \u043f\u0440\u043e\u0433\u0440\u0430\u043c\u043c\u044b \u0443\u0436\u0435 \u0441\u0443\u0449\u0435\u0441\u0442\u0432\u0443\u0435\u0442 \u0432 \u044d\u0442\u043e\u0439 \u0434\u0438\u0440\u0435\u043a\u0442\u043e\u0440\u0438\u0438. \u0415\u0441\u043b\u0438 \u0412\u044b \u0436\u0435\u043b\u0430\u0435\u0442\u0435 \u043f\u0440\u043e\u0434\u043e\u043b\u0436\u0438\u0442\u044c, \u043e\u043d\u0430 \u0431\u0443\u0434\u0435\u0442 \u0443\u0434\u0430\u043b\u0435\u043d\u0430."];


// Constructor
function Component()
{
	// Executable names must be lower-case product name with hyphens instead of spaces
	installer.executableName = installer.value("ProductName").toLowerCase().replace(/\s/g, "-");
	installer.linkExtension = installer.value("os") === "win" ? ".lnk" : "";
	installer.execExtension = installer.value("os") === "win" ? ".exe" : installer.value("os") === "mac" ? ".app" : "";
	installer.maintenanceName = "maintenance" + installer.execExtension;
	installer.shouldDeinstallPrevious = false;

	component.loaded.connect(this, Component.prototype.installerLoaded);
	installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
}

Component.prototype.createOperations = function()
{
	if (installer.shouldDeinstallPrevious) {
		component.addOperation("Execute", "@TargetDir@/" + installer.maintenanceName);
		if (installer.value("os") == "win") {
			var timeoutBatch = "ping 127.0.0.1 -n 4 > nul";
			component.addOperation("Execute", "cmd", "/c", timeoutBatch);
			var joinBatch = "for /l %N in () do (tasklist | find \"cscript\" >nul && ping 127.0.0.1 -n 2 >nul || exit 0) ";
			component.addOperation("Execute", "cmd", "/c", joinBatch);
		}
	}
	component.createOperations();
	if (installer.value("os") == "win") {
		component.addOperation("CreateShortcut"
				, "@TargetDir@/" + installer.executableName + installer.execExtension
				, "@StartMenuDir@/@ProductName@ @Version@" + installer.linkExtension);
		component.addOperation("CreateShortcut"
				, "@TargetDir@/" + installer.maintenanceName
				, "@StartMenuDir@/Uninstall @ProductName@" + installer.linkExtension);
		component.addOperation("Execute"
				, "@TargetDir@/" + installer.executableName + installer.execExtension
				, "--clear-conf");
	} else if (installer.value("os") == "mac") {
		component.addOperation("Execute"
				, "@TargetDir@/" + installer.value("ProductName") + ".app/Contents/MacOS/" + installer.executableName, "--clear-conf");
	} else {
		component.addOperation("Execute"
				, "bash"
				, "-c"
				, "LD_LIBRARY_PATH=@TargetDir@ @TargetDir@/" + installer.executableName + installer.execExtension + " --clear-conf");
	}
}

// Utility function like QString QDir::toNativeSeparators(const QString & pathName) [static]
var Dir = new function () {
	this.toNativeSeparator = function (path) {
		if (installer.value("os") == "win")
			return path.replace(/\//g, '\\');
		return path;
	}

	this.isRelative = function(path) {
		return (installer.value("os") == "win") ? (path.length < 2 || path[1] != ":") : (path[0] != "/");
	}

	this.isRoot = function(path) {
		return (installer.value("os") == "win") ? (path.lenght == 2 && path[1] == ":") : (path == "/");
	}

	this.isHome = function(path) {
		// TODO
		return false;
	}

	this.validatePath = function(path) {
		if (path.length == 0) {
			return nonEmptyMessage[langIndex()];
		}

		if (Dir.isRelative(path)) {
			return noRelative[langIndex()];
		}

		var nativeTargetDir = Dir.toNativeSeparator(path);
		if (!installer.value("allowNonAsciiCharacters")) {
			for (var i = 0; i < nativeTargetDir.length; ++i) {
				if (nativeTargetDir[i] & 0xff80) {
					return noAscii[langIndex()];
				}
			}
		}

		if (Dir.isRoot(path) || Dir.isHome(path)) {
			return noRootOrHome[langIndex()].arg(Dir.toNativeSeparator(path));
		}

		var ambiguousChars;
		if (installer.value("os") == "win") {
			// folder length (set by user) + maintenance tool name length (no extension) + extra padding
			if ((nativeTargetDir.length + installer.maintenanceName.length + 20) >= 260/*MAX_PATH*/) {
				return tooLong[langIndex()];
			}

			if (nativeTargetDir[nativeTargetDir.length - 1] == '.') {
				return endsWithPoint[langIndex()];
			}

			ambiguousChars = "[\"~<>|?*!@#$%^&,; ]|(\\\\CON)|(\\\\PRN)|(\\\\AUX)|(\\\\NUL)|(\\\\COM\\d)|(\\\\LPT\\d)";
		} else {
			ambiguousChars = "[~<>|?*!@#$%^&:,; \\\\]";
		}


		// check if there are not allowed characters in the target path
		if (nativeTargetDir.match(ambiguousChars)) {
			return containsRestricted[langIndex()];
		}

		return "";
	}
};

// Called as soon as the component was loaded
Component.prototype.installerLoaded = function()
{
	if (installer.addWizardPage(component, "TargetWidget", QInstaller.TargetDirectory)) {
		var widget = gui.pageWidgetByObjectName("DynamicTargetWidget");
		if (widget != null) {
			widget.label.text = preclaimer[langIndex()].arg(installer.value("ProductName"));
			widget.targetDirectory.textChanged.connect(this, Component.prototype.targetChanged);
			widget.targetChooser.clicked.connect(this, Component.prototype.chooseTarget);

			widget.windowTitle = header[langIndex()];
			widget.targetDirectory.text = Dir.toNativeSeparator(installer.value("TargetDir"));
		}
	}
}

// Callback when one is clicking on the button to select where to install your application
Component.prototype.chooseTarget = function () {
	var widget = gui.pageWidgetByObjectName("DynamicTargetWidget");
	if (widget != null) {
		var newTarget = QFileDialog.getExistingDirectory(fileDialogHeader[langIndex()], widget.targetDirectory.text);
		if (newTarget != "") {
			widget.targetDirectory.text = Dir.toNativeSeparator(newTarget);
		}
	}
}

Component.prototype.targetChanged = function (text) {
	var widget = gui.pageWidgetByObjectName("DynamicTargetWidget");
	var error = Dir.validatePath(text);
	if (error.length) {
		widget.labelOverwrite.text = "<font color='red'>" + error + "</font>";
		widget.complete = false;
		return;
	}

	if (installer.isInstaller() && widget != null) {
		if (text != "") {
			widget.complete = true;
			installer.setValue("TargetDir", text);
			if (installer.fileExists(text + "/" + installer.maintenanceName)) {
				var warning = "<font color='green'>" + overwrite[langIndex()] + "</font>";
				widget.labelOverwrite.text = warning;
				installer.shouldDeinstallPrevious = true;
			} else {
				widget.labelOverwrite.text = "";
				installer.shouldDeinstallPrevious = false;
			}
			return;
		}
		widget.complete = false;
	}
}

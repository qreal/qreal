function Component()
{
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (installer.value("os") === "win") {
        component.addOperation("RegisterFileType",
                               "qrs",
                               "@TargetDir@/" + installer.executableName + "  '%1'",
                               "@ProductName@ Project",
                               "application/octet-stream");
    }
}

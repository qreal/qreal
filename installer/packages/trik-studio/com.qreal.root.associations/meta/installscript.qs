function Component()
{
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (installer.value("os") === "win") {
        component.addOperation("RegisterFileType",
                               "qrs",
                               "@TargetDir@/trik-studio '%1'",
                               "Trik Studio Project",
                               "application/octet-stream");
    }
}

// A script to automate installer building process on Windows machines.
// Uses FSharp Interactive to detect Qt, make needed adjustments to .xml file
// and to run BitRock InstallBuilder.
//
// Usage:  fsi build.fsx <XML installer script name> <QReal version number>
//
// Example: fsi build.fsx qrealRobots.xml 1.9.4
// Requires fsi.exe, builder-cli.exe and Qt in the PATH.
// Example of their locations: 
// C:\Program Files\Microsoft F#\v4.0\Fsi.exe
// C:\Program Files\BitRock InstallBuilder for Qt 8.0.1\bin\builder-cli.exe
// C:\Libs\QtSDK\Desktop\Qt\4.7.3\mingw\bin

let scriptName = fsi.CommandLineArgs.[1]

let scriptsToPatch = 
    let directoryInfo = System.IO.DirectoryInfo(".")
    directoryInfo.GetFiles("*.xml") |> Seq.map (fun x -> x.Name)
    
let version = fsi.CommandLineArgs.[2]

let installBuilderArgs = fsi.CommandLineArgs |> Seq.skip 3 |> Seq.fold (fun acc x -> acc + " " + x) ""

let autodetectQt = 
    let pathVariable = System.Environment.GetEnvironmentVariable("PATH")
    let pathes = pathVariable.Split ';'

    let rec findQt (list : string list) =
        match list with
        | h::t when h.Contains "Qt" && h.Contains "mingw" && h.Contains "bin" -> h 
        | h::t -> findQt t
        | _ -> ""
        
    pathes |> List.ofArray |> findQt
    
let modifyScriptName (scriptName : string) = (scriptName.Split '.').[0] + "-modified.xml"
    
let patch scriptName =
    let modifiedScriptName = modifyScriptName scriptName
    let installerScript = System.IO.File.ReadAllText scriptName

    let modifiedInstallerScript = ref <| installerScript.Replace("%1", "..").Replace("%2", autodetectQt).Replace("<version>%version%</version>", "<version>" + version + "</version>")
    
    scriptsToPatch |> Seq.iter (
        fun includedScriptName -> 
            modifiedInstallerScript := (!modifiedInstallerScript).Replace("<include file=\"" + includedScriptName + "\" />", "<include file=\"" + (modifyScriptName includedScriptName) + "\" />")
    )
    
    System.IO.File.WriteAllText (modifiedScriptName, !modifiedInstallerScript)

scriptsToPatch |> Seq.iter patch
    
let modifiedScriptName = (scriptName.Split '.').[0] + "-modified.xml"
let installerScript = System.IO.File.ReadAllText scriptName
    
let exec processName args =
    let psi = new System.Diagnostics.ProcessStartInfo(processName)
    psi.Arguments <- args
    psi.UseShellExecute <- false
    let p = System.Diagnostics.Process.Start(psi)
    p.WaitForExit()
    p.ExitCode

exec "builder-cli.exe" ("build " + modifiedScriptName + " --verbose " + installBuilderArgs)

scriptsToPatch |> Seq.iter (fun scriptName -> System.IO.File.Delete (modifyScriptName scriptName))

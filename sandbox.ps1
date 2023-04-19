$ROOTFS = $(Get-Location)
$APPFS  = $(Join-Path $ROOTFS "sandbox")
$SRCFS  = $(Join-Path $APPFS "src")
$BINFS  = $(Join-Path $APPFS "bin")
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Create bin folder
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
if (Test-Path $BINFS) { Remove-Item -Recurse -Force $BINFS  | Out-Null }
New-Item $BINFS -ItemType Directory | Out-Null
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Compile library
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
$COMMAND = "C:\'Program Files'\LLVM\bin\clang.exe"
$COMMAND += " -g -Wall -Werror"
$COMMAND += " $(Get-ChildItem -Path $SRCFS -Recurse -Include *.c)"
$COMMAND += " -I$SRCFS -I$(Join-Path $ROOTFS "engine/src")"
$COMMAND += " -L$(Join-Path $ROOTFS "engine/bin") -l'talabarte.lib'"
$COMMAND += " -o $(Join-Path $BINFS "sandbox.exe")"

Write-Host "Compiling sandbox.exe"
Invoke-Expression "$COMMAND"
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Copy execution dependencies
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Copy-Item -Path $(Join-Path $ROOTFS "/engine/bin/talabarte.dll") -Destination $BINFS

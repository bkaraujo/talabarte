$ROOTFS = $(Get-Location)
$APPFS  = $(Join-Path $ROOTFS "engine")
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
$COMMAND = "C:\'Program Files'\LLVM\bin\clang.exe -std=c17"
$COMMAND += " -o0 -g -shared -Wall -Werror" 
$COMMAND += " -DEXPORT -DUNICODE -D_UNICODE"
$COMMAND += " $(Get-ChildItem -Path $SRCFS -Recurse -Include *.c)"
$COMMAND += " -I$SRCFS"
$COMMAND += " -luser32"
$COMMAND += " -o $(Join-Path $BINFS "talabarte.dll")"

Write-Host "Compiling talabarte.dll"
Invoke-Expression "$COMMAND"
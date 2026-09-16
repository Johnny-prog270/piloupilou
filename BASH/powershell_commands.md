# PowerShell Translation from Bash

> Each Bash command is translated to its PowerShell equivalent.
> When no direct equivalent exists, the closest alternative or a note is provided.

---

## File Navigation

```powershell
Get-ChildItem               # ls
Get-ChildItem -l            # ls -l  (detailed, default in PS)
Get-ChildItem -Force        # ls -a  (show hidden files)
Get-ChildItem -Force        # ls -li (no exact inode equivalent, use -Force)
Get-ChildItem path          # ls path
```

```powershell
New-Item -ItemType HardLink -Name file2 -Value file1   # ln file1 file2
New-Item -ItemType SymbolicLink -Name file2 -Value file1  # ln -s file1 file2
```

```powershell
Set-Location ~              # cd ~
Set-Location path           # cd path
Set-Location dir            # cd dir
Set-Location ..             # cd ..
Set-Location -             # cd -  (go back to previous location)
```

```powershell
Get-Location                # pwd
```

```powershell
Get-Content file            # cat file
Get-Content file1, file2    # cat file1 file2
# No direct equivalent for "cat > file" (interactive write), use:
Set-Content file            # write to file (then type content via script)
```

```powershell
Write-Output "text"         # echo text
Write-Host "text"           # echo -e text  (supports colors/formatting)
```

```powershell
(Get-Content file).Count                         # wc file (word/line count)
(Get-Content file | Measure-Object -Line).Lines  # wc -l file
```

```powershell
# less / more — no direct equivalent, but:
Get-Content file | Out-Host -Paging              # less file / more file
```

```powershell
Select-String "pattern" file                     # grep pattern file
Get-ChildItem -Recurse | Select-String "pattern" # grep -r pattern dir
Select-String -Pattern "pattern" -CaseSensitive:$false file  # grep -i
Select-String "pattern" file | Select-Object LineNumber, Line # grep -n
Select-String -Pattern "pattern" file            # grep -E "pattern"
```

```powershell
Get-ChildItem -Recurse -Filter "pattern"         # find dir -name pattern
Get-ChildItem -Recurse -File                     # find . -type f
```

```powershell
Get-Content file.txt -TotalCount 10              # head -n 10 file.txt
# head -c 10 (bytes) — no direct equivalent, use:
(Get-Content -Raw file.txt).Substring(0, 10)     # head -c 10 file.txt
Get-Content file.txt -Tail 10                    # tail -n 10 file.txt
```

```powershell
# file -b (detect file type) — no native equivalent
# Use .Extension property:
(Get-Item file).Extension                        # rough equivalent of file -b
```

```powershell
Get-Item file | Format-List *                    # stat -c "format" file
```

---

## File Modification

```powershell
Copy-Item src dest                               # cp src dest
Copy-Item srcdir srcdst -Recurse                 # cp -r srcdir srcdst
Copy-Item src dest -Confirm                      # cp -i src dest  (prompt before overwrite)
```

```powershell
Move-Item old new                                # mv old new
Move-Item old path                               # mv old path
Move-Item old new -Confirm                       # mv -i old new
```

```powershell
Remove-Item file                                 # rm file
Remove-Item file -Recurse                        # rm -r file
Remove-Item file -Confirm                        # rm -i file
Remove-Item file -Force                          # rm -f file
```

```powershell
New-Item -ItemType Directory dir                 # mkdir dir
New-Item -ItemType Directory -Force path         # mkdir -p path  (create intermediate dirs)
```

```powershell
Sort-Object                                      # sort file.txt  (pipe: Get-Content file.txt | Sort-Object)
Sort-Object -Descending                          # sort -r file.txt
Sort-Object {[int]$_}                            # sort -n file.txt  (numeric sort)
```

---

## Process

```powershell
Get-Date                                         # date
Get-Date -Format "yyyy-MM-dd HH:mm:ss"           # date +"%Y-%m-%d %H:%M:%S"
[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds() # date +%s%1N (unix timestamp ms)
```

```powershell
# wait pid — no direct equivalent for waiting on specific PID
Wait-Process -Id pid                             # wait pid
# wait %1 (job) — use PowerShell jobs:
Wait-Job 1                                       # wait %1
```

```powershell
# top / htop — no native equivalent
Get-Process | Sort-Object CPU -Descending        # rough equivalent of top
# For interactive top-like view, install: btop or use Task Manager
```

```powershell
Get-Process                                      # ps
Get-Process                                      # ps -e  (all processes, same command)
```

```powershell
Start-Process command                            # setsid command  (detach from terminal)
# For background jobs:
Start-Job { command }
```

```powershell
Stop-Process -Id pid                             # kill process
Stop-Process -Id pid -Force                      # kill -9 process  (force kill)
# kill -l (list signals) — no direct equivalent in PowerShell
[System.Enum]::GetNames([System.Diagnostics.ProcessPriorityClass])
```

---

## Variables / Commands

```powershell
Get-Command executable                           # which executable
Get-Command ls                                   # command ls
Get-Command ls | Format-List                     # command -V ls  (verbose info)
(Get-Command ls).Source                          # command -v ls  (path only)
# \ls (bypass alias) — use & with full path or:
& (Get-Command -CommandType Application ls).Source  # bypass alias
```

```powershell
Get-Command executable                           # where executable (PowerShell already uses Get-Command)
```

```powershell
Get-Content file.txt | Select-Object -Unique     # uniq file.txt
```

```powershell
(Get-Content file.txt) | ForEach-Object { $_.Substring(a, b-a) }  # cut -c a-b file.txt
# cut -sd "," -f a data.csv:
Import-Csv data.csv | Select-Object -ExpandProperty ColumnName     # closest equivalent
```

```powershell
Set-Alias new "command"                          # alias new = "command"
Remove-Item Alias:new                            # unalias new
Remove-Item Alias:*                              # unalias -a  (remove all aliases)
```

```powershell
. $PROFILE                                       # source ~/.bashrc
. $PROFILE                                       # . ~/.bashrc  (same thing)
```

---

## Variables Management

```powershell
Get-Variable                                     # set
$ErrorActionPreference = "Stop"                  # set -e  (stop on error)
Set-StrictMode -Version Latest                   # set -u  (error on undefined vars)
Set-PSDebug -Trace 1                             # set -x  (debug/trace mode)
```

```powershell
Get-ChildItem Env:                               # env
# env -i command — no direct equivalent
# env python3 script.py:
& python3 script.py                              # run script with env
```

```powershell
$env:PATH                                        # printenv PATH
[System.Environment]::GetEnvironmentVariable("PATH")
```

```powershell
Set-Variable -Name VAR -Value val -Option ReadOnly   # declare -r / readonly VAR
```

```powershell
$env:VAR = "value"                               # export VAR  (env vars are auto-exported in PS)
```

```powershell
Remove-Variable VAR                              # unset VAR
Remove-Item Env:VAR                              # unset env var
```

```powershell
Set-Variable -Name VAR -Option ReadOnly          # readonly VAR
```

---

## Compressed Files

```powershell
Compress-Archive -Path f1, f2, dir1 -DestinationPath archive.zip   # zip archive.zip f1 f2 dir1
Expand-Archive -Path archive.zip -DestinationPath .                 # unzip archive.zip
# unzip -l (list contents):
(Add-Type -AssemblyName System.IO.Compression.FileSystem; [IO.Compression.ZipFile]::OpenRead("archive.zip").Entries)
```

```powershell
# gzip, xz — no native PowerShell cmdlets
# Requires 7-Zip or using .NET classes, or installing community modules
# Example with 7zip (if installed):
& "C:\Program Files\7-Zip\7z.exe" a archive.gz file1     # gzip
& "C:\Program Files\7-Zip\7z.exe" e archive.gz           # ungzip
```

```powershell
# tar — PowerShell 5+ on Windows has no native tar cmdlet
# Windows 10/11 ships with BSD tar in System32:
tar -cf archive.tar f1 f2                        # same syntax works in modern Windows!
tar -xf archive.tar
tar -tf archive.tar
tar -czf archive.tar.gz f1 f2
tar -xzf archive.tar.gz
tar -cJf archive.tar.xz f1 f2
tar -xJf archive.tar.xz
# All tar commands work as-is in Windows 10/11 via the built-in tar.exe
```

---

## Permissions and Users

```powershell
# chmod — no direct equivalent (Windows uses ACLs, not Unix permissions)
# Closest equivalent:
$acl = Get-Acl file
$rule = New-Object System.Security.AccessControl.FileSystemAccessRule("user","FullControl","Allow")
$acl.SetAccessRule($rule)
Set-Acl file $acl
# For scripts, use:
Unblock-File script.ps1                          # rough equivalent of chmod +x
```

```powershell
# chown — no direct equivalent
$acl = Get-Acl file
$acl.SetOwner([System.Security.Principal.NTAccount]"alice")
Set-Acl file $acl                                # chown alice file
```

```powershell
# sudo — no exact equivalent in PowerShell
Start-Process powershell -Verb RunAs            # opens elevated (admin) shell — sudo -i
# Run single command as admin requires a new elevated process
```

```powershell
exit                                             # exit
```

```powershell
# su — no direct equivalent (Windows uses runas)
runas /user:alice "cmd"                          # su alice
runas /user:alice "cmd /c ls C:\Users"           # su -c "ls /root" alice
```

```powershell
New-LocalUser -Name "name"                       # adduser name
New-LocalGroup -Name "name"                      # addgroup name
Add-LocalGroupMember -Group "group" -Member "user"  # usermod -aG group user
Set-LocalUser -Name "user" -Password (Read-Host -AsSecureString)  # passwd user
Remove-LocalUser -Name "user"                    # userdel -r user
Remove-LocalGroup -Name "group"                  # groupdel group
```

```powershell
# test command — use if/conditionals directly in PowerShell:
Test-Path path                                   # test -e (file exists)
(Get-Item file).Length -gt 0                     # test -s (file not empty)
```

```powershell
$var = Read-Host "prompt"                        # read -p "prompt" var
$var = Read-Host -AsSecureString                 # read -s  (silent/password input)
# read -d ";" — no direct equivalent
```

```powershell
# trap — use try/catch in PowerShell:
try { command } catch { "error handler" }        # trap 'command' SIGNAL
# Ctrl+C handling:
[Console]::TreatControlCAsInput = $true          # trap - SIGINT  (rough equivalent)
```

```powershell
# exec — no direct equivalent
# Replace current process:
& command                                        # exec command  (runs but doesn't replace shell)
# Redirect output:
& command | Out-File output.log                  # exec 3>output.log
```

```powershell
& command | Tee-Object -Append -FilePath output.log   # tee -a output.log
```

---

## Quick Reference Summary

| Bash | PowerShell | Notes |
|---|---|---|
| `ls` | `Get-ChildItem` / `gci` / `dir` | All aliases work |
| `cd` | `Set-Location` / `sl` | |
| `pwd` | `Get-Location` / `gl` | |
| `cat` | `Get-Content` / `gc` | |
| `cp` | `Copy-Item` / `cp` | Alias `cp` exists |
| `mv` | `Move-Item` / `mv` | Alias `mv` exists |
| `rm` | `Remove-Item` / `rm` | Alias `rm` exists |
| `mkdir` | `New-Item -ItemType Directory` / `mkdir` | |
| `grep` | `Select-String` / `sls` | |
| `find` | `Get-ChildItem -Recurse` | |
| `echo` | `Write-Output` / `echo` | |
| `ps` | `Get-Process` / `gps` | |
| `kill` | `Stop-Process` / `kill` | |
| `chmod` | `Set-Acl` | Windows uses ACLs |
| `sudo` | `Start-Process -Verb RunAs` | Opens new elevated shell |
| `export VAR` | `$env:VAR = "value"` | |
| `unset VAR` | `Remove-Variable VAR` | |
| `alias` | `Set-Alias` | |
| `source` | `. $PROFILE` | |

---

*Generated by Claude Code — Bash to PowerShell translation guide*

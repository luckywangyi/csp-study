#Requires -Version 5.1
<#
.SYNOPSIS
    编译 CSP 练习用的 C++ 源文件（g++ C++17）。
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string] $CppPath,

    [switch] $CompileDebug
)

$ErrorActionPreference = 'Stop'

function Write-Info { param([string]$Msg) Write-Host $Msg -ForegroundColor Cyan }
function Write-Ok { param([string]$Msg) Write-Host $Msg -ForegroundColor Green }
function Write-Bad { param([string]$Msg) Write-Host $Msg -ForegroundColor Red }
function Write-WarnMsg { param([string]$Msg) Write-Host $Msg -ForegroundColor Yellow }

try {
    if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
        Write-Bad "错误：未找到 g++。请先安装 MinGW-w64 或 MSYS2，并确保 g++ 在 PATH 中。"
        exit 1
    }

    $resolved = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($CppPath)
    if (-not (Test-Path -LiteralPath $resolved -PathType Leaf)) {
        Write-Bad "错误：源文件不存在：$CppPath"
        exit 1
    }

    if ([System.IO.Path]::GetExtension($resolved) -ne '.cpp') {
        Write-WarnMsg "警告：文件扩展名不是 .cpp，仍将尝试编译。"
    }

    $srcDir = Split-Path -Parent -Path $resolved
    $outExe = Join-Path $srcDir 'solution.exe'
    $gppExe = (Get-Command g++ -ErrorAction Stop).Source

    if ($CompileDebug) {
        Write-Info "调试模式：启用 -g、-DDEBUG、-fsanitize=undefined（关闭 -O2）。"
        $gppArgs = @(
            '-std=c++17',
            '-g',
            '-DDEBUG',
            '-fsanitize=undefined',
            '-Wall',
            '-o', $outExe,
            $resolved
        )
    }
    else {
        $gppArgs = @('-std=c++17', '-O2', '-Wall', '-o', $outExe, $resolved)
    }

    Write-Info "正在编译：$resolved"
    Write-Info "输出文件：$outExe"

    $sw = [System.Diagnostics.Stopwatch]::StartNew()
    $stderrFile = [System.IO.Path]::GetTempFileName()
    $stdoutFile = [System.IO.Path]::GetTempFileName()
    try {
        $p = Start-Process -FilePath $gppExe -ArgumentList $gppArgs -NoNewWindow -Wait -PassThru `
            -RedirectStandardError $stderrFile -RedirectStandardOutput $stdoutFile
        $sw.Stop()
        $elapsedMs = [math]::Round($sw.Elapsed.TotalMilliseconds, 2)

        $errText = ''
        if (Test-Path -LiteralPath $stderrFile) {
            $errText = [System.IO.File]::ReadAllText($stderrFile, [System.Text.Encoding]::UTF8)
        }

        if ($p.ExitCode -ne 0) {
            Write-Bad "编译失败（退出码 $($p.ExitCode)），耗时 ${elapsedMs} ms。"
            if ($errText.Trim().Length -gt 0) {
                Write-Host $errText
            }
            exit $p.ExitCode
        }

        Write-Ok "编译成功，耗时 ${elapsedMs} ms。"
        if ($errText.Trim().Length -gt 0) {
            Write-WarnMsg "编译器提示（含警告时常见）："
            Write-Host $errText
        }
    }
    finally {
        Remove-Item -LiteralPath $stderrFile -ErrorAction SilentlyContinue
        Remove-Item -LiteralPath $stdoutFile -ErrorAction SilentlyContinue
    }
}
catch {
    Write-Bad ('编译过程出错：' + $_.ToString())
    exit 1
}

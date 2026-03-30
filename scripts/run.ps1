#Requires -Version 5.1
<#
.SYNOPSIS
    按 CSP 考试标准评测 solution.cpp：编译 -> 对 testcases/*.in 逐点评测。
    支持时间限制 (TLE)、内存限制 (MLE)、运行错误 (RE)、答案错误 (WA)。
    得分按「通过测试点 / 总测试点 * 100」折算到 100 分制。
.EXAMPLE
    .\run.ps1 .\problems\201312\t1\solution.cpp
    .\run.ps1 .\problems\201312\t1\solution.cpp -TimeLimit 1 -MemoryLimitMB 256
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string] $CppPath,

    [double] $TimeLimit = 2.0,

    [int] $MemoryLimitMB = 256,

    [string] $SummaryPath = $null
)

$ErrorActionPreference = 'Stop'

if ([string]::IsNullOrEmpty($PSScriptRoot)) {
    if (-not [string]::IsNullOrEmpty($PSCommandPath)) {
        $PSScriptRoot = [System.IO.Path]::GetDirectoryName($PSCommandPath)
    }
    elseif ($MyInvocation.MyCommand.Path) {
        $PSScriptRoot = [System.IO.Path]::GetDirectoryName($MyInvocation.MyCommand.Path)
    }
}

function Write-Info { param([string]$Msg) Write-Host $Msg -ForegroundColor Cyan }
function Write-Ok { param([string]$Msg) Write-Host $Msg -ForegroundColor Green }
function Write-Bad { param([string]$Msg) Write-Host $Msg -ForegroundColor Red }
function Write-WarnMsg { param([string]$Msg) Write-Host $Msg -ForegroundColor Yellow }

function Normalize-OutputText {
    param([string] $Text)
    if ([string]::IsNullOrEmpty($Text)) { return '' }
    $lines = @($Text -split "`r?`n" | ForEach-Object { if ($null -eq $_) { '' } else { $_.TrimEnd() } })
    $last = $lines.Length - 1
    while ($last -ge 0 -and [string]::IsNullOrWhiteSpace($lines[$last])) {
        $last--
    }
    if ($last -lt 0) { return '' }
    return ($lines[0..$last] -join "`n")
}

function Get-PeakMemoryMB {
    param([System.Diagnostics.Process] $Process)
    try {
        $bytes = $Process.PeakWorkingSet64
        return [math]::Round($bytes / 1MB, 2)
    }
    catch {
        return -1
    }
}

function Invoke-BuildIfNeeded {
    param(
        [string] $SourcePath,
        [string] $ExePath
    )
    $needBuild = $true
    if (Test-Path -LiteralPath $ExePath -PathType Leaf) {
        $srcTime = (Get-Item -LiteralPath $SourcePath).LastWriteTimeUtc
        $exeTime = (Get-Item -LiteralPath $ExePath).LastWriteTimeUtc
        if ($exeTime -ge $srcTime) { $needBuild = $false }
    }

    if (-not $needBuild) {
        Write-Info "可执行文件已存在且较新，跳过编译。"
        return $true
    }

    $scriptHome = $PSScriptRoot
    if ([string]::IsNullOrEmpty($scriptHome) -and $PSCommandPath) {
        $scriptHome = [System.IO.Path]::GetDirectoryName($PSCommandPath)
    }
    if ([string]::IsNullOrEmpty($scriptHome)) {
        Write-Bad "错误：无法确定 scripts 目录，无法定位 build.ps1。"
        return $false
    }
    $buildScript = Join-Path $scriptHome 'build.ps1'
    if (-not (Test-Path -LiteralPath $buildScript -PathType Leaf)) {
        Write-Bad "错误：找不到 build.ps1：$buildScript"
        return $false
    }

    Write-Info "正在调用 build.ps1 编译..."
    & $buildScript -CppPath $SourcePath
    if (-not $?) {
        return $false
    }
    if (-not (Test-Path -LiteralPath $ExePath -PathType Leaf)) {
        Write-Bad "错误：编译后仍未找到 $ExePath"
        return $false
    }
    return $true
}

try {
    if ($TimeLimit -le 0) {
        Write-Bad "错误：TimeLimit 必须大于 0。"
        exit 1
    }

    $resolved = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($CppPath)
    if (-not (Test-Path -LiteralPath $resolved -PathType Leaf)) {
        Write-Bad "错误：源文件不存在：$CppPath"
        exit 1
    }

    $srcDir = Split-Path -Parent -Path $resolved
    $exePath = Join-Path $srcDir 'solution.exe'

    if (-not (Invoke-BuildIfNeeded -SourcePath $resolved -ExePath $exePath)) {
        exit 1
    }

    $testRoot = Join-Path $srcDir 'testcases'
    if (-not (Test-Path -LiteralPath $testRoot -PathType Container)) {
        Write-WarnMsg "未找到 testcases 目录，进入交互模式（程序从标准输入读取，Ctrl+Z 后回车结束输入）。"
        Write-Info "运行：$exePath"
        Write-Host '---'
        & $exePath
        if (-not $?) { exit 1 }
        exit 0
    }

    $inFiles = @(Get-ChildItem -LiteralPath $testRoot -Filter '*.in' -File | Sort-Object { $_.Name })
    if ($inFiles.Count -eq 0) {
        Write-WarnMsg "testcases 目录下没有 .in 文件，进入交互模式。"
        Write-Info "运行：$exePath"
        Write-Host '---'
        & $exePath
        if (-not $?) { exit 1 }
        exit 0
    }

    $timeoutMs = [int][math]::Ceiling($TimeLimit * 1000)
    if ($timeoutMs -lt 1) { $timeoutMs = 1 }
    $memLimitBytes = [long]$MemoryLimitMB * 1024 * 1024

    Write-Host ''
    Write-Info '========== CSP 标准评测 =========='
    Write-Info "时间限制：$TimeLimit 秒 / 测试点"
    Write-Info "内存限制：$MemoryLimitMB MB"
    Write-Info "测试点数：$($inFiles.Count)"
    Write-Host ''

    $ac = 0
    $graded = 0
    $total = $inFiles.Count
    $idx = 0
    $results = @()

    foreach ($inFile in $inFiles) {
        $idx++
        $base = [System.IO.Path]::GetFileNameWithoutExtension($inFile.Name)
        $outFile = Join-Path $testRoot ($base + '.out')
        $hasExpected = Test-Path -LiteralPath $outFile -PathType Leaf

        $stdoutFile = [System.IO.Path]::GetTempFileName()
        $stderrFile = [System.IO.Path]::GetTempFileName()
        try {
            $proc = Start-Process -FilePath $exePath `
                -RedirectStandardInput $inFile.FullName `
                -RedirectStandardOutput $stdoutFile `
                -RedirectStandardError $stderrFile `
                -NoNewWindow -PassThru -WorkingDirectory $srcDir

            $sw = [System.Diagnostics.Stopwatch]::StartNew()
            $finished = $false
            $mle = $false

            while (-not $proc.HasExited) {
                $elapsedSoFar = $sw.Elapsed.TotalMilliseconds
                if ($elapsedSoFar -gt $timeoutMs) {
                    $finished = $false
                    break
                }
                try {
                    $proc.Refresh()
                    if ($proc.PeakWorkingSet64 -gt $memLimitBytes) {
                        $mle = $true
                        break
                    }
                } catch { }
                Start-Sleep -Milliseconds 20
            }

            if (-not $proc.HasExited) {
                if (-not $mle) { $finished = $false } # TLE
                try { $proc.Kill() } catch { }
                try { $proc.WaitForExit(2000) } catch { }
            }
            else {
                $finished = $true
            }

            $sw.Stop()
            $ms = [math]::Round($sw.Elapsed.TotalMilliseconds, 2)
            $peakMB = Get-PeakMemoryMB -Process $proc
            $exitCode = if ($proc.HasExited) { $proc.ExitCode } else { -1 }
            try { $proc.Dispose() } catch { }

            $memInfo = if ($peakMB -ge 0) { " 内存：${peakMB} MB" } else { '' }
            $verdict = ''

            if ($mle) {
                $verdict = 'MLE'
                Write-Bad "[#$idx/$total] $($inFile.Name) => MLE（内存超限 >${MemoryLimitMB}MB） 用时：${ms} ms$memInfo"
            }
            elseif (-not $finished) {
                $verdict = 'TLE'
                Write-Bad "[#$idx/$total] $($inFile.Name) => TLE（超时 >${TimeLimit}s） 用时：${ms} ms$memInfo"
            }
            elseif ($exitCode -ne 0) {
                $verdict = 'RE'
                $errTail = ''
                if (Test-Path -LiteralPath $stderrFile) {
                    $errTail = ([System.IO.File]::ReadAllText($stderrFile, [System.Text.Encoding]::UTF8)).Trim()
                }
                Write-Bad "[#$idx/$total] $($inFile.Name) => RE（运行时错误，退出码 $exitCode） 用时：${ms} ms$memInfo"
                if ($errTail.Length -gt 0) {
                    Write-Host "  $errTail" -ForegroundColor DarkYellow
                }
            }
            elseif (-not $hasExpected) {
                $verdict = 'NOOUT'
                Write-WarnMsg "[#$idx/$total] $($inFile.Name) => 无 .out 文件，未比对 用时：${ms} ms$memInfo"
                $actualRaw = [System.IO.File]::ReadAllText($stdoutFile, [System.Text.Encoding]::UTF8)
                $oneLine = ($actualRaw -replace "`r?`n", ' | ').Trim()
                if ($oneLine.Length -gt 200) { $oneLine = $oneLine.Substring(0, 200) + '...' }
                if ($oneLine.Length -gt 0) {
                    Write-Host "  输出预览：$oneLine" -ForegroundColor Gray
                }
            }
            else {
                $graded++
                $actualRaw = [System.IO.File]::ReadAllText($stdoutFile, [System.Text.Encoding]::UTF8)
                $expectedRaw = [System.IO.File]::ReadAllText($outFile, [System.Text.Encoding]::UTF8)
                $a = Normalize-OutputText -Text $actualRaw
                $e = Normalize-OutputText -Text $expectedRaw

                if ($a -ceq $e) {
                    $verdict = 'AC'
                    Write-Ok "[#$idx/$total] $($inFile.Name) => AC 用时：${ms} ms$memInfo"
                    $ac++
                }
                else {
                    $verdict = 'WA'
                    Write-Bad "[#$idx/$total] $($inFile.Name) => WA 用时：${ms} ms$memInfo"
                }
            }

            $results += [PSCustomObject]@{
                TestPoint = $idx
                File      = $inFile.Name
                Verdict   = $verdict
                TimeMs    = $ms
                MemoryMB  = $peakMB
            }
        }
        finally {
            Remove-Item -LiteralPath $stdoutFile -ErrorAction SilentlyContinue
            Remove-Item -LiteralPath $stderrFile -ErrorAction SilentlyContinue
        }
    }

    # === CSP 标准评分汇总 ===
    Write-Host ''
    Write-Info '========== 评测结果 =========='

    $score = 0
    if ($graded -gt 0) {
        $score = [math]::Round(100.0 * $ac / $graded)
    }

    $acCount = ($results | Where-Object { $_.Verdict -eq 'AC' }).Count
    $waCount = ($results | Where-Object { $_.Verdict -eq 'WA' }).Count
    $tleCount = ($results | Where-Object { $_.Verdict -eq 'TLE' }).Count
    $mleCount = ($results | Where-Object { $_.Verdict -eq 'MLE' }).Count
    $reCount = ($results | Where-Object { $_.Verdict -eq 'RE' }).Count

    Write-Host "  测试点总数：$total" -ForegroundColor White
    if ($graded -gt 0) {
        Write-Ok   "  AC（通过）：$acCount"
    }
    if ($waCount -gt 0) { Write-Bad    "  WA（答案错误）：$waCount" }
    if ($tleCount -gt 0) { Write-Bad   "  TLE（超时）：$tleCount" }
    if ($mleCount -gt 0) { Write-Bad   "  MLE（内存超限）：$mleCount" }
    if ($reCount -gt 0) { Write-Bad    "  RE（运行时错误）：$reCount" }

    Write-Host ''
    if ($graded -eq 0) {
        Write-WarnMsg "  没有带 .out 的测试点，无法计分。"
        $score = 0
    }
    else {
        if ($score -eq 100) {
            Write-Ok "  得分：$score / 100 分（满分通过！）"
        }
        elseif ($score -ge 60) {
            Write-WarnMsg "  得分：$score / 100 分"
        }
        else {
            Write-Bad "  得分：$score / 100 分"
        }

        if ($total -lt 10) {
            Write-WarnMsg "  提示：当前仅 $total 个测试点（CSP 标准为 10 个），建议补充更多测试数据以更准确地模拟评分。"
        }
    }
    Write-Host ''

    if (-not [string]::IsNullOrWhiteSpace($SummaryPath)) {
        $sumParent = Split-Path -Parent -Path $SummaryPath
        if (-not [string]::IsNullOrWhiteSpace($sumParent) -and -not (Test-Path -LiteralPath $sumParent)) {
            New-Item -ItemType Directory -Force -Path $sumParent | Out-Null
        }
        $utf8NoBom = New-Object System.Text.UTF8Encoding $false
        [System.IO.File]::WriteAllLines($SummaryPath, @(
            "AC=$ac", "GRADED=$graded", "TOTAL=$total", "SCORE=$score"
        ), $utf8NoBom)
    }
}
catch {
    Write-Bad "运行过程出错：$_"
    exit 1
}

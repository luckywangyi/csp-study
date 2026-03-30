#Requires -Version 5.1
<#
.SYNOPSIS
    CSP 模拟考试提交评测：编译 -> 评测 -> 按 100 分制计分 -> 记录提交历史 -> 追踪最高分。
    模拟真实 CSP 的多次提交取最高分机制（最多 32 次提交）。
.EXAMPLE
    .\submit.ps1 -Problem .\practice\exam-20260330-120000\t1\solution.cpp
#>
param(
    [Parameter(Mandatory = $true)]
    [string] $Problem,

    [double] $TimeLimit = 2.0,

    [int] $MemoryLimitMB = 256
)

$ErrorActionPreference = 'Stop'

$MockRoot = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$StudyRoot = Split-Path -Parent -Path $MockRoot
$RunScript = Join-Path $StudyRoot 'scripts\run.ps1'

function Write-InfoLine { param([string]$Msg) Write-Host $Msg -ForegroundColor Cyan }
function Write-OkLine { param([string]$Msg) Write-Host $Msg -ForegroundColor Green }
function Write-WarnLine { param([string]$Msg) Write-Host $Msg -ForegroundColor Yellow }
function Write-BadLine { param([string]$Msg) Write-Host $Msg -ForegroundColor Red }

if (-not (Test-Path -LiteralPath $RunScript -PathType Leaf)) {
    Write-BadLine "错误：找不到运行脚本：$RunScript"
    exit 1
}

$resolved = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($Problem)
if (-not (Test-Path -LiteralPath $resolved -PathType Leaf)) {
    Write-BadLine "错误：源文件不存在：$Problem"
    exit 1
}

$probDir = Split-Path -Parent -Path $resolved
$examDir = Split-Path -Parent -Path $probDir
$probLeaf = Split-Path -Leaf -Path $probDir
$logPath = Join-Path $examDir 'log.txt'
$scorePath = Join-Path $examDir 'scores.json'

$summaryFile = [System.IO.Path]::GetTempFileName()

function Append-Log {
    param([string]$Line)
    $t = Get-Date -Format 'yyyy-MM-dd HH:mm:ss'
    Add-Content -LiteralPath $logPath -Value "[$t] $Line" -Encoding UTF8
}

function Get-SubmitCount {
    if (-not (Test-Path -LiteralPath $logPath -PathType Leaf)) { return 0 }
    $content = Get-Content -LiteralPath $logPath -Encoding UTF8
    return @($content | Where-Object { $_ -match [regex]::Escape($probLeaf) -and $_ -match '提交 #' }).Count
}

function Update-BestScore {
    param([string]$ProbName, [int]$Score)
    $scores = @{}
    if (Test-Path -LiteralPath $scorePath -PathType Leaf) {
        try {
            $raw = Get-Content -LiteralPath $scorePath -Raw -Encoding UTF8
            $parsed = $raw | ConvertFrom-Json
            foreach ($prop in $parsed.PSObject.Properties) {
                $scores[$prop.Name] = [int]$prop.Value
            }
        } catch { }
    }
    $prev = if ($scores.ContainsKey($ProbName)) { $scores[$ProbName] } else { 0 }
    $best = [math]::Max($prev, $Score)
    $scores[$ProbName] = $best
    $scores | ConvertTo-Json | Set-Content -LiteralPath $scorePath -Encoding UTF8
    return @{ Previous = $prev; Best = $best }
}

$submitNum = (Get-SubmitCount) + 1
$maxSubmits = 32

Write-Host ''
Write-InfoLine "========== CSP 模拟提交 =========="
Write-InfoLine "题目：$probLeaf"
Write-InfoLine "提交次数：第 $submitNum / $maxSubmits 次"

if ($submitNum -gt $maxSubmits) {
    Write-BadLine "已达到最大提交次数 ($maxSubmits)，不再接受提交。"
    Append-Log "$probLeaf | 提交 #$submitNum | 超出提交上限"
    exit 1
}

Write-Host ''

try {
    & $RunScript -CppPath $resolved -TimeLimit $TimeLimit -MemoryLimitMB $MemoryLimitMB -SummaryPath $summaryFile
    $runOk = $?
}
catch {
    $runOk = $false
}

if (-not $runOk -or -not (Test-Path -LiteralPath $summaryFile -PathType Leaf)) {
    Write-BadLine '评测失败。'
    Append-Log "$probLeaf | 提交 #$submitNum | 评测失败 | 得分 0"
    exit 1
}

$summaryText = [System.IO.File]::ReadAllText($summaryFile, [System.Text.Encoding]::UTF8)
Remove-Item -LiteralPath $summaryFile -ErrorAction SilentlyContinue

$ac = 0; $graded = 0; $total = 0; $score = 0
foreach ($line in ($summaryText -split "`r?`n")) {
    $t = $line.Trim()
    if ($t -match '^AC=(\d+)$') { $ac = [int]$matches[1] }
    if ($t -match '^GRADED=(\d+)$') { $graded = [int]$matches[1] }
    if ($t -match '^TOTAL=(\d+)$') { $total = [int]$matches[1] }
    if ($t -match '^SCORE=(\d+)$') { $score = [int]$matches[1] }
}

if ($graded -le 0) {
    $score = 0
}

$bestInfo = Update-BestScore -ProbName $probLeaf -Score $score

Write-Host ''
Write-InfoLine '========== 提交结果 =========='
Write-InfoLine "  本次得分：$score / 100 分（AC $ac / $graded 个测试点）"

if ($bestInfo.Best -gt $bestInfo.Previous -and $bestInfo.Previous -gt 0) {
    Write-OkLine "  最高分更新：$($bestInfo.Previous) -> $($bestInfo.Best) 分"
}
elseif ($bestInfo.Previous -gt 0 -and $score -lt $bestInfo.Best) {
    Write-WarnLine "  历史最高分：$($bestInfo.Best) 分（本次未超越）"
}
elseif ($score -eq 100) {
    Write-OkLine "  满分通过！"
}

Write-InfoLine "  剩余提交次数：$($maxSubmits - $submitNum)"

Append-Log "$probLeaf | 提交 #$submitNum | 得分 $score/100 | AC $ac/$graded | 最高分 $($bestInfo.Best)"

# 显示所有题目的总分汇总
if (Test-Path -LiteralPath $scorePath -PathType Leaf) {
    try {
        $allScores = (Get-Content -LiteralPath $scorePath -Raw -Encoding UTF8) | ConvertFrom-Json
        $totalScore = 0
        $probCount = 0
        foreach ($prop in $allScores.PSObject.Properties) {
            $totalScore += [int]$prop.Value
            $probCount++
        }
        Write-Host ''
        Write-InfoLine "---------- 本场考试总分 ----------"
        foreach ($prop in $allScores.PSObject.Properties) {
            $v = [int]$prop.Value
            $color = if ($v -eq 100) { 'Green' } elseif ($v -ge 60) { 'Yellow' } else { 'Red' }
            Write-Host "  $($prop.Name)：$v 分" -ForegroundColor $color
        }
        Write-Host "  --------------------------------" -ForegroundColor White
        $totalColor = if ($totalScore -ge 300) { 'Green' } elseif ($totalScore -ge 200) { 'Yellow' } else { 'Red' }
        Write-Host "  总分：$totalScore / $($probCount * 100) 分" -ForegroundColor $totalColor
        Write-Host ''
    }
    catch { }
}


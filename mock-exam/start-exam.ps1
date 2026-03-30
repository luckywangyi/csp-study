#Requires -Version 5.1
<#
.SYNOPSIS
    启动 CSP 模拟考试：创建工作目录、复制套题与模板，并计时提醒。
.EXAMPLE
    .\start-exam.ps1
    .\start-exam.ps1 -Set set1
    .\start-exam.ps1 -Duration 120
#>
param(
    [string] $Set,

    [int] $Duration = 240
)

$ErrorActionPreference = 'Stop'

if ($Duration -lt 1) {
    Write-Host '错误：考试时长必须至少为 1 分钟。' -ForegroundColor Red
    exit 1
}

$MockRoot = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$StudyRoot = Split-Path -Parent -Path $MockRoot
$ExamSetsRoot = Join-Path $MockRoot 'exam-sets'
$PracticeRoot = Join-Path $MockRoot 'practice'
$TemplateCpp = Join-Path $StudyRoot 'templates\solution.cpp'

function Write-Title { param([string]$Msg) Write-Host $Msg -ForegroundColor Magenta }
function Write-InfoLine { param([string]$Msg) Write-Host $Msg -ForegroundColor Cyan }
function Write-OkLine { param([string]$Msg) Write-Host $Msg -ForegroundColor Green }
function Write-WarnLine { param([string]$Msg) Write-Host $Msg -ForegroundColor Yellow }
function Write-BadLine { param([string]$Msg) Write-Host $Msg -ForegroundColor Red }

if (-not (Test-Path -LiteralPath $TemplateCpp -PathType Leaf)) {
    Write-BadLine "错误：找不到模板文件：$TemplateCpp"
    exit 1
}

if (-not (Test-Path -LiteralPath $ExamSetsRoot -PathType Container)) {
    Write-BadLine "错误：找不到套题目录：$ExamSetsRoot"
    exit 1
}

$allSets = @(Get-ChildItem -LiteralPath $ExamSetsRoot -Directory | Where-Object {
        Test-Path -LiteralPath (Join-Path $_.FullName 'info.json') -PathType Leaf
    })

if ($allSets.Count -eq 0) {
    Write-BadLine '错误：exam-sets 下没有可用的套题（需包含 info.json）。'
    exit 1
}

[string] $pickedName = $null
[string] $setFolder = $null

if (-not [string]::IsNullOrWhiteSpace($Set)) {
    $candidate = Join-Path $ExamSetsRoot $Set
    if (-not (Test-Path -LiteralPath (Join-Path $candidate 'info.json') -PathType Leaf)) {
        Write-BadLine "错误：找不到套题「$Set」或其 info.json。"
        exit 1
    }
    $pickedName = $Set
    $setFolder = $candidate
}
else {
    $pick = $allSets | Get-Random
    $pickedName = $pick.Name
    $setFolder = $pick.FullName
}

$infoPath = Join-Path $setFolder 'info.json'
$rawInfo = Get-Content -LiteralPath $infoPath -Raw -Encoding UTF8
$info = $rawInfo | ConvertFrom-Json

$stamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$workName = "exam-$stamp"
$workDir = Join-Path $PracticeRoot $workName

New-Item -ItemType Directory -Force -Path $PracticeRoot | Out-Null
New-Item -ItemType Directory -Force -Path $workDir | Out-Null

foreach ($i in 1..5) {
    $src = Join-Path $setFolder "t$i"
    if (-not (Test-Path -LiteralPath $src -PathType Container)) {
        Write-BadLine "错误：套题缺少目录：$src"
        exit 1
    }
    $dst = Join-Path $workDir "t$i"
    Copy-Item -LiteralPath $src -Destination $dst -Recurse -Force
    Copy-Item -LiteralPath $TemplateCpp -Destination (Join-Path $dst 'solution.cpp') -Force
}

$startTime = Get-Date
$endTime = $startTime.AddMinutes($Duration)

$meta = [ordered]@{
    set              = $pickedName
    setDisplayName   = $info.name
    startLocal       = $startTime.ToString('yyyy-MM-dd HH:mm:ss')
    endLocal         = $endTime.ToString('yyyy-MM-dd HH:mm:ss')
    durationMinutes  = $Duration
    workDir          = $workDir
}
($meta | ConvertTo-Json -Depth 8) | Set-Content -LiteralPath (Join-Path $workDir 'exam-info.json') -Encoding UTF8

Write-Host ''
Write-Title '========== 模拟考试已启动 =========='
Write-InfoLine "套题：$($info.name)（$pickedName）"
Write-InfoLine "来源说明：$($info.source)"
Write-InfoLine "开始时间：$($startTime.ToString('yyyy-MM-dd HH:mm:ss'))"
Write-InfoLine "结束时间：$($endTime.ToString('yyyy-MM-dd HH:mm:ss'))"
Write-InfoLine "考试时长：$Duration 分钟"
Write-InfoLine "工作目录：$workDir"
Write-Host ''
Write-Title '--- 题目列表 ---'
foreach ($p in @($info.problems)) {
    Write-Host ("  T{0}  {1}  （{2}，难度 {3}）" -f $p.number, $p.name, $p.source, $p.difficulty) -ForegroundColor White
}
Write-Host ''
Write-OkLine '提示：请在各题目录下编辑 solution.cpp，使用 mock-exam\submit.ps1 提交评测。'
Write-WarnLine '每 30 分钟将提醒一次剩余时间；按 Ctrl+C 可提前结束考试。'
Write-Host ''

Add-Type -AssemblyName System.Windows.Forms

$nextReminder = $startTime.AddMinutes(30)
$endedNormally = $false

try {
    while ((Get-Date) -lt $endTime) {
        Start-Sleep -Seconds 15
        $now = Get-Date
        if ($now -ge $nextReminder -and $now -lt $endTime) {
            $remainMin = [math]::Ceiling(($endTime - $now).TotalMinutes)
            if ($remainMin -lt 0) { $remainMin = 0 }
            Write-WarnLine "【时间提醒】距离考试结束还剩约 $remainMin 分钟。"
            $nextReminder = $nextReminder.AddMinutes(30)
        }
    }
    $endedNormally = $true
    [void][System.Windows.Forms.MessageBox]::Show(
        "考试时间已到。`n`n工作目录：`n$workDir",
        '模拟考试',
        'OK',
        'Information'
    )
}
catch [System.Management.Automation.PipelineStoppedException] {
    [void][System.Windows.Forms.MessageBox]::Show(
        "考试已提前结束（Ctrl+C）。`n`n工作目录：`n$workDir",
        '模拟考试',
        'OK',
        'Warning'
    )
}
catch [System.Management.Automation.OperationStoppedException] {
    [void][System.Windows.Forms.MessageBox]::Show(
        "考试已提前结束（Ctrl+C）。`n`n工作目录：`n$workDir",
        '模拟考试',
        'OK',
        'Warning'
    )
}
finally {
    if ($endedNormally) {
        Write-OkLine '考试已正常结束。'
    }
    else {
        Write-WarnLine '考试计时已停止（可能为提前结束）。'
    }
}

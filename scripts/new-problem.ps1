#Requires -Version 5.1
<#
.SYNOPSIS
    在 problems 下创建题目目录与脚手架文件。
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string] $Session,

    [Parameter(Mandatory = $true)]
    [ValidateRange(1, 5)]
    [int] $Number,

    [Parameter(Mandatory = $true)]
    [string] $Name
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

function Write-Ok { param([string]$Msg) Write-Host $Msg -ForegroundColor Green }
function Write-Bad { param([string]$Msg) Write-Host $Msg -ForegroundColor Red }
function Write-WarnMsg { param([string]$Msg) Write-Host $Msg -ForegroundColor Yellow }
function Write-Info { param([string]$Msg) Write-Host $Msg -ForegroundColor Cyan }

try {
    $invalid = [System.IO.Path]::GetInvalidFileNameChars()
    $safeName = $Name
    foreach ($ch in $invalid) {
        $safeName = $safeName.Replace([string]$ch, '_')
    }
    $safeName = $safeName.Trim()
    if ([string]::IsNullOrWhiteSpace($safeName)) {
        Write-Bad "错误：题目名称在清理无效字符后为空。"
        exit 1
    }
    if ($safeName -ne $Name) {
        Write-WarnMsg "题目名称已清理非法文件名字符，目录名将使用：$safeName"
    }

    $scriptsDir = $PSScriptRoot
    if ([string]::IsNullOrEmpty($scriptsDir) -and $PSCommandPath) {
        $scriptsDir = [System.IO.Path]::GetDirectoryName($PSCommandPath)
    }
    if ([string]::IsNullOrEmpty($scriptsDir)) {
        Write-Bad "错误：无法确定 scripts 目录位置。"
        exit 1
    }
    $repoRoot = [System.IO.Path]::GetFullPath((Join-Path $scriptsDir '..'))
    if ([string]::IsNullOrEmpty($repoRoot)) {
        Write-Bad "错误：无法确定仓库根目录。"
        exit 1
    }
    $sessionDir = Join-Path $repoRoot (Join-Path 'problems' $Session)
    $folderName = 't{0}-{1}' -f $Number, $safeName
    $problemDir = Join-Path $sessionDir $folderName

    if (Test-Path -LiteralPath $problemDir) {
        Write-WarnMsg "目录已存在，为避免覆盖已中止："
        Write-Host $problemDir
        exit 2
    }

    $templateCpp = Join-Path $repoRoot (Join-Path 'templates' 'solution.cpp')
    if (-not (Test-Path -LiteralPath $templateCpp -PathType Leaf)) {
        Write-Bad "错误：找不到模板文件：$templateCpp"
        exit 1
    }

    New-Item -ItemType Directory -Path $problemDir -Force | Out-Null
    $testDir = Join-Path $problemDir 'testcases'
    New-Item -ItemType Directory -Path $testDir -Force | Out-Null

    Copy-Item -LiteralPath $templateCpp -Destination (Join-Path $problemDir 'solution.cpp') -Force

    $problemMdLines = @(
        '# __CSP_NAME__',
        '',
        '**期次**：__CSP_SESSION__  ',
        '**题号**：T__CSP_NUMBER__  ',
        '',
        '## 题目描述',
        '',
        '（在此填写题目描述）',
        '',
        '## 输入格式',
        '',
        '## 输出格式',
        '',
        '## 样例',
        '',
        '### 输入',
        '',
        '```',
        '```',
        '',
        '### 输出',
        '',
        '```',
        '```',
        '',
        '## 数据范围与提示',
        '',
        '## 来源 / 链接',
        ''
    )
    $problemMd = ($problemMdLines -join "`r`n").Replace('__CSP_NAME__', $Name).Replace('__CSP_SESSION__', $Session).Replace('__CSP_NUMBER__', [string]$Number)

    $utf8NoBom = New-Object System.Text.UTF8Encoding $false
    [System.IO.File]::WriteAllText((Join-Path $problemDir 'problem.md'), $problemMd, $utf8NoBom)
    [System.IO.File]::WriteAllText((Join-Path $problemDir 'notes.md'), "# 解题笔记`r`n`r`n", $utf8NoBom)
    [System.IO.File]::WriteAllText((Join-Path $testDir '1.in'), '', $utf8NoBom)
    [System.IO.File]::WriteAllText((Join-Path $testDir '1.out'), '', $utf8NoBom)

    Write-Ok "已创建题目目录："
    Write-Host $problemDir
    Write-Info "已生成：solution.cpp、problem.md、notes.md、testcases/1.in、testcases/1.out"
}
catch {
    $failDetail = [string]$_.Exception.Message
    Write-Bad ('创建失败：' + $failDetail)
    if ($_.InvocationInfo.PositionMessage) {
        Write-Host $_.InvocationInfo.PositionMessage -ForegroundColor DarkGray
    }
    exit 1
}

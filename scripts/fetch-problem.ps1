#Requires -Version 5.1
<#
.SYNOPSIS
    Fetch problem from Luogu and create local directory with problem description, sample test cases, and solution template.
.EXAMPLE
    .\fetch-problem.ps1 P1162
    .\fetch-problem.ps1 P5660 -Force
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string] $ProblemId,

    [string] $TargetDir,

    [switch] $Force
)

$ErrorActionPreference = 'Stop'

$ScriptsDir = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$RepoRoot = Split-Path -Parent $ScriptsDir

function Write-Info { param([string]$M) Write-Host $M -ForegroundColor Cyan }
function Write-Ok { param([string]$M) Write-Host $M -ForegroundColor Green }
function Write-Bad { param([string]$M) Write-Host $M -ForegroundColor Red }
function Write-Warn { param([string]$M) Write-Host $M -ForegroundColor Yellow }

$ProblemId = $ProblemId.Trim()
if ($ProblemId -notmatch '^[Pp]\d+$') {
    Write-Bad "Invalid Luogu problem ID: $ProblemId (expected format: P1162)"
    exit 1
}
$probPid = $ProblemId.ToUpper()

$url = "https://www.luogu.com.cn/problem/${probPid}?_contentOnly=1"
Write-Info "Fetching $probPid from Luogu..."

try {
    $ProgressPreference = 'SilentlyContinue'
    [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
    $resp = Invoke-WebRequest -Uri $url -Headers @{
        'User-Agent' = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36'
    } -UseBasicParsing -TimeoutSec 30
}
catch {
    Write-Bad "Network error: $($_.Exception.Message)"
    exit 1
}

$htmlContent = $resp.Content

$parsed = $null
$scriptMatches = [regex]::Matches($htmlContent, '<script[^>]*>([\s\S]*?)</script>', 'Singleline')
foreach ($sm in $scriptMatches) {
    $sc = $sm.Groups[1].Value.Trim()
    if ($sc -match '"instance"\s*:\s*"main"' -and $sc -match '"problem"') {
        try { $parsed = $sc | ConvertFrom-Json } catch { }
        break
    }
}

if ($null -eq $parsed -or $null -eq $parsed.data -or $null -eq $parsed.data.problem) {
    Write-Bad "Problem $probPid not found on Luogu or failed to parse response."
    exit 1
}

$problem = $parsed.data.problem
$title = [string]$problem.title
$difficulty = 0
try { $difficulty = [int]$problem.difficulty } catch { }
$samples = $problem.samples

$description = ''
$inputFormat = ''
$outputFormat = ''
$hint = ''

$contentArr = $problem.content
if ($null -ne $contentArr) {
    $zhContent = $null
    if ($contentArr -is [array]) {
        foreach ($c in $contentArr) {
            if ($null -ne $c -and $c.locale -eq 'zh-CN') { $zhContent = $c; break }
        }
        if ($null -eq $zhContent -and $contentArr.Count -gt 0) { $zhContent = $contentArr[0] }
    }
    elseif ($contentArr -is [PSCustomObject]) {
        $zhContent = $contentArr
    }

    if ($null -ne $zhContent) {
        try { $description = [string]$zhContent.description } catch { }
        try { $inputFormat = [string]$zhContent.formatI } catch { }
        try { $outputFormat = [string]$zhContent.formatO } catch { }
        try { $hint = [string]$zhContent.hint } catch { }
        if (-not [string]::IsNullOrWhiteSpace($zhContent.name)) {
            $title = [string]$zhContent.name
        }
    }
}

if ([string]::IsNullOrWhiteSpace($description)) {
    $translations = $parsed.data.translations
    if ($null -ne $translations) {
        $zhTrans = $null
        try { $zhTrans = $translations.'zh-CN' } catch { }
        if ($null -ne $zhTrans) {
            try { $description = [string]$zhTrans.description } catch { }
            try { $inputFormat = [string]$zhTrans.formatI } catch { }
            try { $outputFormat = [string]$zhTrans.formatO } catch { }
            try { $hint = [string]$zhTrans.hint } catch { }
        }
    }
}

$safeName = $title
$invalid = [System.IO.Path]::GetInvalidFileNameChars()
foreach ($ch in $invalid) { $safeName = $safeName.Replace([string]$ch, '_') }
$safeName = $safeName.Trim()
if ([string]::IsNullOrWhiteSpace($safeName)) { $safeName = $probPid }

if ([string]::IsNullOrEmpty($TargetDir)) {
    $luoguDir = Join-Path $RepoRoot 'problems\luogu'
    $TargetDir = Join-Path $luoguDir "${probPid}-${safeName}"
}

if ((Test-Path -LiteralPath $TargetDir) -and -not $Force) {
    Write-Warn "Already exists, skipping: $TargetDir"
    return
}

New-Item -ItemType Directory -Path $TargetDir -Force | Out-Null
$testDir = Join-Path $TargetDir 'testcases'
New-Item -ItemType Directory -Path $testDir -Force | Out-Null

$utf8NoBom = New-Object System.Text.UTF8Encoding $false

$diffNames = @('N/A', 'Entry', 'Easy', 'Easy/Mid', 'Mid', 'Mid/Hard', 'Hard', 'Expert')
$diffName = if ($difficulty -ge 0 -and $difficulty -lt $diffNames.Length) { $diffNames[$difficulty] } else { 'Unknown' }

$md = New-Object System.Text.StringBuilder
[void]$md.AppendLine("# ${probPid} ${title}")
[void]$md.AppendLine("")
[void]$md.AppendLine("**Source**: Luogu  ")
[void]$md.AppendLine("**Difficulty**: ${diffName}  ")
[void]$md.AppendLine("**Link**: https://www.luogu.com.cn/problem/${probPid}  ")
[void]$md.AppendLine("")

if (-not [string]::IsNullOrWhiteSpace($description)) {
    [void]$md.AppendLine("## Problem Description")
    [void]$md.AppendLine("")
    [void]$md.AppendLine($description)
    [void]$md.AppendLine("")
}

if (-not [string]::IsNullOrWhiteSpace($inputFormat)) {
    [void]$md.AppendLine("## Input Format")
    [void]$md.AppendLine("")
    [void]$md.AppendLine($inputFormat)
    [void]$md.AppendLine("")
}

if (-not [string]::IsNullOrWhiteSpace($outputFormat)) {
    [void]$md.AppendLine("## Output Format")
    [void]$md.AppendLine("")
    [void]$md.AppendLine($outputFormat)
    [void]$md.AppendLine("")
}

$sampleCount = 0
if ($null -ne $samples -and $samples.Count -gt 0) {
    foreach ($sample in $samples) {
        $sampleCount++
        [void]$md.AppendLine("## Sample $sampleCount")
        [void]$md.AppendLine("")
        [void]$md.AppendLine("### Input")
        [void]$md.AppendLine("")
        [void]$md.AppendLine('```')
        [void]$md.AppendLine($sample[0])
        [void]$md.AppendLine('```')
        [void]$md.AppendLine("")
        [void]$md.AppendLine("### Output")
        [void]$md.AppendLine("")
        [void]$md.AppendLine('```')
        [void]$md.AppendLine($sample[1])
        [void]$md.AppendLine('```')
        [void]$md.AppendLine("")
    }
}

if (-not [string]::IsNullOrWhiteSpace($hint)) {
    [void]$md.AppendLine("## Hints / Constraints")
    [void]$md.AppendLine("")
    [void]$md.AppendLine($hint)
    [void]$md.AppendLine("")
}

if ([string]::IsNullOrWhiteSpace($description) -and [string]::IsNullOrWhiteSpace($inputFormat)) {
    [void]$md.AppendLine("## Note")
    [void]$md.AppendLine("")
    [void]$md.AppendLine("Full problem description available at: https://www.luogu.com.cn/problem/${probPid}")
    [void]$md.AppendLine("")
}

[System.IO.File]::WriteAllText((Join-Path $TargetDir 'problem.md'), $md.ToString(), $utf8NoBom)

$templateCpp = Join-Path $RepoRoot 'templates\solution.cpp'
if (Test-Path -LiteralPath $templateCpp -PathType Leaf) {
    Copy-Item -LiteralPath $templateCpp -Destination (Join-Path $TargetDir 'solution.cpp') -Force
}
else {
    $fallback = "// ${probPid} ${title}`n#include <bits/stdc++.h>`nusing namespace std;`nint main() {`n    ios::sync_with_stdio(false);`n    cin.tie(nullptr);`n    `n    return 0;`n}`n"
    [System.IO.File]::WriteAllText((Join-Path $TargetDir 'solution.cpp'), $fallback, $utf8NoBom)
}

[System.IO.File]::WriteAllText((Join-Path $TargetDir 'notes.md'), "# Notes`r`n`r`n", $utf8NoBom)

if ($null -ne $samples -and $samples.Count -gt 0) {
    $idx = 0
    foreach ($sample in $samples) {
        $idx++
        $inText = [string]$sample[0]
        $outText = [string]$sample[1]
        if (-not $inText.EndsWith("`n")) { $inText += "`n" }
        if (-not $outText.EndsWith("`n")) { $outText += "`n" }
        [System.IO.File]::WriteAllText((Join-Path $testDir "${idx}.in"), $inText, $utf8NoBom)
        [System.IO.File]::WriteAllText((Join-Path $testDir "${idx}.out"), $outText, $utf8NoBom)
    }
}

Write-Ok "Fetched: ${probPid} ${title}"
Write-Info "  Dir: $TargetDir"
Write-Info "  Samples: $sampleCount"

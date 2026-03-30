#Requires -Version 5.1
<#
.SYNOPSIS
    Batch-fetch all Luogu problems referenced in the study plan.
    Reads data/study-problems.json and calls fetch-problem.ps1 for each Luogu entry.
    Skips problems that already exist locally.
.EXAMPLE
    .\init-study-problems.ps1
#>

$ErrorActionPreference = 'Stop'

$ScriptsDir = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$RepoRoot = Split-Path -Parent $ScriptsDir

function Write-Info  { param([string]$M) Write-Host $M -ForegroundColor Cyan }
function Write-Ok    { param([string]$M) Write-Host $M -ForegroundColor Green }
function Write-Bad   { param([string]$M) Write-Host $M -ForegroundColor Red }
function Write-Warn  { param([string]$M) Write-Host $M -ForegroundColor Yellow }

$planFile = Join-Path $RepoRoot 'data\study-problems.json'
if (-not (Test-Path -LiteralPath $planFile -PathType Leaf)) {
    Write-Bad "Study plan data not found: $planFile"
    exit 1
}

$plan = (Get-Content -LiteralPath $planFile -Raw -Encoding UTF8) | ConvertFrom-Json

$luoguIds = @()
foreach ($wk in $plan.weeks) {
    foreach ($p in $wk.problems) {
        if ($p.id -match '^P\d+$') {
            if ($luoguIds -notcontains $p.id) {
                $luoguIds += $p.id
            }
        }
    }
}

if ($luoguIds.Count -eq 0) {
    Write-Warn 'No Luogu problems found in study plan.'
    exit 0
}

$luoguDir = Join-Path $RepoRoot 'problems\luogu'
if (-not (Test-Path -LiteralPath $luoguDir -PathType Container)) {
    New-Item -ItemType Directory -Path $luoguDir -Force | Out-Null
}

$fetchScript = Join-Path $ScriptsDir 'fetch-problem.ps1'
if (-not (Test-Path -LiteralPath $fetchScript -PathType Leaf)) {
    Write-Bad "fetch-problem.ps1 not found: $fetchScript"
    exit 1
}

$total = $luoguIds.Count
$fetched = 0
$skipped = 0
$failed = 0

Write-Host ''
Write-Info "Fetching $total Luogu problems for study plan..."
Write-Host ''

for ($i = 0; $i -lt $total; $i++) {
    $probId = $luoguIds[$i]
    $num = $i + 1

    $existing = @(Get-ChildItem -LiteralPath $luoguDir -Directory -Filter "${probId}-*" -ErrorAction SilentlyContinue)
    if ($existing.Count -gt 0) {
        Write-Host "  [${num}/${total}] ${probId} ... already exists, skip" -ForegroundColor DarkGray
        $skipped++
        continue
    }

    Write-Host "  [${num}/${total}] ${probId} ... " -NoNewline -ForegroundColor White
    try {
        & $fetchScript -ProblemId $probId 2>&1 | Out-Null
        if ($?) {
            Write-Ok 'OK'
            $fetched++
        }
        else {
            Write-Bad 'FAILED'
            $failed++
        }
    }
    catch {
        Write-Bad "ERROR: $($_.Exception.Message)"
        $failed++
    }

    if ($i -lt ($total - 1)) {
        Start-Sleep -Milliseconds 800
    }
}

Write-Host ''
Write-Info '========== Summary =========='
Write-Ok   "  Fetched : $fetched"
if ($skipped -gt 0) { Write-Host "  Skipped : $skipped" -ForegroundColor DarkGray }
if ($failed -gt 0) { Write-Bad "  Failed  : $failed" }
Write-Host ''

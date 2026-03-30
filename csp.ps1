#Requires -Version 5.1
<#
.SYNOPSIS
    CSP study platform unified CLI.
.EXAMPLE
    .\csp help
    .\csp do P1162
    .\csp test
    .\csp plan
#>

$ErrorActionPreference = 'Stop'

$Command = if ($args.Count -gt 0) { $args[0] } else { 'help' }
$Rest = @()
if ($args.Count -gt 1) {
    $Rest = @($args[1..($args.Count - 1)])
}

$Root = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }

# ── helpers ──

function Write-Info  { param([string]$M) Write-Host $M -ForegroundColor Cyan }
function Write-Ok    { param([string]$M) Write-Host $M -ForegroundColor Green }
function Write-Bad   { param([string]$M) Write-Host $M -ForegroundColor Red }
function Write-Warn  { param([string]$M) Write-Host $M -ForegroundColor Yellow }
function Write-Title { param([string]$M) Write-Host $M -ForegroundColor Magenta }

function Resolve-ProblemDir {
    param([string]$Id)
    if ([string]::IsNullOrWhiteSpace($Id)) { return $null }

    if ($Id -match '^[Pp]\d+$') {
        $probPid = $Id.ToUpper()
        $luoguDir = Join-Path $Root 'problems\luogu'
        if (Test-Path -LiteralPath $luoguDir -PathType Container) {
            $hits = @(Get-ChildItem -LiteralPath $luoguDir -Directory -Filter "${probPid}-*" -ErrorAction SilentlyContinue)
            if ($hits.Count -gt 0) { return $hits[0].FullName }
        }
        return $null
    }

    $norm = $Id -replace '-', '/'
    if ($norm -match '^(\d{6})/t(\d)$') {
        $sess = $Matches[1]; $num = $Matches[2]
        $sessDir = Join-Path $Root "problems\$sess"
        if (Test-Path -LiteralPath $sessDir -PathType Container) {
            $hits = @(Get-ChildItem -LiteralPath $sessDir -Directory -Filter "t${num}-*" -ErrorAction SilentlyContinue)
            if ($hits.Count -gt 0) { return $hits[0].FullName }
        }
        return $null
    }

    if (Test-Path -LiteralPath $Id -PathType Container) { return (Resolve-Path $Id).Path }
    return $null
}

function Get-ProblemIdFromDir {
    param([string]$Dir)
    $rel = $Dir
    if ($Dir.StartsWith($Root, [System.StringComparison]::OrdinalIgnoreCase)) {
        $rel = $Dir.Substring($Root.Length).TrimStart('\', '/')
    }
    if ($rel -match '(?:^|[\\/])luogu[\\/](P\d+)') { return $Matches[1] }
    if ($rel -match '(?:^|problems[\\/])(\d{6})[\\/](t\d)') { return "$($Matches[1])/$($Matches[2])" }
    return $null
}

function Find-SolutionCpp {
    param([string[]]$CliArgs)
    if ($CliArgs.Count -gt 0) {
        $a = $CliArgs[0]
        if (Test-Path -LiteralPath $a -PathType Leaf) { return (Resolve-Path $a).Path }
        $d = Resolve-ProblemDir $a
        if ($null -ne $d) {
            $f = Join-Path $d 'solution.cpp'
            if (Test-Path -LiteralPath $f -PathType Leaf) { return $f }
        }
    }
    $cwd = (Get-Location).Path
    $f = Join-Path $cwd 'solution.cpp'
    if (Test-Path -LiteralPath $f -PathType Leaf) { return $f }
    return $null
}

function Get-ProgressData {
    $pf = Join-Path $Root 'data\progress.json'
    if (Test-Path -LiteralPath $pf -PathType Leaf) {
        try {
            $raw = Get-Content -LiteralPath $pf -Raw -Encoding UTF8
            return ($raw | ConvertFrom-Json)
        } catch { }
    }
    return ([PSCustomObject]@{ problems = [PSCustomObject]@{} })
}

function Save-ProgressData {
    param($Data)
    $pf = Join-Path $Root 'data\progress.json'
    $dir = Split-Path -Parent $pf
    if (-not (Test-Path -LiteralPath $dir)) { New-Item -ItemType Directory -Path $dir -Force | Out-Null }
    $Data | ConvertTo-Json -Depth 10 | Set-Content -LiteralPath $pf -Encoding UTF8
}

function Update-ProblemProgress {
    param([string]$ProbId, [int]$Score)
    if ([string]::IsNullOrWhiteSpace($ProbId)) { return }
    $data = Get-ProgressData
    $now = Get-Date -Format 'yyyy-MM-dd'
    $prev = 0
    try { $prev = [int]$data.problems.$ProbId.score } catch { }
    $best = [math]::Max($prev, $Score)
    $st = if ($best -ge 100) { 'solved' } else { 'attempted' }
    $entry = [PSCustomObject]@{ status = $st; score = $best; lastAttempt = $now }
    $data.problems | Add-Member -NotePropertyName $ProbId -NotePropertyValue $entry -Force
    Save-ProgressData $data
}

function Get-StudyPlan {
    $sf = Join-Path $Root 'data\study-problems.json'
    if (-not (Test-Path -LiteralPath $sf -PathType Leaf)) { return $null }
    try {
        $raw = Get-Content -LiteralPath $sf -Raw -Encoding UTF8
        return ($raw | ConvertFrom-Json)
    } catch { return $null }
}

function Show-Help {
    Write-Host ''
    Write-Title 'CSP Study Platform'
    Write-Host ''
    Write-Host 'Usage:  .\csp <command> [args]' -ForegroundColor White
    Write-Host ''
    Write-Info 'Commands:'
    Write-Host '  init               Setup compiler + fetch all study plan problems'
    Write-Host '  do    <id>         Start a problem (P1162 or 201312/t1, auto-fetch)'
    Write-Host '  test  [id]         Run tests (auto-detect in problem dir)'
    Write-Host '  build [id]         Compile solution.cpp'
    Write-Host '  exam  [set]        Start mock exam'
    Write-Host '  submit <t1-t5>     Submit mock exam answer'
    Write-Host '  plan  [week]       Show study plan progress'
    Write-Host '  next               Recommend next problem'
    Write-Host '  list               List all local problems'
    Write-Host '  fetch <Pxxxx>      Fetch a problem from Luogu'
    Write-Host '  help               Show this help'
    Write-Host ''
    Write-Info 'Examples:'
    Write-Host '  .\csp init             # First-time setup'
    Write-Host '  .\csp plan             # See what to study'
    Write-Host '  .\csp do P1162         # Start Luogu P1162'
    Write-Host '  .\csp test             # Test in problem dir'
    Write-Host '  .\csp test P1162       # Test a specific problem'
    Write-Host '  .\csp exam             # Random mock exam'
    Write-Host '  .\csp submit t1        # Submit problem 1'
    Write-Host ''
    Write-Info 'Tip: add an alias for convenience:'
    Write-Host ('  Set-Alias csp "{0}"' -f (Join-Path $Root 'csp.ps1'))
    Write-Host ''
}

# ── commands ──

switch ($Command) {

    # ─── do ───
    'do' {
        if ($Rest.Count -eq 0) {
            Write-Bad 'Usage: .\csp do <problem-id>'
            Write-Host '  e.g.  .\csp do P1162   or   .\csp do 201312/t1'
            exit 1
        }
        $id = $Rest[0]
        $dir = Resolve-ProblemDir $id

        if ($null -eq $dir -and $id -match '^[Pp]\d+$') {
            $fetchScript = Join-Path $Root 'scripts\fetch-problem.ps1'
            Write-Info "Not found locally, fetching from Luogu..."
            & $fetchScript -ProblemId $id
            if (-not $?) { exit 1 }
            $dir = Resolve-ProblemDir $id
        }

        if ($null -eq $dir) {
            Write-Bad "Problem not found: $id"
            exit 1
        }

        Write-Host ''
        Write-Ok "Problem directory: $dir"
        Write-Host ''
        $pmf = Join-Path $dir 'problem.md'
        if (Test-Path -LiteralPath $pmf -PathType Leaf) {
            $lines = @(Get-Content -LiteralPath $pmf -Encoding UTF8 -TotalCount 8)
            foreach ($l in $lines) { Write-Host "  $l" -ForegroundColor Gray }
            if ($lines.Count -ge 8) { Write-Host '  ...' -ForegroundColor DarkGray }
        }
        Write-Host ''
        $tcDir = Join-Path $dir 'testcases'
        $tcCount = 0
        if (Test-Path -LiteralPath $tcDir) {
            $tcCount = @(Get-ChildItem -LiteralPath $tcDir -Filter '*.in' -File -ErrorAction SilentlyContinue).Count
        }
        Write-Info "  solution.cpp  -> edit your solution"
        Write-Info "  testcases/    -> $tcCount test case(s)"
        Write-Host ''
        $dispId = if ($id -match '^[Pp]\d+$') { $id.ToUpper() } else { $id }
        Write-Info "After editing, run:  .\csp test $dispId"
        Write-Host ''
    }

    # ─── test ───
    'test' {
        $cpp = Find-SolutionCpp $Rest
        if ($null -eq $cpp) {
            Write-Bad 'Usage: .\csp test [problem-id or path]'
            Write-Host '  Run inside a problem directory, or specify an id like P1162'
            exit 1
        }

        $summaryFile = [System.IO.Path]::GetTempFileName()
        $runScript = Join-Path $Root 'scripts\run.ps1'
        & $runScript -CppPath $cpp -SummaryPath $summaryFile

        if (Test-Path -LiteralPath $summaryFile -PathType Leaf) {
            $sumText = [System.IO.File]::ReadAllText($summaryFile, [System.Text.Encoding]::UTF8)
            Remove-Item -LiteralPath $summaryFile -ErrorAction SilentlyContinue
            $score = 0
            foreach ($line in ($sumText -split "`r?`n")) {
                if ($line.Trim() -match '^SCORE=(\d+)$') { $score = [int]$Matches[1] }
            }
            $probDir = Split-Path -Parent (Resolve-Path $cpp).Path
            $probId = Get-ProblemIdFromDir $probDir
            Update-ProblemProgress -ProbId $probId -Score $score
        }
    }

    # ─── build ───
    'build' {
        $cpp = Find-SolutionCpp $Rest
        if ($null -eq $cpp) {
            Write-Bad 'Usage: .\csp build [problem-id or path]'
            exit 1
        }
        $buildScript = Join-Path $Root 'scripts\build.ps1'
        & $buildScript -CppPath $cpp
    }

    # ─── exam ───
    'exam' {
        $examScript = Join-Path $Root 'mock-exam\start-exam.ps1'
        if ($Rest.Count -gt 0) {
            & $examScript -Set $Rest[0]
        }
        else {
            & $examScript
        }
    }

    # ─── submit ───
    'submit' {
        if ($Rest.Count -eq 0) {
            Write-Bad 'Usage: .\csp submit <t1-t5>'
            exit 1
        }
        $target = $Rest[0]
        $submitScript = Join-Path $Root 'mock-exam\submit.ps1'

        if ($target -match '^t[1-5]$') {
            $practiceDir = Join-Path $Root 'mock-exam\practice'
            $latest = $null
            if (Test-Path -LiteralPath $practiceDir -PathType Container) {
                $latest = Get-ChildItem -LiteralPath $practiceDir -Directory -Filter 'exam-*' -ErrorAction SilentlyContinue |
                    Sort-Object Name -Descending | Select-Object -First 1
            }
            if ($null -eq $latest) {
                Write-Bad 'No active mock exam found. Run: .\csp exam'
                exit 1
            }
            $solPath = Join-Path $latest.FullName "$target\solution.cpp"
            if (-not (Test-Path -LiteralPath $solPath -PathType Leaf)) {
                Write-Bad "solution.cpp not found: $solPath"
                exit 1
            }
            & $submitScript -Problem $solPath
        }
        else {
            & $submitScript -Problem $target
        }
    }

    # ─── fetch ───
    'fetch' {
        if ($Rest.Count -eq 0) {
            Write-Bad 'Usage: .\csp fetch <P-number>'
            Write-Host '  e.g.  .\csp fetch P1162'
            exit 1
        }
        $fetchScript = Join-Path $Root 'scripts\fetch-problem.ps1'
        & $fetchScript -ProblemId $Rest[0]
    }

    # ─── plan ───
    'plan' {
        $plan = Get-StudyPlan
        if ($null -eq $plan) {
            Write-Bad 'Study plan data not found. Run: .\csp init'
            exit 1
        }
        $progress = Get-ProgressData
        $weekFilter = if ($Rest.Count -gt 0) { $Rest[0] } else { $null }

        Write-Host ''
        Write-Title '========== CSP Study Plan =========='
        Write-Host ''

        $globalSolved = 0; $globalTotal = 0

        foreach ($wk in $plan.weeks) {
            if ($null -ne $weekFilter -and $wk.id -notlike "*${weekFilter}*" -and $wk.name -notlike "*${weekFilter}*") { continue }

            $wkSolved = 0
            Write-Title "$($wk.name)  ($($wk.description))"

            foreach ($p in $wk.problems) {
                $st = $null
                try { $st = $progress.problems.($p.id) } catch { }

                $mark = '[ ]'; $color = 'White'; $extra = ''
                if ($null -ne $st) {
                    if ($st.status -eq 'solved') {
                        $mark = '[x]'; $color = 'Green'; $extra = " ($($st.score)pts)"
                        $wkSolved++
                    }
                    else {
                        $mark = '[~]'; $color = 'Yellow'; $extra = " ($($st.score)pts)"
                    }
                }
                $src = if ($p.source -eq 'CSP') { 'CSP' } else { 'LG' }
                Write-Host "  $mark $($p.id) $($p.name)  [$($p.tags)] {$src}$extra" -ForegroundColor $color
            }

            $cnt = $wk.problems.Count
            $pct = if ($cnt -gt 0) { [math]::Round(100 * $wkSolved / $cnt) } else { 0 }
            Write-Info "  Progress: ${wkSolved}/${cnt} (${pct}%)"
            Write-Host ''
            $globalSolved += $wkSolved; $globalTotal += $cnt
        }

        $gPct = if ($globalTotal -gt 0) { [math]::Round(100 * $globalSolved / $globalTotal) } else { 0 }
        Write-Title "Total: ${globalSolved}/${globalTotal} (${gPct}%)"
        Write-Host ''
    }

    # ─── next ───
    'next' {
        $plan = Get-StudyPlan
        if ($null -eq $plan) {
            Write-Bad 'Study plan data not found. Run: .\csp init'
            exit 1
        }
        $progress = Get-ProgressData
        $found = $false

        foreach ($wk in $plan.weeks) {
            foreach ($p in $wk.problems) {
                $st = $null
                try { $st = $progress.problems.($p.id) } catch { }
                if ($null -eq $st -or $st.status -ne 'solved') {
                    Write-Host ''
                    Write-Ok 'Recommended next:'
                    Write-Info "  $($p.id)  $($p.name)"
                    Write-Host "  Week : $($wk.name)" -ForegroundColor Gray
                    Write-Host "  Tags : $($p.tags)" -ForegroundColor Gray
                    Write-Host ''
                    Write-Info "Run:  .\csp do $($p.id)"
                    Write-Host ''
                    $found = $true
                    break
                }
            }
            if ($found) { break }
        }
        if (-not $found) {
            Write-Ok 'All study plan problems solved! Congratulations!'
        }
    }

    # ─── list ───
    'list' {
        Write-Host ''
        Write-Title '========== Problem List =========='
        Write-Host ''

        $problemsDir = Join-Path $Root 'problems'

        $cspDirs = @(Get-ChildItem -LiteralPath $problemsDir -Directory -ErrorAction SilentlyContinue |
            Where-Object { $_.Name -ne 'luogu' } | Sort-Object Name)
        if ($cspDirs.Count -gt 0) {
            Write-Title 'CSP Official:'
            foreach ($s in $cspDirs) {
                $probs = @(Get-ChildItem -LiteralPath $s.FullName -Directory -ErrorAction SilentlyContinue | Sort-Object Name)
                foreach ($p in $probs) {
                    Write-Host "  $($s.Name)/$($p.Name)" -ForegroundColor White
                }
            }
            Write-Host ''
        }

        $luoguDir = Join-Path $problemsDir 'luogu'
        if (Test-Path -LiteralPath $luoguDir -PathType Container) {
            $lps = @(Get-ChildItem -LiteralPath $luoguDir -Directory -ErrorAction SilentlyContinue | Sort-Object Name)
            if ($lps.Count -gt 0) {
                Write-Title 'Luogu:'
                foreach ($p in $lps) {
                    Write-Host "  $($p.Name)" -ForegroundColor White
                }
                Write-Host ''
            }
        }

        if ($cspDirs.Count -eq 0) {
            $lc = 0; try { $lc = $lps.Count } catch { }
            if ($lc -eq 0) {
                Write-Warn 'No problems found. Run: .\csp init'
            }
        }
    }

    # ─── init ───
    'init' {
        Write-Host ''
        Write-Title '========== CSP Platform Init =========='
        Write-Host ''

        Write-Info '[1/2] Checking C++ compiler...'
        if (Get-Command g++ -ErrorAction SilentlyContinue) {
            Write-Ok '  g++ found.'
        }
        else {
            $setupScript = Join-Path $Root 'setup-compiler.ps1'
            if (Test-Path -LiteralPath $setupScript -PathType Leaf) {
                & $setupScript
            }
            else {
                Write-Warn '  g++ not found. Please install MinGW-w64 manually.'
            }
        }
        Write-Host ''

        Write-Info '[2/2] Fetching study plan problems...'
        $initScript = Join-Path $Root 'scripts\init-study-problems.ps1'
        if (Test-Path -LiteralPath $initScript -PathType Leaf) {
            & $initScript
        }
        else {
            Write-Bad '  init-study-problems.ps1 not found.'
        }

        Write-Host ''
        Write-Ok 'Init complete! Run: .\csp plan'
        Write-Host ''
    }

    # ─── help / default ───
    'help' { Show-Help }
    default { Show-Help }
}

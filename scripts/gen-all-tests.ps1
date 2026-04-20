#Requires -Version 5.1
<#
.SYNOPSIS
    一键批量生成/验证所有题目的测试数据。
    遍历 problems/ 和 mock-exam/exam-sets/ 下所有包含 gen.cpp 的题目目录，
    调用 gen-tests.ps1 生成 10 组测试数据。
    也可以仅验证（不重新生成）现有测试数据的完整性。
.EXAMPLE
    .\gen-all-tests.ps1              # 仅验证
    .\gen-all-tests.ps1 -Generate    # 重新生成（需要 g++）
#>
param(
    [switch] $Generate
)

$ErrorActionPreference = 'Stop'

$ScriptsDir = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$StudyRoot = Split-Path -Parent -Path $ScriptsDir
$GenTestsScript = Join-Path $ScriptsDir 'gen-tests.ps1'

function Write-Info { param([string]$Msg) Write-Host $Msg -ForegroundColor Cyan }
function Write-Ok { param([string]$Msg) Write-Host $Msg -ForegroundColor Green }
function Write-Bad { param([string]$Msg) Write-Host $Msg -ForegroundColor Red }
function Write-WarnMsg { param([string]$Msg) Write-Host $Msg -ForegroundColor Yellow }

# 收集所有题目目录（包含 testcases/ 的目录）
$problemDirs = @()

# problems/
$problemsRoot = Join-Path $StudyRoot 'problems'
if (Test-Path $problemsRoot) {
    Get-ChildItem -Path $problemsRoot -Recurse -Directory -Filter 'testcases' | ForEach-Object {
        $problemDirs += $_.Parent.FullName
    }
}

# mock-exam/exam-sets/
$examSetsRoot = Join-Path $StudyRoot 'mock-exam\exam-sets'
if (Test-Path $examSetsRoot) {
    Get-ChildItem -Path $examSetsRoot -Recurse -Directory -Filter 'testcases' | ForEach-Object {
        $problemDirs += $_.Parent.FullName
    }
}

$problemDirs = $problemDirs | Sort-Object -Unique

Write-Host ''
Write-Info '========== CSP 测试数据验证 =========='
Write-Info "扫描到 $($problemDirs.Count) 个题目目录"
Write-Host ''

$totalProblems = 0
$completeProblems = 0
$incompleteProblems = 0
$report = @()

foreach ($dir in $problemDirs) {
    $totalProblems++
    $relPath = $dir.Replace($StudyRoot, '').TrimStart('\')
    $testDir = Join-Path $dir 'testcases'

    $inCount = 0
    $outCount = 0
    $pairedCount = 0

    if (Test-Path $testDir) {
        $inFiles = @(Get-ChildItem -LiteralPath $testDir -Filter '*.in' -File)
        $outFiles = @(Get-ChildItem -LiteralPath $testDir -Filter '*.out' -File)
        $inCount = $inFiles.Count
        $outCount = $outFiles.Count

        foreach ($inFile in $inFiles) {
            $base = [System.IO.Path]::GetFileNameWithoutExtension($inFile.Name)
            $matchOut = Join-Path $testDir "$base.out"
            if (Test-Path -LiteralPath $matchOut -PathType Leaf) {
                $pairedCount++
            }
        }
    }

    $hasGen = Test-Path -LiteralPath (Join-Path $dir 'gen.cpp') -PathType Leaf
    $hasSol = Test-Path -LiteralPath (Join-Path $dir 'solution.cpp') -PathType Leaf
    $hasBrute = Test-Path -LiteralPath (Join-Path $dir 'brute.cpp') -PathType Leaf

    $status = ''
    $color = 'White'

    if ($pairedCount -ge 10) {
        $status = 'OK (' + $pairedCount + ')'
        $color = 'Green'
        $completeProblems++
    }
    elseif ($pairedCount -gt 0) {
        $status = $pairedCount.ToString() + '/10'
        $color = 'Yellow'
        $incompleteProblems++
    }
    else {
        $status = 'EMPTY'
        $color = 'Red'
        $incompleteProblems++
    }

    $genFlag = if ($hasGen) { '[gen]' } else { '' }
    $solFlag = if ($hasSol) { '[sol]' } else { '' }
    $bruteFlag = if ($hasBrute) { '[brute]' } else { '' }
    $flags = "$genFlag$solFlag$bruteFlag".Trim()

    Write-Host "  $relPath" -NoNewline -ForegroundColor White
    Write-Host " $status" -NoNewline -ForegroundColor $color
    Write-Host " $flags" -ForegroundColor DarkGray

    if ($hasGen -and -not $hasBrute) {
        Write-WarnMsg "    ⚠ 无 brute.cpp：.out 由 solution.cpp 生成，可能不可靠"
    }

    $report += [PSCustomObject]@{
        Path     = $relPath
        InFiles  = $inCount
        OutFiles = $outCount
        Paired   = $pairedCount
        HasGen   = $hasGen
        HasSol   = $hasSol
        Status   = $status
    }
}

Write-Host ''
Write-Info '========== 汇总 =========='
Write-Ok ('  Complete (>=10): ' + $completeProblems)
if ($incompleteProblems -gt 0) {
    Write-WarnMsg ('  Incomplete (<10): ' + $incompleteProblems)
}
Write-Info ('  Total: ' + $totalProblems)

# 如果指定了 -Generate，对有 gen.cpp 的题目重新生成
if ($Generate) {
    Write-Host ''
    Write-Info '========== 重新生成测试数据 =========='

    if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
        Write-Bad "错误：未找到 g++，无法生成测试数据。请先运行 setup-compiler.ps1。"
        exit 1
    }

    if (-not (Test-Path $GenTestsScript)) {
        Write-Bad "错误：找不到 gen-tests.ps1：$GenTestsScript"
        exit 1
    }

    # 定义每个题目的 MaxN 参数
    $maxNMap = @{
        'problems\201312\t1' = 1000
        'problems\201312\t2' = 10
        'problems\201503\t1' = 100
        'problems\201604\t1' = 200
        'problems\201612\t1' = 1000
        'problems\202104\t2' = 100
    }

    $genDirs = $problemDirs | Where-Object {
        Test-Path -LiteralPath (Join-Path $_ 'gen.cpp') -PathType Leaf
    }

    $genCount = 0
    $warnCount = 0

    foreach ($dir in $genDirs) {
        $relPath = $dir.Replace($StudyRoot, '').TrimStart('\')
        $maxN = 100

        foreach ($key in $maxNMap.Keys) {
            if ($relPath -like "*$key*") {
                $maxN = $maxNMap[$key]
                break
            }
        }

        $hasBrute = Test-Path -LiteralPath (Join-Path $dir 'brute.cpp') -PathType Leaf
        if (-not $hasBrute) {
            Write-WarnMsg "  ⚠ $relPath 无 brute.cpp，生成的 .out 可能不可靠"
            $warnCount++
        }

        Write-Info ('  Generate: ' + $relPath + ' MaxN=' + $maxN)
        & $GenTestsScript -ProblemDir $dir -MaxN $maxN

        # 样例 sanity check：用 problem.md 中的样例验证标程输出
        $problemMd = Join-Path $dir 'problem.md'
        $testDir = Join-Path $dir 'testcases'
        $sampleIn = Join-Path $testDir '1.in'
        $sampleOut = Join-Path $testDir '1.out'
        if ((Test-Path -LiteralPath $sampleIn) -and (Test-Path -LiteralPath $sampleOut)) {
            $outContent = [System.IO.File]::ReadAllText($sampleOut, [System.Text.Encoding]::UTF8).Trim()
            if ([string]::IsNullOrWhiteSpace($outContent)) {
                Write-WarnMsg "    ⚠ 1.out 为空，标程可能有问题"
                $warnCount++
            }
        }

        $genCount++
        Write-Host ''
    }

    Write-Host ''
    Write-Ok "生成完成：$genCount 个题目"
    if ($warnCount -gt 0) {
        Write-WarnMsg "警告：$warnCount 个题目需要注意（缺少 brute.cpp 或输出异常）"
    }
}

Write-Host ''

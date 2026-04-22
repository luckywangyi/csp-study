#Requires -Version 5.1
<#
.SYNOPSIS
    CSP 测试数据生成器：编译 gen.cpp + brute.cpp（或 solution.cpp），
    自动生成 N 组分级测试数据（.in + .out）。
.DESCRIPTION
    测试点分级（模拟 CSP 真实考试数据分布）：
      点 1~2  ：极小数据（边界，n_max 约为上界的 1%~5%）
      点 3~5  ：小数据（n_max 约为上界的 10%~30%）
      点 6~8  ：中等数据（n_max 约为上界的 50%~70%）
      点 9~10 ：极限数据（n_max = 上界）
.EXAMPLE
    .\gen-tests.ps1 -ProblemDir .\problems\201312\t1-出现次数最多的数 -MaxN 1000
    .\gen-tests.ps1 -ProblemDir .\problems\202104\t2-邻域均值 -MaxN 600 -Count 10
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string] $ProblemDir,

    [Parameter(Mandatory = $true)]
    [int] $MaxN,

    [int] $Count = 10,

    [string] $GenArgs = '',

    [switch] $KeepOld
)

$ErrorActionPreference = 'Stop'

function Write-Info { param([string]$Msg) Write-Host $Msg -ForegroundColor Cyan }
function Write-Ok { param([string]$Msg) Write-Host $Msg -ForegroundColor Green }
function Write-Bad { param([string]$Msg) Write-Host $Msg -ForegroundColor Red }
function Write-WarnMsg { param([string]$Msg) Write-Host $Msg -ForegroundColor Yellow }

$resolved = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($ProblemDir)
if (-not (Test-Path -LiteralPath $resolved -PathType Container)) {
    Write-Bad "错误：题目目录不存在：$ProblemDir"
    exit 1
}

$genCpp = Join-Path $resolved 'gen.cpp'
if (-not (Test-Path -LiteralPath $genCpp -PathType Leaf)) {
    Write-Bad "错误：找不到 gen.cpp：$genCpp"
    exit 1
}

$bruteCpp = Join-Path $resolved 'brute.cpp'
$solutionCpp = Join-Path $resolved 'solution.cpp'
$stdCpp = ''
if (Test-Path -LiteralPath $bruteCpp -PathType Leaf) {
    $stdCpp = $bruteCpp
}
elseif (Test-Path -LiteralPath $solutionCpp -PathType Leaf) {
    $stdCpp = $solutionCpp
    Write-WarnMsg "未找到 brute.cpp，将使用 solution.cpp 作为标程生成期望输出。"
}
else {
    Write-Bad "错误：找不到 brute.cpp 或 solution.cpp"
    exit 1
}

if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Bad "错误：未找到 g++。请先运行 setup-compiler.ps1 安装 MinGW-w64。"
    exit 1
}

$gpp = (Get-Command g++ -ErrorAction Stop).Source

$testDir = Join-Path $resolved 'testcases'

$needTempCompile = $resolved -match '[^\x00-\x7F]'
if ($needTempCompile) {
    $tmpCompileDir = Join-Path ([System.IO.Path]::GetTempPath()) "csp_gen_$([System.IO.Path]::GetRandomFileName())"
    New-Item -ItemType Directory -Path $tmpCompileDir -Force | Out-Null
    $genExe = Join-Path $tmpCompileDir 'gen.exe'
    $stdExe = Join-Path $tmpCompileDir 'brute.exe'
    $tmpGenCpp = Join-Path $tmpCompileDir 'gen.cpp'
    $tmpStdCpp = Join-Path $tmpCompileDir 'std.cpp'
    Copy-Item -LiteralPath $genCpp -Destination $tmpGenCpp -Force
    Copy-Item -LiteralPath $stdCpp -Destination $tmpStdCpp -Force
    $compileGenSrc = $tmpGenCpp
    $compileStdSrc = $tmpStdCpp
} else {
    $genExe = Join-Path $resolved 'gen.exe'
    $stdExe = Join-Path $resolved 'brute.exe'
    $compileGenSrc = $genCpp
    $compileStdSrc = $stdCpp
}

Write-Info "========== 测试数据生成 =========="
Write-Info "题目目录：$resolved"
Write-Info "数据生成器：$genCpp"
Write-Info "暴力标程：$stdCpp"
Write-Info "数据规模上界：$MaxN"
Write-Info "测试点数量：$Count"
Write-Host ''

# 编译 gen.cpp
Write-Info "[1/3] 编译数据生成器 gen.cpp ..."
$p = Start-Process -FilePath $gpp -ArgumentList @('-std=c++17', '-O2', '-o', $genExe, $compileGenSrc) `
    -NoNewWindow -Wait -PassThru
if ($p.ExitCode -ne 0) {
    Write-Bad "gen.cpp 编译失败"
    exit 1
}
Write-Ok "  gen.exe 编译成功"

# 编译 brute.cpp / solution.cpp
Write-Info "[2/3] 编译暴力标程 ..."
$p = Start-Process -FilePath $gpp -ArgumentList @('-std=c++17', '-O2', '-o', $stdExe, $compileStdSrc) `
    -NoNewWindow -Wait -PassThru
if ($p.ExitCode -ne 0) {
    Write-Bad "标程编译失败"
    exit 1
}
Write-Ok "  brute.exe 编译成功"

# 准备 testcases 目录
if (-not (Test-Path -LiteralPath $testDir -PathType Container)) {
    New-Item -ItemType Directory -Path $testDir -Force | Out-Null
}
if (-not $KeepOld) {
    Get-ChildItem -LiteralPath $testDir -Filter '*.in' -File | Remove-Item -Force
    Get-ChildItem -LiteralPath $testDir -Filter '*.out' -File | Remove-Item -Force
}

# 计算每个测试点的 n_max（分级）
function Get-TestPointNMax {
    param([int]$Point, [int]$TotalPoints, [int]$GlobalMax)
    if ($GlobalMax -le 5) { return $GlobalMax }
    $ratios = @()
    for ($i = 1; $i -le $TotalPoints; $i++) {
        if ($i -le [math]::Ceiling($TotalPoints * 0.2)) {
            $ratios += [math]::Max(1, [int]([math]::Ceiling($GlobalMax * 0.05 * $i)))
        }
        elseif ($i -le [math]::Ceiling($TotalPoints * 0.5)) {
            $frac = 0.1 + 0.2 * ($i - [math]::Ceiling($TotalPoints * 0.2)) / `
                ([math]::Ceiling($TotalPoints * 0.5) - [math]::Ceiling($TotalPoints * 0.2))
            $ratios += [math]::Max(2, [int]([math]::Ceiling($GlobalMax * $frac)))
        }
        elseif ($i -le [math]::Ceiling($TotalPoints * 0.8)) {
            $frac = 0.5 + 0.3 * ($i - [math]::Ceiling($TotalPoints * 0.5)) / `
                ([math]::Ceiling($TotalPoints * 0.8) - [math]::Ceiling($TotalPoints * 0.5))
            $ratios += [math]::Max(3, [int]([math]::Ceiling($GlobalMax * $frac)))
        }
        else {
            $ratios += $GlobalMax
        }
    }
    return $ratios[$Point - 1]
}

Write-Info "[3/3] 生成 $Count 组测试数据 ..."
Write-Host ''

$okCount = 0

for ($i = 1; $i -le $Count; $i++) {
    $nMax = Get-TestPointNMax -Point $i -TotalPoints $Count -GlobalMax $MaxN
    $seed = $i * 1000 + $nMax
    $inFile = Join-Path $testDir "$i.in"
    $outFile = Join-Path $testDir "$i.out"

    $genArgList = @([string]$seed, [string]$nMax)
    if (-not [string]::IsNullOrWhiteSpace($GenArgs)) {
        $genArgList += ($GenArgs -split '\s+')
    }

    # 运行 gen.exe 生成输入
    $tmpIn = [System.IO.Path]::GetTempFileName()
    $p = Start-Process -FilePath $genExe -ArgumentList $genArgList `
        -RedirectStandardOutput $tmpIn -NoNewWindow -Wait -PassThru
    if ($p.ExitCode -ne 0) {
        Write-Bad "  [#$i] gen.exe 退出码 $($p.ExitCode)（seed=$seed, n_max=$nMax）"
        Remove-Item -LiteralPath $tmpIn -ErrorAction SilentlyContinue
        continue
    }

    Copy-Item -LiteralPath $tmpIn -Destination $inFile -Force

    # 运行 brute.exe 生成输出
    $tmpOut = [System.IO.Path]::GetTempFileName()
    $tmpErr = [System.IO.Path]::GetTempFileName()
    $p = Start-Process -FilePath $stdExe `
        -RedirectStandardInput $tmpIn `
        -RedirectStandardOutput $tmpOut `
        -RedirectStandardError $tmpErr `
        -NoNewWindow -Wait -PassThru

    if ($p.ExitCode -ne 0) {
        $errMsg = ''
        if (Test-Path -LiteralPath $tmpErr) {
            $errMsg = ([System.IO.File]::ReadAllText($tmpErr)).Trim()
        }
        Write-Bad "  [#$i] brute.exe 退出码 $($p.ExitCode)（seed=$seed, n_max=$nMax）"
        if ($errMsg) { Write-Host "    $errMsg" -ForegroundColor DarkYellow }
        Remove-Item -LiteralPath $tmpIn, $tmpOut, $tmpErr -ErrorAction SilentlyContinue
        continue
    }

    Copy-Item -LiteralPath $tmpOut -Destination $outFile -Force
    Remove-Item -LiteralPath $tmpIn, $tmpOut, $tmpErr -ErrorAction SilentlyContinue

    $inSize = (Get-Item -LiteralPath $inFile).Length
    $outSize = (Get-Item -LiteralPath $outFile).Length
    Write-Ok "  [#$i/$Count] n_max=$nMax  seed=$seed  in=${inSize}B  out=${outSize}B"
    $okCount++
}

Write-Host ''
if ($okCount -eq $Count) {
    Write-Ok "全部 $Count 组测试数据生成成功！"
}
else {
    Write-WarnMsg "生成完成：$okCount / $Count 组成功。"
}

# 清理临时可执行文件
Remove-Item -LiteralPath $genExe -ErrorAction SilentlyContinue
Remove-Item -LiteralPath $stdExe -ErrorAction SilentlyContinue
if ($needTempCompile -and (Test-Path -LiteralPath $tmpCompileDir)) {
    Remove-Item -LiteralPath $tmpCompileDir -Recurse -Force -ErrorAction SilentlyContinue
}

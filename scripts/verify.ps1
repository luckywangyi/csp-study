#Requires -Version 5.1
<#
.SYNOPSIS
    对拍验证：用 gen.cpp 生成随机数据，同时运行 brute.cpp 和 solution.cpp，比对输出。
    发现不一致时保存反例数据并报错。
.EXAMPLE
    .\verify.ps1 .\problems\201604\t1-折点计数\solution.cpp
    .\verify.ps1 .\problems\201604\t1-折点计数\solution.cpp -Rounds 200 -MaxN 1000
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string] $CppPath,

    [int] $Rounds = 100,

    [int] $MaxN = 100,

    [double] $TimeLimit = 5.0
)

$ErrorActionPreference = 'Stop'

function Write-Info { param([string]$Msg) Write-Host $Msg -ForegroundColor Cyan }
function Write-Ok { param([string]$Msg) Write-Host $Msg -ForegroundColor Green }
function Write-Bad { param([string]$Msg) Write-Host $Msg -ForegroundColor Red }
function Write-WarnMsg { param([string]$Msg) Write-Host $Msg -ForegroundColor Yellow }

function Normalize-OutputText {
    param([string] $Text)
    if ([string]::IsNullOrEmpty($Text)) { return '' }
    $lines = @($Text -split "`r?`n" | ForEach-Object { if ($null -eq $_) { '' } else { $_.TrimEnd() } })
    $last = $lines.Length - 1
    while ($last -ge 0 -and [string]::IsNullOrWhiteSpace($lines[$last])) { $last-- }
    if ($last -lt 0) { return '' }
    return ($lines[0..$last] -join "`n")
}

try {
    $resolved = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($CppPath)
    if (-not (Test-Path -LiteralPath $resolved -PathType Leaf)) {
        Write-Bad "错误：源文件不存在：$CppPath"
        exit 1
    }

    $srcDir = Split-Path -Parent -Path $resolved
    $genCpp = Join-Path $srcDir 'gen.cpp'
    $bruteCpp = Join-Path $srcDir 'brute.cpp'

    if (-not (Test-Path -LiteralPath $genCpp -PathType Leaf)) {
        Write-Bad "错误：找不到 gen.cpp：$genCpp"
        Write-Bad "对拍需要数据生成器 gen.cpp。"
        exit 1
    }
    if (-not (Test-Path -LiteralPath $bruteCpp -PathType Leaf)) {
        Write-Bad "错误：找不到 brute.cpp：$bruteCpp"
        Write-Bad "对拍需要暴力标程 brute.cpp。"
        exit 1
    }
    if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
        Write-Bad "错误：未找到 g++。"
        exit 1
    }

    $gpp = (Get-Command g++ -ErrorAction Stop).Source
    $tempDir = Join-Path ([System.IO.Path]::GetTempPath()) 'csp_verify'
    if (-not (Test-Path -LiteralPath $tempDir)) {
        New-Item -ItemType Directory -Path $tempDir -Force | Out-Null
    }

    $genExe = Join-Path $tempDir 'gen.exe'
    $bruteExe = Join-Path $tempDir 'brute.exe'
    $solExe = Join-Path $tempDir 'solution.exe'

    $needTempSrc = $srcDir -match '[^\x00-\x7F]'
    if ($needTempSrc) {
        $compileGenSrc = Join-Path $tempDir 'gen.cpp'
        $compileBruteSrc = Join-Path $tempDir 'brute.cpp'
        $compileSolSrc = Join-Path $tempDir 'solution.cpp'
        Copy-Item -LiteralPath $genCpp -Destination $compileGenSrc -Force
        Copy-Item -LiteralPath $bruteCpp -Destination $compileBruteSrc -Force
        Copy-Item -LiteralPath $resolved -Destination $compileSolSrc -Force
    } else {
        $compileGenSrc = $genCpp
        $compileBruteSrc = $bruteCpp
        $compileSolSrc = $resolved
    }

    Write-Host ''
    Write-Info '========== 对拍验证 =========='
    Write-Info "题目目录：$srcDir"
    Write-Info "对拍轮数：$Rounds"
    Write-Info "数据规模上界：$MaxN"
    Write-Host ''

    # 编译三个程序
    Write-Info '[1/4] 编译 gen.cpp ...'
    $p = Start-Process -FilePath $gpp -ArgumentList @('-std=c++17', '-O2', '-o', $genExe, $compileGenSrc) `
        -NoNewWindow -Wait -PassThru
    if ($p.ExitCode -ne 0) { Write-Bad 'gen.cpp 编译失败'; exit 1 }
    Write-Ok '  gen.exe OK'

    Write-Info '[2/4] 编译 brute.cpp ...'
    $p = Start-Process -FilePath $gpp -ArgumentList @('-std=c++17', '-O2', '-o', $bruteExe, $compileBruteSrc) `
        -NoNewWindow -Wait -PassThru
    if ($p.ExitCode -ne 0) { Write-Bad 'brute.cpp 编译失败'; exit 1 }
    Write-Ok '  brute.exe OK'

    Write-Info '[3/4] 编译 solution.cpp ...'
    $p = Start-Process -FilePath $gpp -ArgumentList @('-std=c++17', '-O2', '-o', $solExe, $compileSolSrc) `
        -NoNewWindow -Wait -PassThru
    if ($p.ExitCode -ne 0) { Write-Bad 'solution.cpp 编译失败'; exit 1 }
    Write-Ok '  solution.exe OK'

    Write-Info "[4/4] 开始对拍（$Rounds 轮）..."
    Write-Host ''

    $timeoutMs = [int][math]::Ceiling($TimeLimit * 1000)
    $failDir = Join-Path $srcDir 'verify-fails'

    for ($i = 1; $i -le $Rounds; $i++) {
        $seed = $i
        $tmpIn = Join-Path $tempDir "input.txt"
        $tmpBruteOut = Join-Path $tempDir "brute_out.txt"
        $tmpSolOut = Join-Path $tempDir "sol_out.txt"
        $tmpErr = Join-Path $tempDir "stderr.txt"

        # 生成输入
        $p = Start-Process -FilePath $genExe -ArgumentList @([string]$seed, [string]$MaxN) `
            -RedirectStandardOutput $tmpIn -NoNewWindow -Wait -PassThru
        if ($p.ExitCode -ne 0) {
            Write-Bad "  [#$i] gen.exe 退出码 $($p.ExitCode)"
            continue
        }

        # 运行 brute
        $p = Start-Process -FilePath $bruteExe `
            -RedirectStandardInput $tmpIn -RedirectStandardOutput $tmpBruteOut -RedirectStandardError $tmpErr `
            -NoNewWindow -Wait -PassThru
        if ($p.HasExited) { $p.WaitForExit() }
        if ($p.ExitCode -ne 0) {
            Write-Bad "  [#$i] brute.exe RE (exit=$($p.ExitCode))"
            continue
        }

        # 运行 solution
        $p = Start-Process -FilePath $solExe `
            -RedirectStandardInput $tmpIn -RedirectStandardOutput $tmpSolOut -RedirectStandardError $tmpErr `
            -NoNewWindow -Wait -PassThru
        if ($p.HasExited) { $p.WaitForExit() }
        if ($p.ExitCode -ne 0) {
            Write-Bad "  [#$i] solution.exe RE (exit=$($p.ExitCode))"
            # 保存反例
            if (-not (Test-Path -LiteralPath $failDir)) { New-Item -ItemType Directory -Path $failDir -Force | Out-Null }
            Copy-Item -LiteralPath $tmpIn -Destination (Join-Path $failDir "fail_${i}.in") -Force
            Copy-Item -LiteralPath $tmpBruteOut -Destination (Join-Path $failDir "fail_${i}.expected") -Force
            Write-Bad "  反例已保存到 verify-fails/fail_${i}.in"
            continue
        }

        # 比对
        $bruteText = Normalize-OutputText ([System.IO.File]::ReadAllText($tmpBruteOut, [System.Text.Encoding]::UTF8))
        $solText = Normalize-OutputText ([System.IO.File]::ReadAllText($tmpSolOut, [System.Text.Encoding]::UTF8))

        if ($bruteText -cne $solText) {
            Write-Bad "  [#$i] MISMATCH! seed=$seed"
            $inputPreview = ([System.IO.File]::ReadAllText($tmpIn, [System.Text.Encoding]::UTF8)).Trim()
            if ($inputPreview.Length -gt 200) { $inputPreview = $inputPreview.Substring(0, 200) + '...' }
            Write-Host "    输入：$inputPreview" -ForegroundColor Gray
            Write-Host "    期望：$bruteText" -ForegroundColor Green
            Write-Host "    实际：$solText" -ForegroundColor Red

            if (-not (Test-Path -LiteralPath $failDir)) { New-Item -ItemType Directory -Path $failDir -Force | Out-Null }
            Copy-Item -LiteralPath $tmpIn -Destination (Join-Path $failDir "fail_${i}.in") -Force
            [System.IO.File]::WriteAllText((Join-Path $failDir "fail_${i}.expected"), $bruteText + "`n", [System.Text.UTF8Encoding]::new($false))
            [System.IO.File]::WriteAllText((Join-Path $failDir "fail_${i}.actual"), $solText + "`n", [System.Text.UTF8Encoding]::new($false))
            Write-Bad "  反例已保存到 verify-fails/fail_${i}.*"
            Write-Host ''
            Write-Bad "对拍在第 $i 轮发现错误，已停止。"
            exit 1
        }

        if ($i % 10 -eq 0 -or $i -eq $Rounds) {
            Write-Ok "  [#$i/$Rounds] 通过"
        }
    }

    Write-Host ''
    Write-Ok "全部 $Rounds 轮对拍通过！solution.cpp 与 brute.cpp 输出一致。"
    Write-Host ''
}
catch {
    Write-Bad "对拍过程出错：$_"
    exit 1
}

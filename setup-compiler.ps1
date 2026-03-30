# CSP Study Environment - MinGW-w64 Compiler Setup Script
# Usage: .\setup-compiler.ps1

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  CSP C++ 编译环境配置工具" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

function Test-GppInstalled {
    try {
        $version = & g++ --version 2>$null
        if ($LASTEXITCODE -eq 0) {
            Write-Host "[OK] g++ 已安装:" -ForegroundColor Green
            Write-Host "  $($version[0])" -ForegroundColor Gray
            return $true
        }
    } catch {}
    return $false
}

if (Test-GppInstalled) {
    Write-Host ""
    Write-Host "编译器已就绪，无需重复安装。" -ForegroundColor Green
    exit 0
}

Write-Host "[INFO] 未检测到 g++，开始安装 MinGW-w64 ..." -ForegroundColor Yellow
Write-Host ""

# Method 1: Try winget first
$wingetAvailable = $false
try {
    $null = & winget --version 2>$null
    $wingetAvailable = ($LASTEXITCODE -eq 0)
} catch {}

if ($wingetAvailable) {
    Write-Host "[1/3] 通过 winget 安装 MinGW-w64 ..." -ForegroundColor Cyan
    try {
        & winget install -e --id MSYS2.MSYS2 --accept-package-agreements --accept-source-agreements
        if ($LASTEXITCODE -eq 0) {
            Write-Host "[2/3] MSYS2 安装成功，正在安装 g++ 工具链 ..." -ForegroundColor Cyan
            $msys2Path = "C:\msys64\usr\bin\bash.exe"
            if (Test-Path $msys2Path) {
                & $msys2Path -lc "pacman -S --noconfirm mingw-w64-ucrt-x86_64-gcc"
            }

            $gccPaths = @(
                "C:\msys64\ucrt64\bin",
                "C:\msys64\mingw64\bin",
                "C:\msys64\mingw32\bin"
            )

            foreach ($p in $gccPaths) {
                if (Test-Path "$p\g++.exe") {
                    $currentPath = [Environment]::GetEnvironmentVariable("Path", "User")
                    if ($currentPath -notlike "*$p*") {
                        [Environment]::SetEnvironmentVariable("Path", "$currentPath;$p", "User")
                        $env:Path = "$env:Path;$p"
                        Write-Host "[3/3] 已将 $p 添加到 PATH" -ForegroundColor Green
                    }
                    break
                }
            }
        }
    } catch {
        Write-Host "[WARN] winget 安装失败，尝试手动下载 ..." -ForegroundColor Yellow
    }
}

if (-not (Test-GppInstalled)) {
    Write-Host "" -ForegroundColor Yellow
    Write-Host "[INFO] 正在下载 MinGW-w64 独立包 ..." -ForegroundColor Cyan

    $downloadUrl = "https://github.com/niXman/mingw-builds-binaries/releases/download/14.2.0-rt_v12-rev0/x86_64-14.2.0-release-posix-seh-ucrt-rt_v12-rev0.7z"
    $installDir = "C:\mingw64"
    $downloadFile = "$env:TEMP\mingw64.7z"

    if (-not (Test-Path $installDir)) {
        Write-Host "  下载地址: $downloadUrl" -ForegroundColor Gray
        Write-Host "  (文件较大，请耐心等待 ...)" -ForegroundColor Gray

        try {
            [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
            Invoke-WebRequest -Uri $downloadUrl -OutFile $downloadFile -UseBasicParsing

            Write-Host "  下载完成，正在解压到 $installDir ..." -ForegroundColor Cyan

            if (Get-Command 7z -ErrorAction SilentlyContinue) {
                & 7z x $downloadFile -o"C:\" -y
            } else {
                Write-Host ""
                Write-Host "  [ACTION REQUIRED] 需要手动操作:" -ForegroundColor Red
                Write-Host "  1. 下载 7-Zip: https://www.7-zip.org/" -ForegroundColor White
                Write-Host "  2. 解压 $downloadFile 到 C:\ " -ForegroundColor White
                Write-Host "  3. 重新运行此脚本" -ForegroundColor White
                Write-Host ""
                Write-Host "  或者直接访问以下地址下载安装包:" -ForegroundColor Yellow
                Write-Host "  https://winlibs.com/" -ForegroundColor White
                Write-Host "  选择 UCRT runtime, Win64, 下载 zip 版本解压到 C:\mingw64" -ForegroundColor White
                exit 1
            }
        } catch {
            Write-Host ""
            Write-Host "  [ERROR] 自动下载失败。请手动安装:" -ForegroundColor Red
            Write-Host ""
            Write-Host "  方式一 (推荐): 使用 winlibs" -ForegroundColor White
            Write-Host "    1. 访问 https://winlibs.com/" -ForegroundColor Gray
            Write-Host "    2. 下载 GCC 14.x + UCRT, Win64 ZIP 版" -ForegroundColor Gray
            Write-Host "    3. 解压到 C:\mingw64" -ForegroundColor Gray
            Write-Host ""
            Write-Host "  方式二: 使用 MSYS2" -ForegroundColor White
            Write-Host "    1. 访问 https://www.msys2.org/" -ForegroundColor Gray
            Write-Host "    2. 下载安装 MSYS2" -ForegroundColor Gray
            Write-Host "    3. 在 MSYS2 终端运行: pacman -S mingw-w64-ucrt-x86_64-gcc" -ForegroundColor Gray
            Write-Host ""
            exit 1
        }
    }

    $binPath = "$installDir\bin"
    if (Test-Path "$binPath\g++.exe") {
        $currentPath = [Environment]::GetEnvironmentVariable("Path", "User")
        if ($currentPath -notlike "*$binPath*") {
            [Environment]::SetEnvironmentVariable("Path", "$currentPath;$binPath", "User")
            $env:Path = "$env:Path;$binPath"
            Write-Host "[OK] 已将 $binPath 添加到用户 PATH" -ForegroundColor Green
        }
    }
}

Write-Host ""
if (Test-GppInstalled) {
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "  编译器安装成功!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "验证命令: g++ --version" -ForegroundColor Gray
    Write-Host "编译示例: g++ -std=c++17 -O2 -o solution solution.cpp" -ForegroundColor Gray
    Write-Host ""
    Write-Host "请重新打开终端以使 PATH 生效。" -ForegroundColor Yellow
} else {
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "  安装未完成，请按照上述提示手动安装" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    exit 1
}

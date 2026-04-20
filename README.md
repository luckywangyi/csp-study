# CSP 考试学习与模拟测试环境

CCF CSP（计算机软件能力认证）C++ 刷题、学习和模拟考试环境。

## 快速开始

```powershell
# 首次使用：安装编译器 + 拉取所有学习计划题目（约 30 道洛谷题）
.\csp init

# 查看学习计划与进度
.\csp plan

# 开始做一道题（洛谷题号自动拉取）
.\csp do P1162

# 在题目目录下编辑 solution.cpp，然后测试
.\csp test

# 也可以在根目录指定题号测试
.\csp test P1162

# 开始做 CSP 真题
.\csp do 201312/t1

# 模拟考试
.\csp exam

# 提交模考答案
.\csp submit t1

# 查看下一道推荐题目
.\csp next

# 列出所有本地题目
.\csp list
```

### 设置快捷别名（可选）

在 PowerShell 中添加别名，之后可以在任意目录使用 `csp` 命令：

```powershell
# 临时生效
Set-Alias csp "C:\Users\wps\csp-study\csp.ps1"

# 永久生效（写入 PowerShell 配置文件）
Add-Content $PROFILE 'Set-Alias csp "C:\Users\wps\csp-study\csp.ps1"'
```

## 所有命令

| 命令 | 说明 |
|------|------|
| `.\csp init` | 初始化环境（编译器 + 拉取洛谷题目） |
| `.\csp do <题号>` | 开始做题，支持 `P1162`（洛谷）或 `201312/t1`（CSP 真题） |
| `.\csp test [题号]` | 测试（在题目目录下可省略参数） |
| `.\csp build [题号]` | 编译 |
| `.\csp exam [套题]` | 开始 4 小时限时模考 |
| `.\csp submit <t1-t5>` | 提交模考答案 |
| `.\csp plan [周次]` | 查看学习计划进度 |
| `.\csp next` | 推荐下一道题 |
| `.\csp list` | 列出所有本地题目 |
| `.\csp fetch <P题号>` | 从洛谷抓取单道题 |
| `.\csp help` | 显示帮助 |

## 典型工作流

### 日常刷题

```
.\csp plan          → 查看本周计划和进度
.\csp next          → 获取推荐题目
.\csp do P1162      → 拉取题目，查看题面
                       编辑 problems/luogu/P1162-填涂颜色/solution.cpp
.\csp test P1162    → 编译并测试，自动更新进度
.\csp plan          → 确认进度已更新
```

### 模拟考试

```
.\csp exam          → 随机选取套题，开始 4 小时倒计时
                       在 mock-exam/practice/exam-xxx/ 下编辑各题 solution.cpp
.\csp submit t1     → 提交第 1 题，查看得分
.\csp submit t2     → 提交第 2 题
                       （每题最多提交 32 次，取最高分）
```

## 目录结构

```
csp-study/
├── csp.ps1                  # 统一 CLI 入口（所有操作通过此脚本）
├── setup-compiler.ps1       # 编译器安装脚本
├── data/                    # 数据文件
│   ├── study-problems.json  # 学习计划题目清单
│   └── progress.json        # 做题进度
├── scripts/                 # 自动化工具
│   ├── build.ps1            # 编译
│   ├── run.ps1              # 评测
│   ├── new-problem.ps1      # 创建题目
│   ├── fetch-problem.ps1    # 从洛谷拉取题目
│   ├── init-study-problems.ps1  # 批量拉取学习计划题目
│   ├── gen-tests.ps1        # 生成测试数据
│   └── gen-all-tests.ps1    # 批量生成测试数据
├── templates/               # 代码模板
│   ├── solution.cpp         # 标准解题模板
│   ├── stl-reference.cpp    # STL 速查
│   └── algorithms/          # 算法模板库
├── problems/                # 题目
│   ├── 201312/              # CSP 真题（按期次）
│   ├── ...
│   └── luogu/               # 洛谷题目（自动拉取）
│       ├── P1162-填涂颜色/
│       ├── P5660-数字游戏/
│       └── ...
├── practice/                # 个人练习区
├── mock-exam/               # 模拟考试
│   ├── start-exam.ps1
│   ├── submit.ps1
│   └── exam-sets/           # 套题
└── knowledge/               # 知识库
    ├── study-plan.md        # 8 周学习计划
    ├── cheatsheet.md        # 考前速查表
    └── topics/              # 专题文档
```

## CSP 考试须知

| 项目 | 说明 |
|------|------|
| 题目数量 | 5 道编程题 |
| 总分 | 500 分（每题 100 分） |
| 考试时长 | 4 小时 |
| 支持语言 | C/C++、Java、Python |
| 评测方式 | 黑盒测试，每题约 10 个测试点 |
| 提交次数 | 最多 32 次，取最高分 |

### 分数目标参考

- **200 分**: 稳拿 T1 + T2（基础水平）
- **300 分**: 攻克 T3（中级水平，可获优秀证书）
- **400 分**: 突破 T4（高级水平）
- **400+**: T5 拿部分分（竞赛水平）

## 在 CLion 中使用

本项目包含 `CMakeLists.txt`，CLion 打开即可自动加载。

### 打开项目

1. CLion -> Open -> 选择 `csp-study` 文件夹
2. CLion 自动检测 `CMakeLists.txt`，点击 **Open as Project**
3. 等待 CMake 加载完成（右下角进度条）

### 选择题目

所有题目自动生成为独立的 CMake target：

- CSP 真题：`csp_201604_t1`、`csp_201312_t2` 等
- 洛谷题目：`P1162`、`P5660` 等

在工具栏的 **target 下拉列表** 中选择要做的题目即可。

### 编译与调试

| 操作 | 快捷键 |
|------|--------|
| 编译 | Ctrl+F9 |
| 运行 | Shift+F10 |
| 调试 | Shift+F9 |

### 调试时喂入测试数据

CLion 原生支持 stdin 重定向：

1. 先在终端运行 `.\csp test --case 3`，输入会自动复制到 `.debug/debug_input.txt`
2. 点击 **Edit Configurations**（target 下拉列表旁的下拉箭头）
3. 在 **Redirect input from** 中填入：`.debug/debug_input.txt`
4. 按 Shift+F9 调试，程序会自动读取该测试输入

### 测试与对拍

测试命令在 CLion 内置终端中使用（与命令行完全一致）：

```powershell
.\csp test             # 测试当前题目
.\csp test --case 3    # 只跑第 3 个测试点
.\csp verify           # 对拍验证
```

### 添加新题目后

如果新增了题目文件夹（如 `.\csp fetch P1234`），需要在 CLion 中重新加载 CMake：

**Tools -> CMake -> Reload CMake Project**（或点击 CMake 面板的刷新按钮）

## 常见编译命令

```bash
# 标准编译
g++ -std=c++17 -O2 -Wall -o solution solution.cpp

# 调试编译
g++ -std=c++17 -g -DDEBUG -Wall -o solution solution.cpp

# 运行
./solution < input.txt
```

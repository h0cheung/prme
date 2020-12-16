# Process and Resource Management Emulator

A simple process and resource management emulator for study.

计算机操作系统课程的实验代码。

## 特性

- 使用 C++ 编写，跨平台，无第三方库依赖

- 对操作系统进程调度的简单模拟

- 对资源管理的简单模拟

- 简单友好的测试 shell，含帮助和错误提示

- 较好的性能

- 良好的稳定性和健壮性，尽量避免 bug 和漏洞

- 内置支持文件输入输出

## 构建和运行

### 要求

- CMake (>= 3.12)

- 支持 C++17 标准的 64 或 32 位编译套件 (推荐 GCC 10 或 LLVM 11，测试环境为 GCC 10.2.0 x86_64-pc-linux-gnu)

### 构建步骤

在项目目录中使用 CMake 程序，以下为 Linux 系统中示例：

```sh
mkdir build
cmake ..
make
```

或者，导入到支持 CMake 项目的 IDE（QT Creator、KDevelop、CLion 等）中进行构建，亦可使用各种代码编辑器的 CMake 相关插件。

### 运行

构建后会得到名为 `prme` 的可执行文件（Windows 下可能是 `prme.exe`），可在终端中运行。

#### 运行参数

直接运行进入交互模式，可输入 `h` 或 `?` 获取支持命令列表。

参数 `-i, --input <filename>`，从文件读取输入，此时不会输出提示符。

参数 `-o, --output <filename>`，输出到文件。

参数 `-h, --help`，显示帮助。

#### 支持命令

`init` 初始化

`cr <name> <priority>(= 1 or 2)` 创建进程

`de <name>` 递归删除进程

`req <resource name> <#of units>` 请求资源

`rel <resource name> <#of units>` 释放资源

`to` 时间片到或者外部请求中断

`list ready` 显示就绪队列

`list block` 显示阻塞队列

`list res` 显示资源

`pr <name>` 显示进程信息

`?, h` 显示帮助

`<EOF>, <SIGINT>` 退出
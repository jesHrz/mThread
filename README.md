# mThread

mThread 是32位架构的用（wan）户（ju）级线程库。

mThread 所提供的API有：

- `mthread_create`：创建一个线程，需要指定线程的入口函数以及要传递的参数。
- `mthread_yield`：线程主动释放 CPU，调度下一个可调度的线程。
- `mthread_exit`：线程退出，可以给出退出码，当线程自然结束（函数返回时），函数的返回值作为退出码。
- `mthread_sleep`：线程睡眠，时间精确到毫秒，在此期间线程处于不可调度状态。线程唤醒的时间只会比预定时间晚，但绝不会早于预定时间。
- `mthread_join`：主线程等待指定线程的结束。

## How To Use

在项目根目录下执行 `make` 命令就可以生成 mThread 库。

目录 `examples` 中提供了一个简单的示例程序，通过示例程序你可以快速了解mThread的使用方式。执行 `make examples` 就可以编译这个示例程序。

但是在运行示例程序的前，你需要让 Linux 能够找到 mThread 提供的两个动态库 `libmthread.so.1` 和 `context/libcontext.so.1`。一种方法是设置环境变量 `LD_LIBRARY_PATH` 到 mThread 的目录以及 mThread 目录下的 `context`，Linux 会根据这个环境变量来寻找动态链接库。第二种方法是把这两个库文件复制到 Linux 可以搜索到的目录下，比如 `/usr/lib`。至此，你就可以运行这个示例程序了。

To generate the mThread library just run `make` at root directory of the project.

There is a demo program in directory `examples` that you can have a quick look of mThread. Command `make examples` will compile the demo program.

Before you run it, you should make sure that Linux can find two libraries: `libmthread.so.1` and `context/libcontext.so.1`. To achieve this, you can set environment variable `LD_LIBRARY_PATH` to `/path/to/mthread` and `/path/to/mthread/context` or just move `libmthread.so.1` and `context/libcontext.so.1` to `/usr/lib`. Then You can execute the demo program as usual.

## 测试环境

在 Debian 10 amd64 环境下示例程序测试通过。
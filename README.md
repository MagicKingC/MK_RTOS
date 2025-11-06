## MK_RTOS

说明：基于cortex-m3和qmenu内核设计的一款RTOS，预计支持信号量，互斥量，定时器，事件等功能，待基本功能完善就开始支持多种硬件的支持，如stm32系列，gd32系列等
    同时会添加多种组件，如文件系统，网络协议栈等，帮助开发者快速搭建嵌入式系统。体验RTOS的魅力，享受嵌入式开发的乐趣。

---------------------------------------


#### 系统架构

后面补上，哈哈哈哈


#### 系统代码风格

###### 函数命名风格 ： mk_用途，例子：
```c
mk_startup_kernel : 表示系统入口
```

#### 环境搭建
```bash
# 开发环境：Ubuntu 20.04 + arm-none-eabi-工具链（如果有小伙伴想在win下开发，可以联系本作者出一个win下的环境搭建教程）
# 安装工具
sudo apt-get install  cmake
sudo dpkg -i tools/gdb-arm-none-eabi_7.10-1ubuntu3+9_amd64.deb

# Ubuntu 22.04（可以使用一下命令进行安装）
sudo apt update
sudo apt install gcc-arm-none-eabi gdb-multiarch -y

# 注意：如果使用gdb-multiarch 那么需要再 launch.json 中将 arm-none-eabi-gdb 改为 gdb-multiarch

```

#### 编译说明
该工程使用cmake进行任务管理
```bash
# 克隆项目
git clone git@github.com:MagicKingC/MK_RTOS.git
# 进入目录
cd MK_RTOS
# 创建 build 文件夹
mkdir build
# 进入编译文件夹
cd build 
cmake ..
# 编译
make 
```

#### 调试说明
```bash
# 创建.vscode文件夹
mkdir .vscode
# 将 tools/vscode_config 里面的文件拷贝到 .vscode
# 方式1
# 开启debug调试
make debug 
# 点击vscode 左边调试按键
# 然后点击上面绿色小按钮，选择 MK_RTOS Debug （千万不要选错）

# 方式2 （无效）
# 使用vscode 调试插件
1、点击vscode 左边调试按键
2、选择 MK_RTOS Build And Debug Run，点击左边绿色小按钮
3、选择 MK_RTOS Debug，点击左边绿色小按钮
```


#### 进度说明
1、实现了基本的任务切换（已完成）

2、添加系统延迟函数（已完成）

3、基于时间片的任务切换（已完成）

4、信号量（已完成）

5、互斥量（已完成）

6、队列

#### 额外说明
目前还在研发中，有兴趣的小伙伴可以一起研发，共同开发属于自己的rtos

讨论qq群： 77436255



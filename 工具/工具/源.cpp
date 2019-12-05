//包管理器 yum
//编辑器 vim
//编译器 g++ / gcc
//调试器 gdb makefile git
//
//yum
//yum - y[packetname] 安装
//yum remove 删除
//yum list 展示
//
//正常模式命令
//vim 移 删 撤 替 复 更 跳
//vim  filename  普通模式
//i I o O
//插入模式
//命令行模式
//
//移动 h j k l
//删除 x X  dd
//复制 yy yw
//替换 r R
//撤销 u ctrl
//更改 cw c3w
//跳 ctrl + g / 6G
//命令行模式
//set nu / set nonu 显示行号
//s / src / tar / 替换当前行的第一个
//%s / src / tar / 替换所有行的第一个
//%s / src / tar / g  替换所有
//
//g++ / gcc
//gcc - E test.c - o test.i  预处理
//gcc - S test.i - o test.s 编译
//gcc - C test.s - o test.o 汇编
//* gcc  test.c - o out 动态链接
//* gcc  test.c - o out1 - static 静态链接
//gcc  test.c - o out - g 生成debug版本的可执行文件
//md5sum out
//ldd out 查看当前文件依赖哪些文件
//file out 查看文件属性
//ll - h 查看文件大小
//
//gdb 调试
//事前
//list 查看源码
//run
//breakpoint + 行号  打断点
//b 行号 在main函数所在的文件打断点
//b test.c:行号
//enable 序号 使断点生效
//disable 序号 使断点不生效
//delete 序号 删除断点
//info b  查看断点
//i b
//n 逐过程 next
//s 逐语句 step
//bt 查看调用堆栈 backtrace / where
//c 继续执行 continue
//查看变量的值
//p 变量
//i locals 查看临时变量的值
//事中
//ps aux 查看当前所有进程
//ps aux | grep . / out 管道查看. / out(可执行程序)
//查看进程
//gdb - p  PID 查看进程
//gdb attach PID 查看进程
//pstack 查看进行的调用堆栈
//
//事后
//ulimit - a 查看系统限制信息 ulimit - c 修改大小
//* core file size 核心转储文件的大小
//* gdb 可执行程序 coredumpfile(核心转储文件)
//bt 查看调用堆栈-- > 查看对应的源码代码行进行确认-- > 代码是否是与核心转储文件版
//本一致 / 决定是否进行版本回退 / 是否为debug版本 
//f 堆栈号
//
//make
//解释 makefile 中的内容
//linux makefile.gcc
//包含 : makefile.inc
//	 win makefile.mvc
// 包含 : makefile.inc
//  makefile :
//目标对象 : 依赖对象
//	编译命令
//	1.对比最后一次修改时间, 决定是否执行编译命令
//	.PHONY : out 伪目标, 屏蔽比较修改时间
//	2.判断依赖对象是否存在,
//	存在, 执行编译命令
//	不存在则查找生成依赖对象的方法, 找不到报错
//	3.只为生成第一个目标对象
//
//	预定义对象
//	$^ 所有依赖对象
//	$@ 目标对象
//	$ < 依赖的第一个对象
//clean : 不依赖对象, 执行清理工作
//
//		$ %.c %.a
//git clone
//git add
//git commit - m "描述信息"
//git push
//git status 信息
//git pull

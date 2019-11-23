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
//gcc  test.c - o out 动态链接
//gcc  test.c - o out1 - static 静态链接
//将
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
//bt 调用堆栈 backtrace / where
//c 继续执行 continue
//查看变量的值
//p print
//i locals 查看临时变量的值
//事中
//事后
//
//

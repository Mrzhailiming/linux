//工具 ： VMware centos7 xshell
//获取ip地址 ： ifconfig
//ens33 	innet
//NAT模式	桥接模式	host - only
//
//unix ——》 linux / mac os
//
//
//ls(list) :
//查看当前目录下的文件 / 文件夹
//ls / 根目录
//ls ~用户的家目录   ls / home / user
//pwd : 查看当前目录
//	  ls - l 查看属性
//	  ls - a 查看包括隐藏的文件
//	  ls - lrt 按照时间排序   最近修改的文件 / 文件夹放在最下面
//
//
//	  cd change dir
//	  cd ~直接回到家目录
//	  cd / 直接回到根目录
//	  cd - 回到上一次打开的目录
//
//	  touch 创建文件
//	  touch[filename]
//	  touch 相对 / 绝对路径[filename]
//
//	  mkdir 创建文件夹
//	  mkdir 相对 / 绝对路径[foldername]
//	  mkdir - p 创建多级文件夹
//
//	  echo
//	  echo "str" > test.c
//
//	  cat 查看文件内容
//	  cat[filename]
//	  cat - n[filename] 输出文件的行号, 和内容
//
//	  rm remove 删除普通文件
//	  rm - r 递归删除(文件夹)
//	  rm - rf force 强制删除
//	  rm - rf * 删除文件夹下所有文件  	* ? 通配符
//
//	  history 查看历史用过的命令
//
//	  cp copy 拷贝文件 / 文件夹
//	  cp 相对路径 / 绝对路径[srcfilename][path]
//	  cp - r 相对路径 / 绝对路径[srcfoldename][path]    拷贝文件夹
//
//
//	  mv 移动 / 重命名 文件 / 文件夹
//	  mv filename path 							win 动态库.dll  静态库.lib
//	  mv filename dest_filename		                    linux.so.a
//
//
//
//  tar :
//gzip :
//	tar - zcvf filename.tar.gz[被压缩的文件]  压缩
//	tar - zxvf filename   解压缩
//bzip2 :
//	 tar - jcvf filename.tar.bz2[被压缩的文件]
//
//		 zip / unzip :
//		 zip name.zip filename 压缩文件
//		 zip - r name.zip foldername	压缩文件夹
//		 unzip filename / foldername
//
//		 !执行上一次执行的命令
//
//	 查看文件内容 :
//	 more f b 回车
//		 less 			f向后 b向前 pageup pagedown
//		 less - n filename 显示行号
//		 head
//		 head filename 默认显示10行
//		 head - [n] filename 显示多少行
//		 tail
//		 tail filename 默认显示10行
//		 tail - f filename 实时监控文件
//
//		 find
//		 find[path] - name filename 在目录下按照文件名查找文件
//		 grep
//		 grep "str"[path] - r 查找str
//		 ls | grep "str" 管道
//
//		 date
//		 date 查看时间
//		 date - s "20131111 01:01:01"  修改时间
//
//		 cal
//		 cal 查看日历
//
//		 uname
//		 uname - a
//
//		 ulimit
//		 ulimit - a 查看系统限制信息
//		 core file 内核转储文件 镜像
//
//		 ctrl c
//		 ctrl a
//
//		 su 切换权限
//		 su -
//
//
//		 read
//		 write
//		 execute
//		 chmod 修改权限
//		 chmod u / g / o + / -x / r / w[filename]   : 修改当前用户 所属组 其他用户
//		 chmod 777[filename]
//
//		 rwx
//	 对于普通文件 :
// 对于文件夹 : 没有
//
// chown : 修改拥有者
//	 chgrp : 修改所属组
//			 沾滞位
//			 chmod + / -t  filename
//			 有了沾滞位之后谁可以删除
//			 root
//			 文件拥有者
//
//		 umask : 初始权限
//				 系统初始权限 & (~umask)
//
//
//
//
//

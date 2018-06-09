# OS-Experiment
operating system experiment
****
## 文件读写编程题目
### myecho.c
&emsp;&emsp;myecho.c的功能与系统echo程序相同<br>
&emsp;&emsp;接受命令行参数，并将参数打印出来，例子如下：<br>
```
$ ./myecho x
x
$ ./myecho a b c
&a b c
```
```
    通过main函数的标准输入参数获取到我们从命令行输入的参数个数argc和参数组成的二维数组argv，因为第一个参数是./myecho所以
不用输出第一个参数，只要输出剩余的参数即可。使用一个循环由1-argc输出获取到的参数便完成了myecho。
```
### mycat.c
&emsp;&emsp;mycat.c的功能与系统cat程序相同<br>
&emsp;&emsp;mycat将指定的文件内容输出到屏幕，例子如下：<br>
&emsp;&emsp;要求使用系统调用open/read/write/close实现<br>
```
$ cat /etc/passwd 
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
...
$ ./mycat /etc/passwd 
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
```
```
    首先判断argc是否大于2，是否给定了打开的文件目录。如果给定了文件名，则从argv[1]中提取该名称，使用open函数打开文件，
使用read_only只读模式打开，得到文件描述符fd。使用read函数从文件描述符中逐个字符读出并输出，直到read函数返回0表示文件
到达了末尾。
```
### mycp.c
&emsp;&emsp;mycp.c的功能与系统cp程序相同<br>
&emsp;&emsp;将源文件复制到目标文件，例子如下：<br>
&emsp;&emsp;要求使用系统调用open/read/write/close实现<br>
```
$ cat /etc/passwd
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
...
$ ./mycp /etc/passwd passwd.bak 
$ cat passwd.bak
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
```
```
    首先判断获取的参数个数是否大于3，如果大于等于3说明用户从命令行输入了源文件名以及复制的目标地址名。使用open函数，使用
fd_old文件描述符只读模式打开源文件地址，使用fd_new文件描述符创建读写模式打开目标文件地址。这里对于目标文件打开时，需要给
定open的可选项，给定0777最高的权限，这样最终的输出结果能够直接被打开或被其他文件使用，否则可能会导致需要使用chmod增加文件
权限才能使用。最后使用read函数从fd_old中逐个字符读入使用write函数输出到fd_new的目标文件中。
```
****
## 多进程题目
### mysys.c: 实现函数mysys，用于执行一个系统命令，要求如下
&emsp;&emsp;mysys的功能与系统函数system相同，要求用进程管理相关系统调用自己实现一遍<br>
&emsp;&emsp;使用fork/exec/wait系统调用实现mysys<br>
&emsp;&emsp;不能通过调用系统函数system实现mysys<br>
```
测试程序
#include <stdio.h>

int main()
{
    printf("--------------------------------------------------\n");
    system("echo HELLO WORLD");
    printf("--------------------------------------------------\n");
    system("ls /");
    printf("--------------------------------------------------\n");
    return 0;
}
测试程序的输出结果
--------------------------------------------------
HELLO WORLD
--------------------------------------------------
bin    core  home	     lib	 mnt   root  snap  tmp	vmlinuz
boot   dev   initrd.img      lost+found  opt   run   srv   usr	vmlinuz.old
cdrom  etc   initrd.img.old  media	 proc  sbin  sys   var
--------------------------------------------------
```
### sh1.c: 实现shell程序，要求具备如下功能
&emsp;&emsp;支持命令参数
```
$ echo arg1 arg2 arg3
$ ls /bin /usr/bin /home
实现内置命令cd、pwd、exit
$ cd /bin
$ pwd
/bin
```
### sh2.c: 实现shell程序，要求在第1版的基础上，添加如下功能
&emsp;&emsp;实现文件重定向
```
$ echo hello >log
$ cat log
hello
```
### sh3.c: 实现shell程序，要求在第2版的基础上，添加如下功能
&emsp;&emsp;实现管道
```
$ cat /etc/passwd | wc -l
实现管道和文件重定向
$ cat input.txt
3
2
1
3
2
1
$ cat <input.txt | sort | uniq | cat >output.txt
$ cat output.txt
1
2
3
```
****
## 多线程题目
### pi1.c: 使用2个线程根据莱布尼兹级数计算PI
&emsp;&emsp;莱布尼兹级数公式: 1 - 1/3 + 1/5 - 1/7 + 1/9 - ... = PI/4<br>
&emsp;&emsp;主线程创建1个辅助线程<br>
&emsp;&emsp;主线程计算级数的前半部分<br>
&emsp;&emsp;辅助线程计算级数的后半部分<br>
&emsp;&emsp;主线程等待辅助线程运行結束后,将前半部分和后半部分相加<br>
### pi2.c: 使用N个线程根据莱布尼兹级数计算PI
&emsp;&emsp;与上一题类似，但本题更加通用化，能适应N个核心，需要使用线程参数来实现<br>
&emsp;&emsp;主线程创建N个辅助线程<br>
&emsp;&emsp;每个辅助线程计算一部分任务，并将结果返回<br>
&emsp;&emsp;主线程等待N个辅助线程运行结束，将所有辅助线程的结果累加<br>
### sort.c: 多线程排序
&emsp;&emsp;主线程创建一个辅助线程<br>
&emsp;&emsp;主线程使用选择排序算法对数组的前半部分排序<br>
&emsp;&emsp;辅助线程使用选择排序算法对数组的后半部分排序<br>
&emsp;&emsp;主线程等待辅助线程运行結束后,使用归并排序算法归并数组的前半部分和后半部分<br>
### pc1.c: 使用条件变量解决生产者、计算者、消费者问题
&emsp;&emsp;系统中有3个线程：生产者、计算者、消费者<br>
&emsp;&emsp;系统中有2个容量为4的缓冲区：buffer1、buffer2<br>
&emsp;&emsp;生产者生产'a'、'b'、'c'、‘d'、'e'、'f'、'g'、'h'八个字符，放入到buffer1<br>
&emsp;&emsp;计算者从buffer1取出字符，将小写字符转换为大写字符，放入到buffer2<br>
&emsp;&emsp;消费者从buffer2取出字符，将其打印到屏幕上<br>
### pc2.c: 使用信号量解决生产者、计算者、消费者问题
&emsp;&emsp;功能和前面的实验相同，使用信号量解决<br>
### ring.c: 创建N个线程，它们构成一个环
&emsp;&emsp;创建N个线程：T1、T2、T3、… TN<br>
&emsp;&emsp;T1向T2发送整数1<br>
&emsp;&emsp;T2收到后将整数加1<br>
&emsp;&emsp;T2向T3发送整数2<br>
&emsp;&emsp;T3收到后将整数加1<br>
&emsp;&emsp;T3向T4发送整数3<br>
&emsp;&emsp;…<br>
&emsp;&emsp;TN收到后将整数加1<br>
&emsp;&emsp;TN向T1发送整数N<br>

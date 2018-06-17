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
```
  通过main获得输入参数个数argc和输入参数argv。程序开始输出第一条分割线，随后调用子函数mysys，子函数中首先创建一个子进程，
用于执行execvp，因为execvp函数执行后会有return返回，如果不使用子进程可能会导致主进程的结束，同时考虑到各种命令所处于的目
录不同，所以使用默认使用系统的所有环境变量寻址的执行函数，而且适应argv的参数类型，使用execvp而不是execvl函数。使用时需要
剔除命令行中第一个参数./mysys，所以从argv[1]开始传递。将execvp(argv[1],argv+1)放置于子进程中执行，在父进程中使用
wait(NULL)等待子进程执行结束，之后输出第二条分割线，此处如果不等待子进程执行，两条分割线便会在系统指令执行出结构前输出。
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
```
    主函数中使用while(1)循环，每次执行输出模拟shell命令的当前工作目录，读取用户输入的一行指令，对指令进行分析判断指令类
型，进行处理，清空数组使用，等待下一次输入。
    用于读取用户输入的函数getcommand(int *argc,char *argv[])，函数结束时通过指针调用返回用户输入的参数个数argc以及输入
的参数argv的char *型数组。在函数体中，使用while(1)循环始终等待用户输入，逐个字符读取，直到用户输入了一个换行符便跳出循环
。使用text_flag判断一行的第一个非空格字符是否出现，即忽略行首的连续空格，使用space_flag判断参数之间的空格，即忽略参数间
连续出现的多个空格最终只保留一个。如果输入的是非空格字符则添加至当前字符串当前位置之后，如果输入了空格则在当前字符串末尾添
加’\0’结束符，并且进入下一字符串的读取。使得最终返回的参数格式与main函数接收的标准参数一致，后序处理以及使用execvp函数更
加方便。
    用于执行指令的mysys(int *argc,char *argv[])，获取getcommand所处理之后的参数。首先创建一个子进程，在子进程中执行输
入的指令，并在父进程中等待执行结束，否则可能出现下一次等待用户输入的提示信息提前输出在执行结果之前。
    主函数循环中，判断输入的第一个指令参数，如果为exit则直接跳出循环结束程序，如果为空说明用户未输入信息直接按下了回车，
则直接进入下一次循环，如果为内置指令cd、pwd则直接调用chdir以及getcwd函数处理工作目录的调整以及获取当前工作目录的功能。
```
### sh2.c: 实现shell程序，要求在第1版的基础上，添加如下功能
&emsp;&emsp;实现文件重定向
```
$ echo hello >log
$ cat log
hello
```
```
    主体与函数基本和sh1类似，在mysys函数中添加文件重定向的功能。
    函数mysys中首先对获取的参数进行判断，当参数个数大于三个且倒数第二个参数为’>’符号时，便执行输出结果重定向至文件的操作
，最后一个参数为文件的名称。实现时，首先使用一个临时字符数组存储文件位置，随后在参数数组中将最后两个赋值为空。之后再次创建
一个子进程用于创建管道，这里必须再建立一个进程，避免在重定向标准输入输出端口时修改到主进程的端口，导致后序的输入输出产生问
题。在子进程中将进程的标准输出连接到管道的写端，执行指令，在父进程中等待子进程执行完成，将进程的标准输入连接到管道的读端，
逐个字符从输入中读取，并同时输出到目标文件中。
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
```
    在sh2的基础上只需要修改mysys函数中两处execvp函数执行的地方，用自己的函数替换，实现管道操作，保正输出为标准输出即可，
这样之前的所有操作包括文件重定向也不需要更改。
    首先在函数中对获取到的参数数组进行预处理，遍历获取所有’|’出现的位置并在flag[]数组中记录，在数组首尾添加-1以及argv用
于标识两边边界。在处理中判断上一个’|’出现的位置是否在前一个之后，如果出现则直接返回报错。预处理结束之后使用run_pipe(
int start,int end,int fcnt,char *argv[])函数递归处理。前两个参数为当前处理的指令的首末位置，第三个参数为整行指令被’|’
拆分为了几段，最后一个参数为获取的参数列表。
    递归函数中首先判断end是否为start+1即是否为最左侧的指令，如果不是则需要建立子进程，并建立新的管道用于处理左侧的下一条
指令，该进程处理当前最右侧指令，在子进程中需要将自己的标准输出重定向到管道的写端，随后调用run_pipe(
start,end-1,fcnt,argv)，子进程结束，父进程首先等待子进程运行，随后判断是否为第一条指令，如果不是需要将自己的标准输入连接
到管道的读端，并提取出当前所要执行的指令的所有信息，随后调用execvp函数执行。
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

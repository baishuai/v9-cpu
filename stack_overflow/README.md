## 实验楼“缓冲区溢出漏洞实验”实验报告

在线实验地址链接 https://www.shiyanlou.com/courses/231

按照实验步骤得到的实验结果，成果获取root权限shell
![图片描述](https://dn-simplecloud.qbox.me/yMiQtH8j9m1QXt9fsNtrrX5dhNF4ST5V5fjHrOWL-wm)

使用其他的shell，比如bash，不能获得root权限的shell
![图片描述](https://dn-simplecloud.qbox.me/jj8cuvf2a40SXvI99NcEaxxhLd8K42X8A7pojIx8-wm)

打开地址随机化选项，堆栈地址随机化，返回地址猜测不正确，也不能成功获取root shell
![图片描述](https://dn-simplecloud.qbox.me/J0h0rOg4IzCZLobVuWZEdd1xTb7yLV5EIq5npy0Z-wm)

按照实验说明指导可以毫无障碍完成本次实验。所以后面主要讨论一下实验说明中没有讲清楚的地方。

缓冲区溢出漏洞攻击主要有几个步骤。
1. 定位可能缓冲区溢出的相关函数
2. 构造shellcode以及定位shellcode的地址
3. 修改返回地址

### 定位缓冲区溢出位置
实验stack.c 代码中很容易可以看书在buf函数中调用了不安全的strcpy函数。可以确定这里会发生缓冲区溢出。main函数中读入了517字节长度的字符串，并在函数buf中将517字节的字符串拷贝到内存分配只有12字节的buffer中。整个字符串将会覆盖bof函数中的相关栈帧。包括buf的返回地址等信息。

### 构造shellcode以及shellcode定位
缓冲区溢出的攻击中，shellcode具有一定的通用性，所以很方便的可以找到一段shellcode，shellcode其实就是调用系统程序/bin/sh的一段编译后的二进制代码。我们将这段shellcode保存在buffer中偏移量100的位置，所以定位shellcode只需要确定main函数中str的变量内存位置。通过gdb调试设置断点查看esp寄存器值就可以知道确定的地址，简单的python语句`'hex(0xffffd330+100)'`就可以计算出shellcode的16进制地址。填充至字符串相应位置。

### 修改返回地址
首先明确需要修改的是bof函数的返回地址。通过gdb调试要计算出bof返回地址与bof栈帧中buffer变量的地址相对偏移量。
通过设置断点查看esp寄存器的值可以确定buffer的地址为`0xffffd300`，bof返回地址为`0xffffd31c`。相对的偏移量为`0xffffd31c - 0xffffd300 - 4`。在攻击字符生成程序中，`strcpy(buffer + 0xffffd31c - 0xffffd300 - 4, "\x94\xd3\xff\xff");`将shellcode地址填充到buffer对应偏移量的位置上。
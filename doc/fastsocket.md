## Scalable Kernel TCP Design and Implementation for Short-Lived Connections

###### 白帅 P15240002

随着网络带宽的不断增长、单机上CPU内核的增多以及移动应用对短期连接需求的增多，可扩展性的TCP协议栈成为性能提升的关键。尽管人们提出许多新设计，但是设计环境仍然需要自底向上，兼容现有应用的协议栈的设计。
作者提出了Fastsocket概念，它可以兼容BSD套接字和可扩展性的内核套接字的设计。它可以在TCP协议栈实现桌面级的连接划分，确保局部性的主动连接和被动连接的实现。Fastsocket架构是一个分块的设计包括从NIC中断到应用程序，该架构能够去除整个栈内的各种竞争锁定。此外，Fastsocket保持所有TCP协议栈基本功能和BSDsocket兼容的 API，应用程序不需做任何修改。
在24核机器中，Fastsocket实现短期连接负载下的加速，优于Linux内核TCP的实现。当扩展为24CPU内核，Fastsocket会增加Nginx和HAProxy相应的吞吐量，性能提升267%和621%。也证明了Fastsocket可以扩展并且保存BSD套接字API。Fastsocket已用于新浪微博环境，为5000万日活跃用户服务，响应每天数十亿的请求。

文章的三个贡献点：
* 文章引用内核网络栈的设计用来实现TCB管理的桌面级分割，并实现任意连接类型的连接局限性。
* 论述BSD套接字API不太可能成为网络堆栈可扩展性的障碍。
* 对内核适度修改，可以建立一个高度可扩展的内核网络协议栈。Fastsocket遵循此此理念，已大量部署在新浪微博环境中。


作者着重于可扩展的瓶颈：TCB管理和VFS抽象。为彻底解决这些瓶颈并向后兼容现有TCP栈分析了所面临的挑战。

### Fastsocket的设计
Fastsocket架构，它由三部分构成。TCB数据结构的分割（local listen Table，local established Table），Receive Flow Deliver，Fastsocket-aware VFS。这三部分通过协作为每一个连接提供一个Per-Core 进程区域。为实现 the Per-Core Process Zone，作者对共享数据结构，连接内核绑定和VFS抽象实现进行了改进：
1. Fastsocket对数据结构分割和管理全局TCB tables。这样当NET_RX请求到达TCP层，Fastsocket使用per-core Local Listen Table 和per-core Local Established Table进行整个TCB管理。
2. 在一个数据包进入网络协议栈之前，Fastsocket使用Receive Flow Deliver来引导数据包进入一个对应local Table的适当的CPU内核。利用这种方式，Fastsocket能实现最大的本地连接和最小的CPU cache反弹。
3. 内核的VFS层是套接字API抽象和兼容的关键。支持Fastsocket的VFS旁路所有不必要的和集中锁定的程序；使用专门的套接字快速路径去消除可扩展性瓶颈。

评价Fastsocket的性能及扩展性，侧重点在以下两个问题：
Fastsocket是否在实际应用程序有益？相比于先进的Linux TCP内核栈的实现，Fastsocket怎样的规模可以实现短暂的TCP连接处理？

文章就是介绍了Fastsocket，一个新型可扩展的TCP套接字。Fastsocket进一步可实现PerCore Process Zone，以避免锁竞争并保持同一时间的兼容性。主要设计是，Local Listen Table 划分成 global listen table，并确保被动连接的局部性；Local Established Table 划分global established table，并保证TCB管理完整的分区；接收流程的传递确保局限性的主动连接与被动连接共存； Fastsocket-aware VFS 在避免套接字上的严重锁竞争机制的同时，保持与现有应用程序的兼容。

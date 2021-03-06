# Scalable address space using RCU balanced tree

> Clements, A. T., Kaashoek, M. F., & Zeldovich, N. (2012). Scalable address spaces using RCU balanced trees. ACM SIGPLAN Notices, 47(4), 199-210.

本篇论文主要讲的是通过RCU平衡树实现可扩展的地址空间。

## 背景

现在的软件开发者开发的软件都采用多线程和多进程，这种方式下的多个进程通常共享一个单独的地址空间，共享地址空间就会产生一些额外的代价，比如说解决缺页中断、增加地址空间，文件映射等。现有的大部分操作系统均是通过每个进程依靠一个单独的锁来管理进而实现上述操作的串行化处理。

现在的操作系统采用的是每一个进程一个锁来单独进行管理，这样就会导致缺页中断操作和内存映射操作串行进行，限制了多线程VM加强应用的可扩展性。而我们的目标是让缺页中断可以在多个core间同时并行进行处理。要想达到该目标我们需要做两个工作，第一，必须允许page faults和mmap或者munmap可以同时并行处理，然而这三个操作都会修改地址空间的数据结构，所以达到该目的会有一定难度。第二，即使我们让page faults不修改cache lines进而避免串行等待的时间，可是即使是在读模式下，锁仍然要求修改cache lines。

RCU虽然已经被广泛的应用在linux kernel，但没有被应用在address space，原因有两个

第一，地址空间必须遵从复杂的多样性;
第二，需要一个兼容RCU的并行平衡树结构。
而我们本篇论文针对上述问题通过两种方式将RCU应用于address space,

第一，提出了一种依靠RCU方式进而实现解决软缺页中断和其他操作在一个共享的缓冲区可以并行处理，引入了基于RCU的平衡二叉树，即BONSAI tree来解决该问题。
第二，通过可以并发的地址空间解决了该问题。

## BONSAI tree

该平衡二叉树的设计是允许类似于查找之类的读操作可以在没有锁的机制下与插入或者删除这样的修改操作并行进行，这对于大的多核处理器可以避免频繁的cache一致性通信。

## Concurrent address space

在本篇论文中实现了三种方式，分别为fault locking、hybrid locking/RCU、pure RCU。

## 结论

通过在三种不同的benchmark上进行实验，验证了bonsai tree很好的解决了随着核心数量增加由地址空间带来的瓶颈问题，而且由于bonsai tree所带来的系统额外的代价也是一定的，并不会随着核心的数量大量增加。

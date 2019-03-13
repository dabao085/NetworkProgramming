### C/C++网络编程  
主要用来记录网络编程学习中的过程，参考资料为《UNIX网络编程 卷1：套接字联网API》、《Linux高性能服务器编程》、《Linux多线程服务端编程 muduo》等，编译方式为  
`make`  
可在各子目录下生成指定的可执行程序，其中`server`存放的是服务器端程序，`client`存放客户端程序，`chatroom`存放聊天室程序，`include`存放以上项目的头文件，`tinyReactor`为一个独立的子项目，实现了一个简单的Reactor模式，这是服务端编程最常使用的编程模式。
#### 使用方法  
对于`server`目录下的可执行程序，设置好IP地址与端口号，如：`./serverwithepoll 127.0.0.1 6610`  
用户可以使用`telnet`工具来发起连接，连接至已开启的服务端。  
对于`chatroom`目录下的可执行程序，分别运行一个server和多个client，可以模拟群聊的效果。  
#### tinyReactor  
# newobj 通用开发库

newobj 是一个支持全面的开发库，可满足通用开发需求。

---

## 项目简介
支持Windows、Linux平台，完整编译需要 C++17 语言版本支持。集成Qt、网络、数据库、文件、日志、系统、JSON等几乎所有的通用开发需求。



<b>开发者：</b>nianhua

<b>QQ：</b>1585346868 (欢迎交流指正和构建部署答疑讨论)

<b>开源协议：</b>暂无

<b>平台支持：</b>Windows、Linux 32及64位

<b>项目结构：</b>

* bin(生成文件)
* include(三方库头文件)

* module (模块)
	- network (网络)
	- qt (qt)
* thirdparty (三方及封装)
	- clickhosue 
	- curl
	- hpsocket
	- leveldb
	- libzip
	- lua
	- mimalloc
	- mysql
	- redis
	- rpclib
	- sqlite3
	- zlib
* src (核心)
---

## 功能介绍


### 一、网络
支持Tcp、Udp、Ftp(client)、Http。其中非常推荐使用HttpServer相关用于开发Web服务应用，支持百万连接及线程池、业务用连接池配套组件。

<b>TCP</b> ：支持Pull、Pack、Push 三种模型)
	- TcpClient
	- TcpServer
	- TcpForward

<b>UDP</b>
	- UdpNode

<b>HTTP</b> ：支持Http(s)Client及Http(s)Server,服务端提供站点、路由、拦截器、处理器等
	- HttpClient
	- HttpServer
	- HttpRouter
	- HttpRequest
	- HttpResponse
	- HttpInterceptor
	- HttpParser
	- HttpWebSite
	- HttpSession
	- HttpCookie
	- HttpCenter

<b>FTP</b>
	- FtpClient

### 二、Qt
常用Qt相关封装，ui文件夹内简化了常用组件的信号槽，改为回调函数绑定。

 <font color=gray>目前支持Qt5.9.9及Qt4.8.5两个版本，使用其它版本复制头文件至 include/qt 即可</font>


<b>UI</b>
	- button
	- checkbox
	- comobox
	- highlighter
	- image
	- label
	- lineedit
	- radio
	- topbar
	- widget
### 三、数据库

封装可靠的原生客户端代码实现高效开发，提供通用连接池组件。

<b>Clickhouse</b>

<b>Mysql</b>

<b>Redis</b>

<b>LevelDB</b>

<b>Sqlite3</b>:(不支持连接池)



### 四、核心组件
核心组件即newobj.a静态库，其它封装库均应用该库头文件或函数，故链接应用程序请将该库放置到尾部。

<b>algorithm</b>:常用算法

<b>db</b>
    - sqlite3
        - ...

<b>log</b>
        - log_no : 通用日志类
        - ...

<b>public</b>: 公开
        - buffer: 字节流类
                - string: 字符串类
                - exception： 异常
                        - environment: 全局变量相关类
	- ... 

<b>util</b>
    - buiild: 源码构建函数(仅支持VC++)
    - bytes: 字节操作
    - cache: 本地缓存
    - codec: 编解码相关
    - compress: 解压缩
    - config: 配置文件(仅支持INI)
    - debug：调试异常相关
    - desktop: 虚拟桌面函数封装(仅支持Windows)
    - file: 文件操作
    - forstring: 字符串操作
    - html： HTML解析
    - idx_maker: 计数器
    - json：CJson封装类
    - keyboard: 模拟按键(仅支持Windows)
    - lock： 锁
    - map：有锁map
    - math： 数学相关
    - mem： 内存相关
    - mouse：鼠标(仅支持Windows)
    - package：字节流结构化打包
    - point：指针相关
    - point_pool：指针池
    - pool：连接池
    - print：打印
    - process: 进程相关
    - queue：有锁队列
    - system：系统相关
    - thread：线程
    - time：时间
    - timeout：定时器
    - vector：有锁vector
    - verify：验证相关
    - windows：窗口操作(仅支持Windows)


## 构建编译

生成程序链接本库请添加宏: NEWOBJ_STATIC

**功能开关**

修改 src/public/define.h 头文件

```c++
/***************************************************[功能]***************************************************/
// HPSocket 开关
#define LIB_HPSOCKET
// HPSocket 静态编译
#define HPSOCKET_STATIC_LIB
// HPSocket 支持SSL
#define LIB_HPSOCKET_SSL

// QT
#define LIB_QT
#ifdef LIB_QT
#ifdef MSVC_2010
// QT4
#define QT4
#else
// QT5
#define QT5
#endif
#endif
```

<font color=red>① HPSocket为基础网络库，使用network则必须开启该宏</font>

<font color=red>② QT为可选组件，注释 LIB_QT 可关闭QT支持</font>

### Windows
支持Vs2010及以上编译器

1、打开 newobj.sln 

2、全部构建

默认构建选项支持 VS2019、VS2010 MD及MT

<font color=red> 注意：mysql、clickhouse、redis 等请自行编译或下载官方库链接至应用程序。</font>

### Linux

G++ 需要 C++ 17 及以上语言版本支持

**开发环境：** ubuntu



**1、简易安装**

* 克隆仓库

* ```
  cd newobj/build
  chmod 777 build.sh
  ./build.sh
  ```

  

**1、完整安装**

```shell
#!/bin/bash
# 构建目录
mkdir bin
mkdir thirdparty
mkdir thirdparty/hpsocket
# 安装软件
sudo apt -y install g++
sudo apt -y install cmake
sudo apt -y install make
sudo apt -y install unzip
sudo apt -y install libhiredis-dev
sudo apt -y install libmysqlclient-dev
sudo apt -y install git
sudo apt -y install libmysqlcppconn-dev
sudo apt -y install libhiredis-dev
# 编译构建clickhosue
cd thirdparty
rm -rf clickhouse-cpp
git clone https://github.com/ClickHouse/clickhouse-cpp.git
cd clickhouse-cpp
cmake .
make -j8
sudo make install
cd ../..
cp thirdparty/clickhouse-cpp/contrib/absl/absl/libabsl_int128.a bin/
cp thirdparty/clickhouse-cpp/contrib/cityhash/cityhash/libcityhash.a bin/
cp thirdparty/clickhouse-cpp/contrib/lz4/lz4/liblz4.a bin/
cp thirdparty/clickhouse-cpp/clickhouse/libclickhouse-cpp-lib.a bin/

# 编译构建hpsocket基础网络库
cd thirdparty
rm -rf hpsocket-5.8.5-linux.zip
rm -rf hpsocket
git clone https://github.com/ldcsaa/HP-Socket.git
cd HP-Socket/Linux
chmod -R 777 *

./script/compile.sh <<EOF
Y
EOF
pwd
cd ../../..
pwd
cp ./thirdparty/HP-Socket/Linux/lib/hpsocket/x64/libhpsocket.a ./bin/
cp ./thirdparty/HP-Socket/Linux/lib/hpsocket/x64/libhpsocket_d.a ./bin/
cp ./thirdparty/HP-Socket/Linux/lib/hpsocket/x64/libhpsocket.so ./bin/
cp ./thirdparty/HP-Socket/Linux/lib/hpsocket/x64/libhpsocket_d.so ./bin/

# 构建 newobj 
git clone https://github.com/Liuccysdgg/newobj.git
chmod 777 ./newobj/build/build.sh
cd ./newobj/build
./build.sh

cp newobj/bin/libmodule_network_x64_release.a ./bin
cp newobj/bin/libnewobj_x64_release.a ./bin
cp newobj/bin/libthirdparty_mysql_x64_release.a ./bin
cp newobj/bin/libthirdparty_redis_x64_release.a ./bin
cp newobj/bin/libthirdparty_clickhouse_x64_release.a ./bin
cd ../../
```


# newobj c++ 跨平台开发库

newobj 是一个支持全面的开发库，可满足通用开发需求。

---

## 项目简介
支持Windows、Linux平台，完整编译需要 C++17 语言版本支持。集成Qt、网络、数据库、文件、日志、系统、JSON等几乎所有的通用开发需求。



**开发者:** nianhua

**QQ群:**   213222098 (欢迎技术交流和构建部署答疑讨论)

**开源协议:** 暂无

**平台支持:** Windows、Linux 32及64位

**项目下载:** https://github.com/Liuccysdgg/newobj.git

**项目结构:** 

* bin(生成文件)
* include(三方库头文件)
* module (模块)
	* network (网络)
	* qt (qt)
* thirdparty (三方及封装)
	* clickhosue 
	* curl
	* hpsocket
	* leveldb
	* libzip
	* lua
	* mimalloc
	* mysql
	* redis
	* rpclib
	* sqlite3
	* zlib
* src (核心)
---

## 功能介绍


### 一、网络
支持Tcp、Udp、Ftp(client)、Http。其中非常推荐使用HttpServer相关用于开发Web服务应用，支持百万连接及线程池、业务用连接池配套组件。

* **TCP** ：支持Pull、Pack、Push 三种模型)
	* TcpClient
	* TcpServer
	* TcpForward

* **UDP**
	* UdpNode

* **HTTP** ：支持Http(s)Client及Http(s)Server,服务端提供站点、路由、拦截器、处理器等
	* HttpClient
	* HttpServer
	* HttpRouter
	* HttpRequest
	* HttpResponse
	* HttpInterceptor
	* HttpParser
	* HttpWebSite
	* HttpSession
	* HttpCookie
	* HttpCenter

**FTP**
	* FtpClient

### 二、Qt
常用Qt相关封装，ui文件夹内简化了常用组件的信号槽，改为回调函数绑定。

 <font color=gray>目前支持Qt5.9.9及Qt4.8.5两个版本，使用其它版本复制头文件至 include/qt 即可</font>


* **UI**
	* button
	* checkbox
	* comobox
	* highlighter
	* image
	* label
	* lineedit
	* radio
	* topbar
	* widget
### 三、数据库

封装可靠的原生客户端代码实现高效开发，提供通用连接池组件。

* **Clickhouse**

* **Mysql**

* **Redis**

* **LevelDB**

* **Sqlite3**:(不支持连接池)



### 四、核心组件
核心组件即newobj.a静态库，其它封装库均应用该库头文件或函数，故链接应用程序请将该库放置到尾部。

* **algorithm**:常用算法

* **db**
	* sqlite3
	* ...

* **log**
    * log_no : 通用日志类
    * ...

* **public** :公开
    * buffer: 字节流类
    * string: 字符串类
    * exception： 异常
    * environment: 全局变量相关类
	* ... 

* **util**
    * buiild: 源码构建函数(仅支持VC++)
    * bytes: 字节操作
    * cache: 本地缓存
    * codec: 编解码相关
    * compress: 解压缩
    * config: 配置文件(仅支持INI)
    * debug：调试异常相关
    * desktop: 虚拟桌面函数封装(仅支持Windows)
    * file: 文件操作
    * forstring: 字符串操作
    * html： HTML解析
    * idx_maker: 计数器
    * json：CJson封装类
    * keyboard: 模拟按键(仅支持Windows)
    * lock： 锁
    * map：有锁map
    * math： 数学相关
    * mem： 内存相关
    * mouse：鼠标(仅支持Windows)
    * package：字节流结构化打包
    * point：指针相关
    * point_pool：指针池
    * pool：连接池
    * print：打印
    * process: 进程相关
    * queue：有锁队列
    * system：系统相关
    * thread：线程
    * time：时间
    * timeout：定时器
    * vector：有锁vector
    * verify：验证相关
    * windows：窗口操作(仅支持Windows)


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

**开发环境:** ubuntu



**编译安装**

```shell
# 下载安装脚本(建议开发环境非root运行)
wget http://file.newobj.org/build.sh
# 设置执行权限
chmod 777 build.sh
# 运行脚本
./build.sh
```


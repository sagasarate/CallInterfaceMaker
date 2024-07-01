2024-7-1：
	更改项目使用UNICODE字符编码，以支持Json，配置文件Config和TypeDef，以及环境文件Env更改为Json，以方便手动修改，但如果Json文件不存在也会尝试加载旧的XML格式文件，协议保存文件仍旧使用XML格式，因为发现改成Json格式后反而难于手动编辑。
	TypeScript的导出由单一文件更改为多文件，这样就可以支持数据对象的分块导出了。
	c++导出支持文件日志相关导出。

2023-6-23：增加TypeScript和Lua模板，使得可以导出这些语言的版本


这是一个接口代码生成工具，可以生成用于网络通讯的协议以及相关的数据结构，支持生成c++和C#的代码
所需依赖库可在https://github.com/sagasarate/EasyGameLibs中找到

This is an interface code generation tool that can be generated for network communication protocols and data structures
support the generation of c++ and C# code 
dependent lib can be found in https://github.com/sagasarate/EasyGameLibs 

### 测试

1. 修改`example/jieba_test.cpp`中的`dataBasePath`指定结巴资源路径。
2. make
3. ./test

### 结巴分词封装说明

* 3rdparty存放经过修改的分词代码，主要是从string加载
* 在实际项目中，有可能加载资源之后，在多处使用，所以添加了单例支持。但是单例资源无法释放，所以添加手动加载和释放接口。
  ```
  JiebaSegment::Instance()->LoadResource(dictBuffer, hmmBuffer,userBuffer);
  JiebaSegment::Instance()->ReleaseJiebaResource();
  ```
* cppjieba已经被封装，在实际编译完之后，不再需要该头文件。

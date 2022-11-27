# QtTranslate
​		使用 QNetworkAccessManager get请求调用百度翻译API进行文字翻译，并使用json文件对开发者账号ID和KEY进行加密存储，加密使用主机CPU序列号+随机字符作为AES加密的Key进行加密，解密时先使用CPU序列号作为key 解密出随机数后在解密开发者Key。

​		为了解析get请求的回复和解析配置文件，使用QJson相关接口进行数据解析。

​		引入日志系统。

​		最初设计该程序时打算将市面上常用的翻译API进行引入，在进行腾讯翻译API接口使用时计算签名的HmacSha256算法计算出的数据错误签名失败，根据腾讯提供的SDK仅能在linux系统使用，该项目以失败告终，待后续再完善


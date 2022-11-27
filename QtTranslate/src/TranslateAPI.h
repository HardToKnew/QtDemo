#ifndef TRANSLATEAPI_H
#define TRANSLATEAPI_H
#include <QString>
#include "BaseApi.h"
#include "api.h"

class JesonFileManage ;
class TranslateAPI
{
public:
    enum APIType
    {
        atBaidu,
        atTencent
    };

    TranslateAPI();
    ~TranslateAPI();
    void setTranslateApiType(Api::APIType  type);//设置调用Api接口模式
    void setAppid(QString Appid);
    void setKey(QString Key);
    void setDataDecryptFlag(bool flag);//设置数据加密
    QString getSplicingData(BaseApi::LanguageType from, BaseApi::LanguageType to, QString strWorld);
    Api::APIType getTranslateApiType();
    QString getUrl();
    QString getAppid();
    QString getKey();
    bool getDataDecryptFlag();//获取数据加密标志
    QString getResultData(QString input);//翻译后数据解码
private:
    Api api;
    BaseApi *BaiduApi;
    BaseApi *TencentApi;
    QString BaseKey;
    QString HashKey;
    QString fileName;
    JesonFileManage *jesonFileManage;

    QString getHashData(QString input);




};

#endif // TRANSLATEAPI_H

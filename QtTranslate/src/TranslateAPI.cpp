#include "TranslateAPI.h"
#include <QCryptographicHash>
#include <QDebug>
#include "BaiduTranslateApi.h"
#include "TencentTranslateApi.h"
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include "JesonFileManage.h"
TranslateAPI::TranslateAPI()
{
    BaiduApi = new BaiduTranslateApi;
    TencentApi = new TencentTranslateApi;
    //readJson();

    //读配置
    jesonFileManage = new JesonFileManage;
    //jesonFileManage->readJson();
    api.Type = jesonFileManage->getCurrentApiType();

    BaiduTranslateApi *baiduApi = dynamic_cast<BaiduTranslateApi*>(BaiduApi);
    baiduApi->setAppid(jesonFileManage->getBaiduAppid());
    baiduApi->setKey(jesonFileManage->getBaiduKey());
    baiduApi->setUrl(jesonFileManage->getBaiduUrl());
    dynamic_cast<TencentTranslateApi*>(TencentApi)->setAppid(jesonFileManage->getTencentAppid());
    dynamic_cast<TencentTranslateApi*>(TencentApi)->setKey(jesonFileManage->getTencentKey());
    dynamic_cast<TencentTranslateApi*>(TencentApi)->setUrl(jesonFileManage->getTencentUrl());


}

TranslateAPI::~TranslateAPI()
{
    jesonFileManage->wirteJson();
    delete BaiduApi;
    delete TencentApi;
    delete jesonFileManage;
    //qDebug()<<"退出程序"<<endl;

}

void TranslateAPI::setTranslateApiType(Api::APIType type)
{
    api.Type = type;
    jesonFileManage->setCurrentApiType(type);
    switch (api.Type) {
    case atBaidu:
        if(BaiduTranslateApi *apiTrans  = dynamic_cast<BaiduTranslateApi*>(BaiduApi))
        {
            api.Appid  = apiTrans->getAppid();
            api.Key = apiTrans->getKey();
            api.Url = apiTrans->getUrl();
        }
        break;
    case atTencent:
        if(TencentTranslateApi *apiTrans  = dynamic_cast<TencentTranslateApi*>(TencentApi))
        {
            api.Appid  = apiTrans->getAppid();
            api.Key = apiTrans->getKey();
            api.Url = apiTrans->getUrl();
        }
        break;
    default:
        break;
    }
}

void TranslateAPI::setAppid(QString Appid)
{
    switch (api.Type) {
    case atBaidu:
        jesonFileManage->setBaiduAppid(Appid);
        if(BaiduTranslateApi *api  = dynamic_cast<BaiduTranslateApi*>(BaiduApi))
            api->setAppid(Appid);
        break;
    case atTencent:
        jesonFileManage->setTencentAppid(Appid);
        if(TencentTranslateApi *api  = dynamic_cast<TencentTranslateApi*>(TencentApi))
            api->setAppid(Appid);
        break;
    default:
        break;
    }
    //setTranslateApiType(api.Type);
}

void TranslateAPI::setKey(QString Key)
{
    switch (api.Type) {
    case atBaidu:
        jesonFileManage->setBaiduKey(Key);
        if(BaiduTranslateApi *api  = dynamic_cast<BaiduTranslateApi*>(BaiduApi))
            api->setKey(Key);
        break;
    case atTencent:
        jesonFileManage->setTencentKey(Key);
        if(TencentTranslateApi *api  = dynamic_cast<TencentTranslateApi*>(TencentApi))
            api->setKey(Key);
        break;
    default:
        break;
    }
}

void TranslateAPI::setDataDecryptFlag(bool flag)
{
    jesonFileManage->setDataDecryptFlag(flag);
}

QString TranslateAPI::getSplicingData(BaseApi::LanguageType sourceType, BaseApi::LanguageType transResultType, QString strWorld)
{
    QString url;
    QString strSalt = QString::number(qrand()% 1000);
    switch (api.Type) {
    case atBaidu:
        if(BaiduTranslateApi *api  = dynamic_cast<BaiduTranslateApi*>(BaiduApi))
        {
            url = QString("%1?q=%2&from=%3&to=%4&appid=%5&salt=%6&sign=%7")
                        .arg(api->getUrl())
                        .arg(strWorld)
                        .arg(api->getLanguageString(sourceType))
                        .arg(api->getLanguageString(transResultType))
                        .arg(api->getAppid())
                        .arg(strSalt)
                        .arg(getHashData(api->getAppid()+strWorld+strSalt+api->getKey()));
        }
        break;
    case atTencent:
        break;
    default:
        break;
    }
    return url;
}

Api::APIType TranslateAPI::getTranslateApiType()
{
    return api.Type;
}

QString TranslateAPI::getUrl()
{
    return api.Url;
}

QString TranslateAPI::getAppid()
{
    return api.Appid;
}

QString TranslateAPI::getKey()
{
    return api.Key;
}

bool TranslateAPI::getDataDecryptFlag()
{
    return jesonFileManage->getDataDecryptFlag();
}

QString TranslateAPI::getResultData(QString input)
{
    //qDebug()<<input<<endl;
    QString string;
    switch (api.Type) {
    case atBaidu:
        if(BaiduTranslateApi *api  = dynamic_cast<BaiduTranslateApi*>(BaiduApi))
        {
            string = api->getResultData(input);
        }
        break;
    case atTencent:
        break;
    default:
        break;
    }
    return string;
}

QString TranslateAPI::getHashData(QString input)
{
    //返回哈希数据，第二个参数是采用何种算法
    QByteArray hashData = QCryptographicHash::hash(input.toUtf8(),QCryptographicHash::Md5);
    //返回字节数组的十六进制编码，编码使用数字0-9和字母a-f
    return QString (hashData.toHex());
}






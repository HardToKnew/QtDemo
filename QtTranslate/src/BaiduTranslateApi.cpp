#include "BaiduTranslateApi.h"
#include <QStringList>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QDebug>
BaiduTranslateApi::BaiduTranslateApi()
    : BaseApi()
{
    language <<"auto"<<"zh"<<"en"<<"de"<<"jp"<<"spa";

}

QString BaiduTranslateApi::getLanguageString(BaseApi::LanguageType type)
{
    return language.at(type);
}

QString BaiduTranslateApi::getResultData(QString input)
{
    //qDebug()<<input.toUtf8()<<endl;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(input.toUtf8().data());
    if(jsonDocument.isNull())
    {
        return "错误";
    }
    QJsonObject rootObj = jsonDocument.object();
    //QJsonObject rootObj;

    if(rootObj.contains("error_code"))
    {
        QString str = rootObj.value("error_code").toString();
        QStringList list;
        list<<"52000"<<"52001"<<"52002"<<"52003"<<"54000"<<"54001"<<"54003"<<"54004"<<"54005"
           <<"58000"<<"58001"<<"58002"<<"90107";
        QString result;
        switch (list.indexOf(str))
        {
        case 0:
            result = "成功";
            break;
        case 1:
            result ="请求超时 	 请重试";
            break;
        case 2:
            result = "系统错误 	 请重试";
            break;
        case 3:
            result = "未授权用户 	 请检查appid是否正确或者服务是否开通";
            break;
        case 4:
            result = "必填参数为空 	 请检查是否少传参数";
            break;
        case 5:
            result = "签名错误 	 请检查您的签名生成方法";
            break;
        case 6:
            result = "访问频率受限 	 请降低您的调用频率，或进行身份认证后切换为高级版/尊享版";
            break;
        case 7:
            result = "账户余额不足 	 请前往管理控制台为账户充值";
            break;
        case 8:
            result = "长query请求频繁 	 请降低长query的发送频率，3s后再试";
            break;
        case 9:
            result = "客户端IP非法 	 检查个人资料里填写的IP地址是否正确，可前往开发者信息-基本信息修改";
            break;
        case 10:
            result = "译文语言方向不支持 	 检查译文语言是否在语言列表里 ";
            break;
        case 11:
            result = "服务当前已关闭 	 请前往管理控制台开启服务";
            break;
        default:
            result = "认证未通过或未生效 	 请前往我的认证查看认证进度";
            break;
        }
        return result;

    }

    if(!rootObj.contains("trans_result")
         || rootObj["trans_result"].toArray().isEmpty()
         || !rootObj["trans_result"].toArray()[0].toObject().contains("dst")) { return "错误"; }

        return rootObj["trans_result"].toArray()[0].toObject()["dst"].toString();
}

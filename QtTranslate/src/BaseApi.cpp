#include "BaseApi.h"
#include <QDebug>
BaseApi::BaseApi()
{

}

void BaseApi::setAppid(QString appid)
{
    Appid = appid;
    //qDebug()<<"Appid: "<<appid<<endl;

}

void BaseApi::setKey(QString key)
{
    Key = key;
    //qDebug()<<"Key: "<<key<<endl;
}

void BaseApi::setUrl(QString url)
{
    Url = url;
    //qDebug()<<"Url: "<<url<<endl;
}

QString BaseApi::getAppid() const
{
    return Appid;
}

QString BaseApi::getKey() const
{
    return Key;
}

QString BaseApi::getUrl() const
{
    return Url;
}

#ifndef BASEAPI_H
#define BASEAPI_H
#include <QString>
#include <QStringList>
class BaseApi
{
public:
    enum  LanguageType
    {
        Auto,
        zh,
        en,
        de,
        jp,
        spa
    };
public:
    BaseApi();
    virtual void setAppid(QString appid);
    virtual void setKey(QString key);
    virtual void setUrl(QString url);

    virtual QString getAppid() const;
    virtual QString getKey() const;
    virtual QString getUrl() const;
    virtual QString getLanguageString(BaseApi::LanguageType type) = 0;//获取语言简称
protected:
    QString Appid = "";
    QString Key = "";
    QString Url = "";
    QStringList language = {"auto","zh","en","de","jp","spa"};
};

#endif // BASEAPI_H

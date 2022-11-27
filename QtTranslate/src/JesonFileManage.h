#ifndef JESONFILEMANAGE_H
#define JESONFILEMANAGE_H
#include "api.h"
#include <QString>
class JesonFileManage
{
public:
    enum EncryptionTimeType
    {
        everyRead,//每次读文件更新密钥
        everyWire//每次写入文件更新密钥
    };
    JesonFileManage();
    void readJson();
    void wirteJson();

    Api::APIType getCurrentApiType() const;
    bool getDataDecryptFlag();
    QString getBaiduAppid() const;
    QString getBaiduKey() const;
    QString getBaiduUrl() const;
    QString getTencentAppid() const;
    QString getTencentKey() const;
    QString getTencentUrl() const;

    void setCurrentApiType(Api::APIType type);
    void setDataDecryptFlag(bool flag);
    void setBaiduAppid(QString Appid);
    void setBaiduKey(QString Key);
    void setTencentAppid(QString Appid);
    void setTencentKey(QString Key);

private:

    QString fileName;


    //加密
    bool bEncryption = false;//加密标志
    EncryptionTimeType  EncrypTimeType = everyRead;
    QString baseHashKey;//基础加密密钥 被加密密钥
    QString hashKey;

    Api BaiduApi;
    Api TencentApi;

    Api::APIType CurrentApi = Api::atBaidu;

//Encryption to decrypt
    QString encryptBaseHashKey();
    QString decryptBaseHashKey(QString BaseHashKey);

    QString encrypt(QString text, QString Key);
    QString decrypt(QString text, QString Key);


};

#endif // JESONFILEMANAGE_H

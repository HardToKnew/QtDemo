#include "JesonFileManage.h"
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <Qtime>
#include <QDebug>
#include "qaesencryption.h"
#include "ComputerInfo.h"
#include <QCryptographicHash>
JesonFileManage::JesonFileManage()
{
    readJson();
}

void JesonFileManage::readJson()
{
    QString AppPath = QCoreApplication::applicationDirPath();
    QString FilePath = AppPath + "/config/";
    QString Path = FilePath.left(FilePath.lastIndexOf('/'));
    fileName =  Path+"/config.json";
    //文件是否存在
    if(!QFile::exists(FilePath.toUtf8().data()))
    {
        //qDebug()<<"文件不存在"<<endl;
        QDir dir(Path);
        if(!dir.exists(Path.toUtf8().data()))
        {
            //路径不存在，则创建路径
            //qDebug()<<"创建路径"<<endl;
            if(!dir.mkdir(Path.toUtf8().data()))
                return;//路径创建失败就没办法了，通常不会失败
        }
    }
    //qDebug()<<fileName<<endl;
    QFile file( fileName.toUtf8().data() );
    if ( !file.open( QIODevice::ReadWrite ) ) {
            qDebug() << "文件打开失败!\n";
            exit( 1 );
    }
        //qDebug() << "文件打开成功\n";

        // 读取文件的全部内容
        QTextStream stream(&file);
        stream.setCodec("UTF-8");		// 设置读取编码是UTF8
        QString str = stream.readAll();
        file.close();

        QJsonParseError jsonParserError;
        QJsonDocument   jsonDocument =
        QJsonDocument::fromJson(str.toUtf8(), &jsonParserError );

        if ( !jsonDocument.isNull() &&
             jsonParserError.error != QJsonParseError::NoError ) {
            qDebug() << "文件解析错误\n";
            return;
        }

        ComputerInfo com;
        QJsonObject rootObj = jsonDocument.object();
        baseHashKey = rootObj.value("HashKey").toString();
        //qDebug()<<"HashKey: "<<baseHashKey<<endl;
        QString KeyNum = com.getCpuProcessorId()+com.getBiosSerialNum() + decryptBaseHashKey(baseHashKey);
        bEncryption = rootObj.value("Encryption").toBool();
        if(rootObj.value("EncryptionTimeType").toInt() == 0)
            EncrypTimeType == everyRead;
        else
            EncrypTimeType = everyWire;
        CurrentApi = (Api::APIType)rootObj.value("CurrentApiType").toInt();
        QJsonValue ApiTypeValue = rootObj.value("ApiType");
        if(ApiTypeValue.type() == QJsonValue::Object)
        {
            QJsonObject ApiTypeObj = ApiTypeValue.toObject();
            QStringList ApiTypeList = ApiTypeObj.keys();
            for(QJsonObject::iterator it = ApiTypeObj.begin(); it!= ApiTypeObj.end(); it++)
            {
                QString current = it.key();
                QJsonValue Value = ApiTypeObj.value(current);
                if(Value.type() == QJsonValue::Object)
                {
                    QJsonObject Obj = Value.toObject();
                    QString Appid = Obj.value("Appid").toString();
                    QString Key = Obj.value("Key").toString();
                    QString url = Obj.value("Url").toString();

                    if(bEncryption == true)//数据已加密
                    {
                        Appid = decrypt(Appid, KeyNum);
                        Key = decrypt(Key, KeyNum);
                        //url = decrypt(url, KeyNum);
                    }
                    QStringList Options;
                    Options <<"baidu"<<"tencent";
                    switch (Options.indexOf(current)) {
                    case 0:
                        BaiduApi.Appid = Appid;
                        BaiduApi.Key = Key;
                        BaiduApi.Url = url;
                        break;
                    case 1:
                        TencentApi.Appid = Appid;
                        TencentApi.Key = Key;
                        TencentApi.Url = url;
                        break;
                    default:
                        break;
                    }
                    //qDebug()<<current<<": "<<endl;
                    //qDebug()<<"\tAppid: "<<Appid<<endl;
                    //qDebug()<<"\tKey: "<<Key<<endl;
                    //qDebug()<<"\tUrl: "<<url<<endl;

                }

            }

        }
        if(EncrypTimeType == everyRead)
        {
           baseHashKey =  encryptBaseHashKey();
           decryptBaseHashKey(baseHashKey);
        }
}

void JesonFileManage::wirteJson()
{
    QString AppPath = QCoreApplication::applicationDirPath();
    QString FilePath = AppPath + "/config/";
    QString Path = FilePath.left(FilePath.lastIndexOf('/'));
    fileName =  Path+"/config.json";
    //文件是否存在
    if(!QFile::exists(FilePath.toUtf8().data()))
    {
        QDir dir(Path);
        if(!dir.exists(Path.toUtf8().data()))
        {
            //路径不存在，则创建路径
            if(!dir.mkdir(Path.toUtf8().data()))
                return;//路径创建失败就没办法了，通常不会失败
        }
    }
    QFile file( fileName.toUtf8().data() );
    if ( !file.open( QIODevice::ReadWrite ) ) {
            qDebug() << "文件打开失败!\n";
            exit( 1 );
    }

    ComputerInfo com;
    QString KeyNum = com.getCpuProcessorId()+com.getBiosSerialNum() + decryptBaseHashKey(baseHashKey);

    //百度
    QJsonObject baiduObj;
    QString Appid = BaiduApi.Appid;
    QString Key = BaiduApi.Key;
    QString Url = BaiduApi.Url;
    //bEncryption = false;
    if(bEncryption)
    {
        Appid =  encrypt(Appid,KeyNum);
        Key = encrypt(Key, KeyNum);
    }
    baiduObj.insert("Appid", Appid);
    baiduObj.insert("Key", Key);
    baiduObj.insert("Url", Url);

    QJsonObject tencentObj;
    Appid = TencentApi.Appid;
    Key = TencentApi.Key;
    Url = TencentApi.Url;
    if(bEncryption)
    {
        Appid =  encrypt(Appid,KeyNum);
        Key = encrypt(Key, KeyNum);
    }
    tencentObj.insert("Appid", Appid);
    tencentObj.insert("Key", Key);
    tencentObj.insert("Url", Url);

    QJsonObject apiTypeObj;
    // 插入元素，对应键值对
    apiTypeObj.insert("baidu", baiduObj);
    apiTypeObj.insert("tencent", tencentObj);


    QJsonObject rootObj;
    rootObj.insert("ApiType", apiTypeObj);
    rootObj.insert("HashKey", baseHashKey);
    rootObj.insert("Encryption", bEncryption);
    rootObj.insert("EncryptionTimeType", (int)EncrypTimeType);
    rootObj.insert("CurrentApiType", (int)CurrentApi);
    //rootObj.insert("ApiType", apiTypeObj);

    // 将修改后的内容写入文件
    QJsonDocument   doc;
    doc.setObject(rootObj);
    QTextStream wirteStream(&file);
    wirteStream.setCodec("UTF-8");		// 设置读取编码是UTF8
    wirteStream << doc.toJson();		// 写入文件
    file.close();					// 关闭文件*/

    /*QByteArray byte_array = doc.toJson();
    file.write(byte_array.to);
    file.close();*/
}

Api::APIType JesonFileManage::getCurrentApiType() const
{
    return CurrentApi;
}

bool JesonFileManage::getDataDecryptFlag()
{
    return bEncryption;
}

QString JesonFileManage::getBaiduAppid() const
{
    return BaiduApi.Appid;
}

QString JesonFileManage::getBaiduKey() const
{
    return BaiduApi.Key;
}

QString JesonFileManage::getBaiduUrl() const
{
    return BaiduApi.Url;
}

QString JesonFileManage::getTencentAppid() const
{
    return TencentApi.Appid;
}

QString JesonFileManage::getTencentKey() const
{
    return TencentApi.Key;
}

QString JesonFileManage::getTencentUrl() const
{
    return TencentApi.Url;
}
void JesonFileManage::setCurrentApiType(Api::APIType type)
{
    CurrentApi = type;
}

void JesonFileManage::setDataDecryptFlag(bool flag)
{
    bEncryption = flag;
}

void JesonFileManage::setBaiduAppid(QString Appid)
{
    BaiduApi.Appid = Appid;
}

void JesonFileManage::setBaiduKey(QString Key)
{
    BaiduApi.Key  = Key;
}

void JesonFileManage::setTencentAppid(QString Appid)
{
    TencentApi.Appid = Appid;
}

void JesonFileManage::setTencentKey(QString Key)
{
    TencentApi.Key  = Key;
}


QString JesonFileManage::encryptBaseHashKey()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));          //设置随机数种子
    int max = 999999;
    int min = 100000;
    QString rand = QString::number(qrand() % (max-min));

    ComputerInfo com;
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB);
    QString key = com.getCpuProcessorId()+com.getBiosSerialNum();
    QByteArray hashKey = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Md5);
    QByteArray encoded = encryption.encode(rand.toUtf8(), hashKey);

    QString encodedText = encoded.toBase64();
    //encoded =  QByteArray::fromBase64(encodedText.toUtf8());
    //qDebug()<<"Rand: "<<rand<<endl;
    //qDebug()<<"key: "<<hashKey.toBase64()<<"    encodedText: "<<encoded.toBase64()<< " HEX:"<<encoded.toBase64().toHex()<<endl;
    //qDebug()<<"key: "<<hashKey.toBase64()<<"    encodedText: "<<encodedText<< " HEX:"<<QByteArray::fromBase64(encodedText.toUtf8()).toBase64().toHex()<<endl;
    return encodedText;
}

QString JesonFileManage::decryptBaseHashKey(QString BaseHashKey)//解密
{
    QByteArray encoded = QByteArray::fromBase64(BaseHashKey.toUtf8());
    ComputerInfo com;
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB);
    QString key = com.getCpuProcessorId()+com.getBiosSerialNum();
    QByteArray hashKey = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Md5);
    QByteArray decoded = encryption.decode(encoded, hashKey);
    QString decodedText = QString::fromUtf8(QAESEncryption::RemovePadding(decoded));//移除填充；
    //qDebug()<<"HashKey: "<<BaseHashKey<<"  key: "<<hashKey.toBase64()<<"    encodedText: "<<encoded.toBase64()<< " HEX:"<<encoded.toBase64().toHex()<<endl;
    //qDebug()<<"decodedText:" <<decodedText<<": "<<decodedText.toUtf8().toHex()<<endl;
    return decodedText;
}

QString JesonFileManage::encrypt(QString text, QString Key)
{
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB);
    QByteArray hashKey = QCryptographicHash::hash(Key.toUtf8(), QCryptographicHash::Md5);
    QByteArray encoded = encryption.encode(text.toUtf8(), hashKey);
    QString encodedText = encoded.toBase64();
    //qDebug()<<"Rand: "<<text<<endl;
    //qDebug()<<"key: "<<hashKey.toBase64()<<"    encodedText: "<<encoded.toBase64()<< " HEX:"<<encoded.toBase64().toHex()<<endl;
    //qDebug()<<"key: "<<hashKey.toBase64()<<"    encodedText: "<<encodedText<< " HEX:"<<QByteArray::fromBase64(encodedText.toUtf8()).toBase64().toHex()<<endl;
    return encodedText;
}

QString JesonFileManage::decrypt(QString text, QString Key)
{
    QByteArray encoded = QByteArray::fromBase64(text.toUtf8());
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB);
    QByteArray hashKey = QCryptographicHash::hash(Key.toUtf8(), QCryptographicHash::Md5);
    QByteArray decoded = encryption.decode(encoded, hashKey);
    QString decodedText = QString::fromUtf8(QAESEncryption::RemovePadding(decoded));//移除填充；
    //qDebug()<<"decodedText:" <<decodedText<<endl;
    return decodedText;
}


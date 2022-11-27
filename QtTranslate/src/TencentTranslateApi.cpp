#include "TencentTranslateApi.h"
#include <string>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include <QDateTime>
#include <QDebug>
using namespace  std;
string get_data(int64_t &timestamp)
{
    string utcDate;
    char buff[20] = { 0 };
    // time_t timenow;
    struct tm sttime;
    //sttime = *gmtime(&timestamp);
    strftime(buff, sizeof(buff), "%Y-%m-%d", &sttime);
    utcDate = string(buff);
    return utcDate;
}
QString HexEncode(QString &input)
{
    static const char* const lut = "0123456789abcdef";
    size_t len = input.length();

    QString output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input.toStdString().c_str()[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}
TencentTranslateApi::TencentTranslateApi()
{
    //自动 中文 英语 德语 日语 西班牙语
    language <<"auto"<<"zh"<<"en"<<"de"<<"ja"<<"es";

    /*
https://cvm.tencentcloudapi.com/?Limit=10&Offset=0

Authorization: TC3-HMAC-SHA256 Credential=AKID********EXAMPLE/2018-10-09/cvm/tc3_request, SignedHeaders=content-type;host, Signature=5da7a33f6993f0614b047e5df4582db9e9bf4672ba50567dba16c6ccf174c474
Content-Type: application/x-www-form-urlencoded
Host: cvm.tencentcloudapi.com
X-TC-Action: DescribeInstances
X-TC-Version: 2017-03-12
X-TC-Timestamp: 1539084154
X-TC-Region: ap-guangzhou

*/
        QString SECRET_ID = "AKIDz8krbsJ5yKBZQpn74WFkmLPx3*******";
        QString SECRET_KEY = "Gu5t9xGARNpq86cd98joQYCN3*******";

        QString service = "cvm";
        QString host = "cvm.tencentcloudapi.com";
        QString region = "ap-guangzhou";
        QString action = "DescribeInstances";
        QString version = "2017-03-12";
        int64_t timestamp = 1551113065;
        QDateTime time ;
        time = QDateTime::fromTime_t(timestamp);
        time = time.toUTC();
        //qDebug()<<time.date().year()<<time.date().month()<<time.date().day()<<endl;
        QString date = QString::asprintf("%04d-%02d-%02d",time.date().year(),time.date().month(),time.date().day());
        //qDebug()<<date<<endl;
        time = QDateTime::currentDateTime();   //获取当前时间
        int timeT = time.toTime_t();   //将当前时间转为时间戳
        QCryptographicHash SHA256(QCryptographicHash::Sha256);

        // ************* 步骤 1：拼接规范请求串 *************
        QString httpRequestMethod = "POST";
        QString canonicalUri = "/";
        QString canonicalQueryString = "";
        QString canonicalHeaders = "content-type:application/json; charset=utf-8\nhost:" + host + "\n";
        QString signedHeaders = "content-type;host";
        QString payload = "{\"Limit\": 1, \"Filters\": [{\"Values\": [\"\\u672a\\u547d\\u540d\"], \"Name\": \"instance-name\"}]}";
        SHA256.addData(payload.toLatin1().data());
        QString hashedRequestPayload = SHA256.result().toHex().toLower();//sha256Hex(payload);
        QString canonicalRequest = httpRequestMethod + "\n" + canonicalUri + "\n" + canonicalQueryString + "\n"
            + canonicalHeaders + "\n" + signedHeaders + "\n" + hashedRequestPayload;
        //qDebug() << canonicalRequest << endl;
         //qDebug()<<date<<endl;
        // ************* 步骤 2：拼接待签名字符串 *************
        QString algorithm = "TC3-HMAC-SHA256";
        QString RequestTimestamp = QString::number(timestamp);
        QString credentialScope = date + "/" + service + "/" + "tc3_request";
        SHA256.reset();
        SHA256.addData(canonicalRequest.toLatin1().data());
        QString hashedCanonicalRequest = SHA256.result().toHex().toLower();
        QString stringToSign = algorithm + "\n" + RequestTimestamp + "\n" + credentialScope + "\n" + hashedCanonicalRequest;
        //qDebug() << stringToSign << endl;

        // ************* 步骤 3：计算签名 ***************
        QString kKey = "TC3" + SECRET_KEY;
        //qDebug()<<kKey<<"  "<<date<<endl;
        //qDebug()<<date.toLatin1()<<"  "<<kKey.toLatin1()<<endl;
        QString kDate = QMessageAuthenticationCode::hash(date.toUtf8(), kKey.toUtf8(), QCryptographicHash::Sha256).toBase64();
        //qDebug()<<kDate.toUtf8().toHex()<<endl;

        //qDebug()<<service.toLatin1()<<"  "<<kDate.toLatin1()<<endl;
        QString kService = QMessageAuthenticationCode::hash(service.toUtf8(), kDate.toUtf8(), QCryptographicHash::Sha256).toBase64();
        //qDebug()<<kService.toUtf8().toHex()<<endl;

        QString tc3_request= "tc3_request";
        //qDebug()<<tc3_request.toLatin1()<<"  "<<kService.toLatin1()<<endl;
        QString kSigning = QMessageAuthenticationCode::hash(tc3_request.toUtf8(), kService.toUtf8(), QCryptographicHash::Sha256).toBase64();
        //qDebug()<<kSigning.toUtf8().toHex()<<endl;
        //qDebug()<<stringToSign.toUtf8()<<"  "<<kSigning<<endl;
        QString signature = QMessageAuthenticationCode::hash(stringToSign.toUtf8(), kSigning.toUtf8(), QCryptographicHash::Sha256).toBase64();
        //string signature = HexEncode(HmacSha256(kSigning, stringToSign));
        //qDebug()<<signature.toLatin1().toHex()<<endl;
        //qDebug()<<signature.toUtf8()<<endl;
        //qDebug() << HexEncode(signature) << endl;
        signature = "e0ee7a0aa1472f29edc0bced928eb8bf112086a9d8b8986c2389f3765f27b7e9";
        //qDebug() << HexEncode(signature) << endl;




        // ************* 步骤 4：拼接 Authorization *************
        /*string authorization = algorithm + " " + "Credential=" + SECRET_ID + "/" + credentialScope + ", "
            + "SignedHeaders=" + signedHeaders + ", " + "Signature=" + signature;
        cout << authorization << endl;

        string curlcmd = "curl -X POST https://" + host + "\n"
            + " -H \"Authorization: " + authorization + "\n"
            + " -H \"Content-Type: application/json; charset=utf-8\"" + "\n"
            + " -H \"Host: " + host + "\n"
            + " -H \"X-TC-Action: " + action + "\n"
            + " -H \"X-TC-Timestamp: " + RequestTimestamp + "\n"
            + " -H \"X-TC-Version: " + version + "\n"
            + " -H \"X-TC-Region: " + region + "\n"
            + " -d '" + payload;
        cout << curlcmd << endl;*/
}

QString TencentTranslateApi::getLanguageString(BaseApi::LanguageType type)
{
    return language.at(type);
}



#ifndef API_H
#define API_H
#include <QString>

class Api
{
public:
    enum APIType
    {
        atBaidu,
        atTencent
    };

    QString Appid = "";
    QString Key = "";
    QString Url = "";
    APIType Type = atBaidu;
};

#endif // API_H

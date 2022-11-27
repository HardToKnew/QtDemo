#ifndef BAIDUTRANSLATEAPI_H
#define BAIDUTRANSLATEAPI_H
#include "BaseApi.h"

class BaiduTranslateApi : public BaseApi
{
public:
    BaiduTranslateApi();

    QString getLanguageString(BaseApi::LanguageType type);
    QString getResultData(QString input);

private:

};

#endif // BAIDUTRANSLATEAPI_H

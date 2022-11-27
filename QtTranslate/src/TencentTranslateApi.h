#ifndef TENCENTTRANSLATEAPI_H
#define TENCENTTRANSLATEAPI_H
#include "BaseApi.h"

class TencentTranslateApi : public BaseApi
{
public:
    TencentTranslateApi();
    QString getLanguageString(BaseApi::LanguageType type);
};

#endif // TENCENTTRANSLATEAPI_H

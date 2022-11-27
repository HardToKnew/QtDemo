#include "QtTranslate.h"
#include "ui_qttranslate.h"
#include "TranslateAPI.h"
#include "src/log/LogHandler.h"
#include <QTextCodec>
#include <QStyledItemDelegate>
#define MY_APID 20201230000659249 //这个位置定义你的开发账号如 2015063000000001
#define MY_APID_KEY "Xh9LUT3drK1AB5YqC5yr" //这个位置定义你的开发密匙如： 12345678
QtTranslate::QtTranslate(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtTranslate)
{
    Singleton<LogHandler>::getInstance().installMessageHandler();
    ui->setupUi(this);

    api = new  TranslateAPI;
    ui->cbApiType->setCurrentIndex(int (api->getTranslateApiType()));
    ui->actConfigData->setChecked(api->getDataDecryptFlag());
    //验证是否支持OpenSSL
    bool bSupp = QSslSocket::supportsSsl();
    QString buildVersion = QSslSocket::sslLibraryBuildVersionString();
    QString version = QSslSocket::sslLibraryVersionString();
    //qDebug() << bSupp << buildVersion << version << endl;
    //QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    //qDebug() << accessManager->supportedSchemes();


    m_manager = new QNetworkAccessManager(this);//新建QNetworkAccessManager对象
    connect(m_manager, SIGNAL(finished(QNetworkReply    *)), this, SLOT(slot_replayFinished(QNetworkReply*))); //关联信号和槽



}

QtTranslate::~QtTranslate()
{
    delete ui;
    delete m_manager;
    delete api;
}

void QtTranslate::slot_replayFinished(QNetworkReply *reply)
{
    QTextCodec *codec = QTextCodec::codecForName("utf8");
        //使用utf8编码, 这样可以显示中文
    QString str = codec->toUnicode(reply->readAll());

    ui->textBrowser->setText(api->getResultData(str).toUtf8());
    reply->deleteLater();//最后要释放replay对象
}

void QtTranslate::on_actSetting_triggered()//设置翻译接口
{
    ui->stackedWidget->setCurrentIndex(SettingApiW);
    int index = ui->cbApiType->currentIndex();
    ui->cbApiType->currentIndexChanged(index);
}

void QtTranslate::on_btnOK_clicked()
{
    ui->stackedWidget->setCurrentIndex(MainTranslateW);
    QString Appid  = ui->ledtAppID->text();
    QString Key = ui->ledtKey->text();
    api->setAppid(Appid);
    api->setKey(Key);
}

void QtTranslate::on_btnTranslate_clicked()
{
    //TranslateAPI api;
    QString world = ui->textEdit->toPlainText();
    ui->textBrowser->clear();
    if(!world.isEmpty())
    {
        QString myurl;
        myurl = api->getSplicingData((BaseApi::LanguageType)SourceLanguage
                                    ,(BaseApi::LanguageType)TransResultLanguage,world);
       //qDebug()<<myurl<<endl;
       m_manager->get(QNetworkRequest(QUrl(myurl)));//发送上传
    }
}

void QtTranslate::on_cbxSource_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        SourceLanguage = BaseApi::Auto;
        break;
    case 1:
        SourceLanguage = BaseApi::zh;
        break;
    case 2:
        SourceLanguage = BaseApi::en;
        break;
    default:
        SourceLanguage = BaseApi::Auto;
        break;
    }
    SourceLanguage = BaseApi::LanguageType(index);
}

void QtTranslate::on_cbxTransResult_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        TransResultLanguage = BaseApi::zh;
        break;
    case 1:
        TransResultLanguage = BaseApi::en;
        break;
    default:
        TransResultLanguage = BaseApi::zh;
        break;
    }
    TransResultLanguage = BaseApi::LanguageType(index+1);
}

void QtTranslate::on_cbApiType_currentIndexChanged(int index)
{
    //qDebug()<<"cbApiType_currentIndexChanged"<<endl;
    switch (index) {
    case 0:
        //TransResultLanguage = BaseApi::zh;
        api->setTranslateApiType(Api::atBaidu);
        break;
    case 1:
        api->setTranslateApiType(Api::atTencent);
        break;
    default:
        api->setTranslateApiType(Api::atBaidu);
        break;
    }
    ui->ledtAppID->setText(api->getAppid());
    ui->ledtKey->setText(api->getKey());
}

void QtTranslate::on_actConfigData_toggled(bool arg1)
{
    api->setDataDecryptFlag(arg1);
}

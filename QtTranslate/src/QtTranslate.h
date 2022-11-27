#ifndef QTTRANSLATE_H
#define QTTRANSLATE_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
QT_BEGIN_NAMESPACE
namespace Ui { class QtTranslate; }
QT_END_NAMESPACE
class TranslateAPI;
class QtTranslate : public QMainWindow
{
    Q_OBJECT
    enum StackedWidgetStyle
    {
        MainTranslateW = 0,
        SettingApiW = 1
    };

public:
    QtTranslate(QWidget *parent = nullptr);
    ~QtTranslate();

private:
    Ui::QtTranslate *ui;
    QNetworkAccessManager *m_manager;
    int SourceLanguage = 0;//源语言
    int TransResultLanguage = 1;//目标语言
    TranslateAPI *api;

private slots:
    void slot_replayFinished(QNetworkReply *reply);
    void on_actSetting_triggered();
    void on_btnOK_clicked();
    void on_btnTranslate_clicked();
    void on_cbxSource_currentIndexChanged(int index);
    void on_cbxTransResult_currentIndexChanged(int index);
    void on_cbApiType_currentIndexChanged(int index);
    void on_actConfigData_toggled(bool arg1);
};
#endif // QTTRANSLATE_H

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:

    Dialog(QString app_id, QString app_key, QWidget* parent = nullptr);
    virtual void paintEvent(QPaintEvent*);
    ~Dialog();

private:
    Dialog(QWidget *parent = nullptr);
    Ui::Dialog *ui;

private:
    QString from_string = "中文";
    QString to_string = "英文";
    QNetworkAccessManager* manger;
    QString app_id;
    QString app_key;

private:
    void change_translation();
    void translation();
    void set_translation_res(QNetworkReply* reply);
    void show_error_code(const QJsonObject& obj);
};
#endif // DIALOG_H

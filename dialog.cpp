#include "dialog.h"
#include "ui_dialog.h"
#include <QCryptographicHash>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QStyleOption>
#include <QPainter>
#include <QPixmap>

QByteArray getMd5(QString s);
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    Qt::WindowFlags flag = Qt::Dialog;
    flag |= Qt::WindowMinimizeButtonHint;
    flag |= Qt::WindowCloseButtonHint;
    setWindowFlags(flag);
    this->setFixedSize(this->width(), this->height());
    connect(ui->change_button, &QPushButton::pressed, this, &Dialog::change_translation);
    connect(ui->submit_button, &QPushButton::pressed, this, &Dialog::translation);
    manger = new QNetworkAccessManager(this);
    connect(manger, &QNetworkAccessManager::finished, this, &Dialog::set_translation_res);

    ui->from_edit->setAlignment(Qt::AlignHCenter);
    ui->to_edit->setAlignment(Qt::AlignHCenter);
}

Dialog::Dialog(QString app_id, QString app_key, QWidget *parent)
    : Dialog(parent)
{
    this->app_id = std::move(app_id);
    this->app_key = std::move(app_key);
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::change_translation() {
    this->from_string.swap(this->to_string);
    ui->from_label->setText(from_string);
    ui->to_label->setText(this->to_string);
}

void Dialog::translation() {
    srand(time(nullptr));
    const QString str = ui->from_edit->text();
    if(str.isEmpty()) return;
    const QString parm_from = from_string == "中文" ? "zh" : "en";
    const QString parm_to = to_string == "中文" ? "zh" : "en";
    const int random = rand();
    const QString base_url = "http://api.fanyi.baidu.com/api/trans/vip/translate?q=%1&from=%2&to=%3&appid=%4&salt=%5&sign=%6";

    QString parm = app_id + str + QString::number(random) + app_key;
    QString parm_md5 = getMd5(parm);
    QString url = base_url.arg(str).arg(parm_from).arg(parm_to)
            .arg(app_id).arg(random).arg(parm_md5);

    QNetworkRequest request(url);
    manger->get(request);
}

void Dialog::set_translation_res(QNetworkReply* reply) {
    auto tmp = reply->readAll();
    QJsonObject res = QJsonDocument::fromJson(tmp).object();

    if(res["error_code"].isString()) {
        show_error_code(res);
        return;
    }
    QString s = res["trans_result"].toArray().first().toObject()["dst"].toString();
    ui->to_edit->setText(s);
}

void Dialog::show_error_code(const QJsonObject& obj) {
    QString error_code = "error code: " + obj["error_code"].toString();
    QString error_message = "error message: " + obj["error_msg"].toString();
    QMessageBox::critical(this, "错误", error_code + "\n" + error_message, QMessageBox::Cancel);
}

void Dialog::paintEvent(QPaintEvent* ) {
//    QPixmap map(":/image/aili.png");
//    map.scaled(this->size());
//    QPainter painter(this);
//    painter.drawPixmap(this->rect(), map);
}

QByteArray getMd5(QString s) {
    return QCryptographicHash::hash(s.toUtf8(), QCryptographicHash::Md5).toHex();
}

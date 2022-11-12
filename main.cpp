#include "dialog.h"

#include <QApplication>

#include <QFile>
#include <QJsonDocument>
#include <QMessageBox>

QString read_json_key(const QJsonDocument& doc, const QString& key) {
    auto value = doc[key];
    if(!value.isString()) {
        QMessageBox::critical(nullptr, "读取配置文件错误", "config.json文件内容错误", QMessageBox::Cancel);
        qApp->exit();
    }

    return value.toString();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile config_file("./config.json");
    config_file.open(QIODevice::ReadOnly);

    if(!config_file.isOpen()) {
        QMessageBox::critical(nullptr, "读取配置文件错误", "读取 config.json 文件出错", QMessageBox::Cancel);
        a.exit();
        return 0;
    }

    QJsonDocument doc = QJsonDocument::fromJson(config_file.readAll());
    QString appkey = read_json_key(doc, "appkey");
    QString appid = read_json_key(doc, "appid");
    config_file.close();

    QFile qss_file(":/qss/main.qss");

    qss_file.open(QIODevice::ReadOnly);
    if(!qss_file.isOpen()) {
          qDebug() << "open style failed";
    }
    QString style = qss_file.readAll();
    qApp->setStyleSheet(style);
    qss_file.close();
    Dialog w(appid, appkey);
    w.show();
    return a.exec();
}

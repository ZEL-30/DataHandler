#include "do_order.h"

#include "utils/utils.h"

#include <qdebug>
#include <qdir>
#include <qfile>
#include <qstringlist>

DoOrder::DoOrder(Path *path)
    : path_(path) {}

DoOrder::~DoOrder() {}

bool DoOrder::authenticationDir(QString data_filename, QString header, QString data) {

    // 创建鉴权文件夹
    createFolder(path_->authenticationPath());

    // 将首条个人化数据文件移动到鉴权文件夹
    if (!dataToAuthDir(data_filename, header, data)) return false;

    // 总部脚本文件夹拷贝到鉴权文件夹下
    if (!copyFolder(path_->zhScriptPath(), path_->scriptPath(), true)) {
        return false;
    }

    return true;
}

bool DoOrder::screenshotDir(QString filename) {

    // 创建截图文件夹
    createFolder(path_->screenshotPath());

    QFile file(path_->screenshotPath() + "/" + filename);
    if (!file.open(QIODevice::WriteOnly)) return false;
    file.close();

    return true;
};

bool DoOrder::printDir() {

    // 创建打印文件夹
    createFolder(path_->printPath());

    // 拷贝需要打印的文件
    QString filename;
    for (auto zh_print_path : path_->zhPrintPaths()) {
        filename = zh_print_path.mid(zh_print_path.lastIndexOf("/") + 1);
        if (!copyFile(zh_print_path, path_->printPath() + "/" + filename, true)) return false;
    }

    return true;
}

bool DoOrder::tagDataDir() {

    // 创建标签数据文件夹
    createFolder(path_->tagDataPath());

    // 拷贝标签数据
    copyFolder(path_->zhTagDataPath(), path_->tagDataPath(), true);

    // 压缩文件
    if (!compressionZipFile(path_->tagDataPath())) return false;

    // 删除原文件
    if (!deleteFileOrFolder(path_->tagDataPath())) return false;

    return true;
}

bool DoOrder::clearScriptDir(QString filename, QString clear_script, QString atr3) {

    // 创建MD5清卡文件夹
    createFolder(path_->clearCardPath());

    QFile file(path_->clearCardPath() + "/" + filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(" [DATA]\r\nE_DataFormat=ds.NUMBER:20,ds.CRC:0\r\nMD5=\r\nATR3=");
        file.write((atr3 + "\r\n").toStdString().c_str());
        file.write(".START=1\r\n");
        file.write(clear_script.toStdString().c_str());

        file.close();
        return true;
    }

    return false;
}

bool DoOrder::dataToAuthDir(QString data_filename, QString header, QString data) {

    QFile file(path_->authenticationPath() + "/" + data_filename);
    if (file.open(QIODevice::WriteOnly)) {
        header += "\r\n";
        data += "\r\n";
        file.write(header.toStdString().c_str());
        file.write(data.toStdString().c_str());
    } else {
        qDebug() << "open file error";
    }

    file.close();

    return true;
}
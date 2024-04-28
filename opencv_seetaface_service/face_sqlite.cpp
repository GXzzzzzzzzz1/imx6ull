#include "face_sqlite.h"
#include <QDebug>
face_sqlite::face_sqlite()
{
    //打开数据库
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("./db/name_faceid.db");//打开数据库
    if (DB.open())
        qDebug() << "Database opened successfully！";
    else
        qDebug() << "无法打开数据库：" << DB.lastError().text();

    // 创建表(如果不存在)
     QSqlQuery createTableQuery(DB);
     createTableQuery.exec("CREATE TABLE IF NOT EXISTS face_data ("
                           "name TEXT, "
                           "face_id INTEGER PRIMARY KEY)");
}

bool face_sqlite::insert_name_faceid(QString name, int faceid)
{
    // 检查name是否已存在
    QSqlQuery checkNameQuery(DB);
    checkNameQuery.prepare("SELECT face_id FROM face_data WHERE name = :name");
    checkNameQuery.bindValue(":name", name);
    if (checkNameQuery.exec() && checkNameQuery.next()) {
        qDebug() << "Name" << name << "already exists with face_id:" << checkNameQuery.value(0).toInt();
        return false;
    }

    // 检查faceid是否已存在
    QSqlQuery checkFaceidQuery(DB);
    checkFaceidQuery.prepare("SELECT name FROM face_data WHERE face_id = :face_id");
    checkFaceidQuery.bindValue(":face_id", faceid);
    if (checkFaceidQuery.exec() && checkFaceidQuery.next()) {
        qDebug() << "Face ID" << faceid << "already exists with name:" << checkFaceidQuery.value(0).toString();
        return false;
    }

    // 如果name和faceid都不存在,则插入数据
    qDebug() << "name和faceid都不存在,则插入数据";
    QSqlQuery insertQuery(DB);
    insertQuery.prepare("INSERT INTO face_data (name, face_id) VALUES (:name, :face_id)");
    insertQuery.bindValue(":name", name);
    insertQuery.bindValue(":face_id", faceid);
    if (insertQuery.exec()) {
        qDebug() << "数据库插入 name:" << name << "and face_id:" << faceid;
        return true;
    } else {
        qDebug() << "数据库插入失败:" << insertQuery.lastError().text();
        return false;
    }
}

bool face_sqlite::check_faceid_exist(int faceid)
{
    QSqlQuery query(DB);
    query.prepare("SELECT name FROM face_data WHERE face_id = :face_id");
    query.bindValue(":face_id", faceid);
    if (query.exec() && query.next()) {
        //qDebug() << "Face ID" << faceid << "exists, name:" << query.value(0).toString();
        return true;
    } else {
        qDebug() << "Face ID" << faceid << "does not exist";
        return false;
    }
}

bool face_sqlite::check_name_exist(QString name)
{
    QSqlQuery query(DB);
    query.prepare("SELECT face_id FROM face_data WHERE name = :name");
    query.bindValue(":name", name);
    if (query.exec() && query.next()) {
        qDebug() << "Name" << name << "exists, face_id:" << query.value(0).toInt();
        return true;
    } else {
        qDebug() << "Name" << name << "does not exist";
        return false;
    }
}

QString face_sqlite::get_name_by_faceid(int faceid)
{
    QSqlQuery query(DB);
    query.prepare("SELECT name FROM face_data WHERE face_id = :face_id");
    query.bindValue(":face_id", faceid);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    } else {
        qDebug() << "Failed to get name for face_id" << faceid << ":" << query.lastError().text();
        return QString();
    }
}

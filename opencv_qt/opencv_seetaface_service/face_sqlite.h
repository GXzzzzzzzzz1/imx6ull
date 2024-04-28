#ifndef FACE_SQLITE_H
#define FACE_SQLITE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class face_sqlite
{
public:
    face_sqlite();

    //注册
    bool insert_name_faceid(QString name,int faceid);
    //检查faceid是否存在
    bool check_faceid_exist(int faceid);
    //检查名称是否存在
    bool check_name_exist(QString name);
    //通过faceid获得name
    QString get_name_by_faceid(int faceid);
private:
    QSqlDatabase DB;
};

#endif // FACE_SQLITE_H

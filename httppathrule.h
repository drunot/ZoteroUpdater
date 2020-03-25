#ifndef HTTPPATHRULE_H
#define HTTPPATHRULE_H
#include<QString>
#include"httpobject.h"


typedef HTTPObject * (pathFunction)(HTTPObject * obj);

class HTTPPathRule
{
public:
    HTTPPathRule(QString path, pathFunction * funktion);
    ~HTTPPathRule();
    inline HTTPObject * run(HTTPObject * obj)
    {
        return (*_funktion)(obj);
    }
    static bool runMatch(HTTPObject * obj);


    //Operators

    inline bool operator==(const HTTPPathRule & right)
    {
        return _path == right._path;
    }
    inline bool operator==(const QString & right)
    {
        return _path == right;
    }
    inline bool operator<(const HTTPPathRule & right)
    {
        return _path < right._path;
    }
    inline bool operator<(const QString & right)
    {
        return _path < right;
    }
    inline bool operator>(const HTTPPathRule & right)
    {
        return _path > right._path;
    }
    inline bool operator>(const QString & right)
    {
        return _path > right;
    }
    friend bool operator==(HTTPPathRule * left, const QString);
private:
    QString _path;
    pathFunction * _funktion;
    static QList<HTTPPathRule *> _allHTTPPathRules;
};

inline bool operator==(HTTPPathRule * left, const QString right)
{
    return left->_path == right;
}
#endif // HTTPPATHRULE_H

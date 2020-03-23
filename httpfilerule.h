#ifndef HTTPFileRule_H
#define HTTPFileRule_H
#include<QString>
#include"httpobject.h"
typedef HTTPObject * (fileFunction)(HTTPObject * obj);

class HTTPFileRule
{
public:
    HTTPFileRule(QString extension, fileFunction * funktion);
    ~HTTPFileRule();
    inline HTTPObject * run(HTTPObject * obj)
    {
        return (*_funktion)(obj);
    }
    static bool runMatch(HTTPObject * obj);


    //Operators

    inline bool operator==(const HTTPFileRule & right)
    {
        return _extension == right._extension;
    }
    inline bool operator==(const QString & right)
    {
        return _extension == right;
    }
    inline bool operator<(const HTTPFileRule & right)
    {
        return _extension < right._extension;
    }
    inline bool operator<(const QString & right)
    {
        return _extension < right;
    }
    inline bool operator>(const HTTPFileRule & right)
    {
        return _extension > right._extension;
    }
    inline bool operator>(const QString & right)
    {
        return _extension > right;
    }
    friend bool operator==(HTTPFileRule * left, const QString right);
private:
    QString _extension;
    fileFunction * _funktion;
    static QList<HTTPFileRule *> _allHTTPFileRules;
};

inline bool operator==(HTTPFileRule * left, const QString right)
{
    return left->_extension == right;
}
#endif // HTTPFileRule_H

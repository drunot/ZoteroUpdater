#include "httpfilerule.h"

HTTPFileRule::HTTPFileRule(QString extension, fileFunction * funktion)
{
    _extension = extension;
    _funktion = funktion;
    _allHTTPFileRules.push_back(this);
    std::sort(_allHTTPFileRules.begin(),_allHTTPFileRules.end());
}

HTTPFileRule::~HTTPFileRule()
{
    _allHTTPFileRules.removeOne(this);
}



bool HTTPFileRule::runMatch(HTTPObject * obj)
{
    QString extension = obj->getGetAddress().split(".").last();
    if(extension.length() == 0)
    {
        return false;
    }
    QList<HTTPFileRule *>::iterator it(std::find(_allHTTPFileRules.begin(),_allHTTPFileRules.end(),extension));
    if(it != _allHTTPFileRules.end())
    {
        (*it)->run(obj);
        return true;
    }
    return false;
}


QList<HTTPFileRule *> HTTPFileRule::_allHTTPFileRules;

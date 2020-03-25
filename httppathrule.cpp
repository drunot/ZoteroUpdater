#include "httppathrule.h"

HTTPPathRule::HTTPPathRule(QString extension, pathFunction * funktion)
{
    _path = extension;
    _funktion = funktion;
    _allHTTPPathRules.push_back(this);
    std::sort(_allHTTPPathRules.begin(),_allHTTPPathRules.end());
}

HTTPPathRule::~HTTPPathRule()
{
    _allHTTPPathRules.removeOne(this);
}



bool HTTPPathRule::runMatch(HTTPObject * obj)
{
    QList<HTTPPathRule *>::iterator it(std::find(_allHTTPPathRules.begin(),_allHTTPPathRules.end(),obj->getGetAddress()));
    if(it != _allHTTPPathRules.end())
    {
        (*it)->run(obj);
        return true;
    }
    return false;
}


QList<HTTPPathRule *> HTTPPathRule::_allHTTPPathRules;

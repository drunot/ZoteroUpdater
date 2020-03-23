#ifndef ZOTEROUPDATER_H
#define ZOTEROUPDATER_H

#include <QStringList>
#include <QDateTime>
#include "httpobject.h"

HTTPObject * ZoteroUpdater(HTTPObject * obj);

const QString c404page("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>404 Not Found</title>\n\t</head>\n\t<body>\n\t\t<h1>404 Not Found: Page not found</h1>\n\t\t<p>The page %1 was not found.</br>\n\tCheck if the address was misspelled.</p>\n\t</body>\n</html>");

const QString c404user("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>404 Not Found</title>\n\t</head>\n\t<body>\n\t\t<h1>404 Not Found: User not found</h1>\n\t\t<p>The user %1 was not found.</br>\n\tCheck if the address was misspelled.</p>\n\t</body>\n</html>");

const QString c404group("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>404 Not Found</title>\n\t</head>\n\t<body>\n\t\t<h1>404 Not Found: Group not found</h1>\n\t\t<p>The group %1 was not found.</br>\n\tCheck if the address was misspelled.</p>\n\t</body>\n</html>");

const QString c503("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>503 Service Unavailable</title>\n\t</head>\n\t<body>\n\t\t<h1>503 Service Unavailable: Could not fetch data</h1>\n\t\t<p>Could not fetch data from Zotero. Check if the Zotero api is down or broken.</br>\n\tContact your admin if that didn't make sense.</p>\n\t</body>\n</html>");
#endif // ZOTEROUPDATER_H

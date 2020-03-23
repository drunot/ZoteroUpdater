#ifndef HTTPFILEOPERATIONS_H
#define HTTPFILEOPERATIONS_H
#include "httpobject.h"
namespace HTTPFileOperations {
//Help functions for fileoperations.
HTTPObject * FileHTML(HTTPObject * obj);
HTTPObject * LoadCode(HTTPObject * obj);
HTTPObject * FileHTML(HTTPObject * obj);
HTTPObject * FileGeneral(HTTPObject *obj, QString dataType);
HTTPObject * LoadCode(HTTPObject * obj);

//Basic rules. Needed to be added manualy.
HTTPObject * loadIndex(HTTPObject * obj);
HTTPObject * loadImage(HTTPObject * obj);
}
#endif // HTTPFILEOPERATIONS_H

#include "Glass.h"

void Glass::assign(Glass* g)
{
    _id = g->_id;
    _title = g->_title;
    _comment = g->_comment;
    _lambdaMin = g->_lambdaMin;
    _lambdaMax = g->_lambdaMax;
}

#include "View.h"

void View::sendResponse(http_request request, const status_code &code, const string &body, const string &contentType)
{
    request.reply(code, body, contentType);
}

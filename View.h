#ifndef VIEW_H
#define VIEW_H

#include <cpprest/http_listener.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class View {
public:
    static void sendResponse(http_request request, const status_code& code, const string& body, const string& contentType = "application/json");
};

#endif

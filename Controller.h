#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Model.h"
#include <cpprest/http_listener.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class Controller
{
private:
    Database db;
    http_listener listener;

public:
    Controller(const string &url);
    void start();
    void handleGetUsers(http_request request);
    void handleGetUser(http_request request);
    void handlePostUser(http_request request);
    void handlePutUser(http_request request);
    void handleDeleteUser(http_request request);
};

#endif

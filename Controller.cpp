#include "Controller.h"
#include <iostream>
#include <sstream>

Controller::Controller(const string &url) : listener(url)
{
    listener.support(methods::GET, std::bind(&Controller::handleGetUsers, this, std::placeholders::_1));
    listener.support(methods::GET, std::bind(&Controller::handleGetUser, this, std::placeholders::_1));
    listener.support(methods::POST, std::bind(&Controller::handlePostUser, this, std::placeholders::_1));
    listener.support(methods::PUT, std::bind(&Controller::handlePutUser, this, std::placeholders::_1));
    listener.support(methods::DEL, std::bind(&Controller::handleDeleteUser, this, std::placeholders::_1));
}

void Controller::start()
{
    listener.open().then([]()
                         { cout << "Listening on port 8080..." << endl; })
        .wait();
}

void Controller::handleGetUsers(http_request request)
{
    // Return list of all users
    stringstream response;
    response << "[";
    for (const auto &user : db.getUsers())
    {
        response << "{\"id\":" << user.id << ",\"name\":\"" << user.name << "\",\"email\":\"" << user.email << "\"},";
    }
    if (!db.getUsers().empty())
    {
        response.seekp(-1, response.end); // Remove the last comma
    }
    response << "]";
    request.reply(status_codes::OK, response.str(), "application/json");
}

void Controller::handleGetUser(http_request request)
{
    // Extract user ID from URL and return user details
    auto path = request.relative_uri().path();
    string idStr = path.substr(path.find_last_of('/') + 1);
    int id;
    try
    {
        id = stoi(idStr);
    }
    catch (...)
    {
        request.reply(status_codes::BadRequest, "Invalid user ID");
        return;
    }
    User user = db.getUser(id);
    if (user.id != -1)
    {
        stringstream response;
        response << "{\"id\":" << user.id << ",\"name\":\"" << user.name << "\",\"email\":\"" << user.email << "\"}";
        request.reply(status_codes::OK, response.str(), "application/json");
    }
    else
    {
        request.reply(status_codes::NotFound, "User not found");
    }
}

void Controller::handlePostUser(http_request request)
{
    // Extract user data from request body and create a new user
    request.extract_json().then([=](json::value body)
                                {
        if (body.is_object() && body.has_field("name") && body.has_field("email")) {
            string name = body["name"].as_string();
            string email = body["email"].as_string();
            User newUser = db.createUser(name, email);
            stringstream response;
            response << "{\"id\":" << newUser.id << ",\"name\":\"" << newUser.name << "\",\"email\":\"" << newUser.email << "\"}";
            request.reply(status_codes::Created, response.str(), "application/json");
        } else {
            request.reply(status_codes::BadRequest, "Invalid request body");
        } })
        .wait();
}

void Controller::handlePutUser(http_request request)
{
    // Extract user ID from URL and update user details
    auto path = request.relative_uri().path();
    string idStr = path.substr(path.find_last_of('/') + 1);
    int id;
    try
    {
        id = stoi(idStr);
    }
    catch (...)
    {
        request.reply(status_codes::BadRequest, "Invalid user ID");
        return;
    }
    request.extract_json().then([=](json::value body)
                                {
        if (body.is_object() && body.has_field("name") && body.has_field("email")) {
            string name = body["name"].as_string();
            string email = body["email"].as_string();
            db.updateUser(id, name, email);
            request.reply(status_codes::OK, "User updated successfully");
        } else {
            request.reply(status_codes::BadRequest, "Invalid request body");
        } })
        .wait();
}

void Controller::handleDeleteUser(http_request request)
{
    // Extract user ID from URL and delete the user
    auto path = request.relative_uri().path();
    string idStr = path.substr(path.find_last_of('/') + 1);
    int id;
    try
    {
        id = stoi(idStr);
    }
    catch (...)
    {
        request.reply(status_codes::BadRequest, "Invalid user ID");
        return;
    }
    db.deleteUser(id);
    request.reply(status_codes::OK, "User deleted successfully");
}

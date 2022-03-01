#pragma once

#include <string>

#include <drogon/drogon.h>

#include "books_genre.h"
#include "mongodb_handler.h"
#include "status.h"

using namespace drogon;
typedef std::function<void(const HttpResponsePtr &)> Callback ;

namespace RestAPI{
    constexpr char kAllBooksHandler[] ="/";
    constexpr char kBookHandler[] ="/book";
    constexpr char kAddBookHandler[] ="/book/add";
    constexpr char kTradeHandler[] ="/book/trade";
    constexpr char kReviewsHandler[] = "/book/review";
    constexpr char kAddReviewHandler[] = "/book/review/add";
    constexpr char kIpAddress[] = "0.0.0.0";
    constexpr int kPort = 8080;
    constexpr int kThreads = 10;

    auto AddBookToDB(const HttpRequestPtr &request, Callback &&callback){
        Json::Value jsonBody;
        auto request_body = request->getJsonObject();
        if (request_body == nullptr){
            jsonBody["status"] = "error";
            jsonBody["message"] = "body is required";
            auto response = HttpResponse::newHttpJsonResponse(jsonBody);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }
        if (!request_body->isMember("username") && !request_body->isMember("bookName")
                                        && !request_body->isMember("author") && !request_body->isMember("year")
                                        && !request_body->isMember(("genre")) && !request_body->isMember("status")
                                        && !request_body->isMember("price")){
            jsonBody["status"] = "error";
            jsonBody["message"] = "field is required";
            auto response = HttpResponse::newHttpJsonResponse(jsonBody);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }

        auto username = request_body->get("username","").asString();
        auto bookName = request_body->get("bookName","").asString();
        auto author = request_body->get("author","").asString();
        auto year = request_body->get("year","").asInt();
        auto genre = request_body->get("genre","").asString();
        auto status = request_body->get("status","").asString();
        auto price = request_body->get("price","").asInt();

        if (string_to_books_genre.find(genre) == string_to_books_genre.end()
                 && string_to_status.find(status) == string_to_status.end()){
            auto response = HttpResponse::newHttpJsonResponse(jsonBody);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }

        MongoDBHandler mhandler;
        bool addBook_successful = mhandler.AddBooksToDB(username, bookName, author, year,
                     string_to_books_genre.find(genre)->second, string_to_status.find(status)->second, price);

        auto response = HttpResponse::newHttpJsonResponse(jsonBody);

        addBook_successful ? response->setStatusCode(HttpStatusCode::k200OK)
                           : response->setStatusCode(HttpStatusCode::k400BadRequest);

        callback(response);
    } //200 OK

    auto AddReview(const HttpRequestPtr &request, Callback &&callback){
        Json::Value jsonBody;
        auto request_body = request->getJsonObject();
        if (request_body == nullptr){
            jsonBody["status"] = "error";
            jsonBody["message"] = "body is required";
            auto response = HttpResponse::newHttpJsonResponse(jsonBody);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }
        if (!request_body->isMember("bookId") && !request_body->isMember("review")
            && !request_body->isMember("username")){
            jsonBody["status"] = "error";
            jsonBody["message"] = "field is required";
            auto response = HttpResponse::newHttpJsonResponse(jsonBody);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }

        auto bookId = request_body->get("bookId","").asString();
        auto review = request_body->get("review","").asString();
        auto username = request_body->get("username","").asString();

        MongoDBHandler mhandler;
        bool addreview_successful = mhandler.AddReview(bookId, review, username);

        auto response = HttpResponse::newHttpJsonResponse(jsonBody);

        addreview_successful ? response->setStatusCode(HttpStatusCode::k200OK)
                           : response->setStatusCode(HttpStatusCode::k400BadRequest);

        callback(response);
    } //200 OK

    auto GetAllBooks(const HttpRequestPtr &request, Callback &&callback){
        MongoDBHandler mhandler;
        const Json::Value all_books = mhandler.GetAllBooks();
        auto response = HttpResponse::newHttpJsonResponse(all_books);
        callback(response);
    } //200 OK

    auto GetBook(const HttpRequestPtr &request, Callback &&callback){
        Json::Value jsonBody;
        auto request_body = request->getJsonObject();
        if (request_body == nullptr){
            jsonBody["status"] = "error";
            jsonBody["message"] = "body is required";
            auto response = HttpResponse::newHttpJsonResponse(jsonBody);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }
        if (!request_body->isMember("bookId")){
            jsonBody["status"] = "error";
            jsonBody["message"] = "field 'bookId' is required";
            auto response = HttpResponse::newHttpJsonResponse(jsonBody);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }
        MongoDBHandler mhandler;
        auto bookId = request_body->get("bookId", "").asString();

        jsonBody =  mhandler.GetBook(bookId);
        auto response = HttpResponse::newHttpJsonResponse(jsonBody);

        callback(response);
    } //200 OK

    auto GetReviews(const HttpRequestPtr &request, Callback &&callback){
        Json::Value jsonBody;
        auto request_body = request->getJsonObject();
        if (request_body == nullptr) {
            jsonBody["status"] = "error";
            jsonBody["message"] = "body is required";
            auto response = HttpResponse::newHttpJsonResponse(jsonBody);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }
        if (!request_body->isMember("bookId")){
            jsonBody["status"] = "error";
            jsonBody["message"] = "field 'bookId' is required";
            auto response = HttpResponse::newHttpJsonResponse(jsonBody);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }
        MongoDBHandler mhandler;
        auto bookId = request_body->get("bookId", "").asString();
        jsonBody =  mhandler.GetReviews(bookId);
        auto response = HttpResponse::newHttpJsonResponse(jsonBody);

        callback(response);

    } //200 OK

    auto TradeBook(const HttpRequestPtr &request, Callback &&callback){
        Json::Value jsonBody;
        auto request_body = request->getJsonObject();

        if (request_body == nullptr) {
            jsonBody["status"] = "error";
            jsonBody["message"] = "body is required";
            auto response = HttpResponse::newHttpJsonResponse(jsonBody);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }
        if (!request_body->isMember("bookId") && !request_body->isMember("username")){
            jsonBody["status"] = "error";
            jsonBody["message"] = "field is required";
            auto response = HttpResponse::newHttpJsonResponse(jsonBody);
            response->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(response);
            return;
        }

        auto bookId = request_body->get("bookId", "").asString();
        auto username = request_body->get("username", "").asString();

        MongoDBHandler mhandler;
        bool trade_successful = mhandler.tradeBook(bookId, username);
        auto response = HttpResponse::newHttpJsonResponse(jsonBody);
        trade_successful ? response->setStatusCode(HttpStatusCode::k200OK)
                         : response->setStatusCode(HttpStatusCode::k400BadRequest);

        callback(response);
    } //200 OK

    void StartServer(){
        app()
            .addListener(kIpAddress, kPort)
            .setThreadNum(kThreads)
            .enableServerHeader(true)
            .registerHandler(kAllBooksHandler, &GetAllBooks, {Get})
            .registerHandler(kBookHandler, &GetBook, {Get})
            .registerHandler(kReviewsHandler, &GetReviews, {Get})
            .registerHandler(kAddBookHandler, &AddBookToDB, {Post})
            .registerHandler(kAddReviewHandler, &AddReview, {Put})
            .registerHandler(kTradeHandler, &TradeBook, {Put})
            .run();

    }
}

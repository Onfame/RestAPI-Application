#pragma once

#include <string>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/oid.hpp>
#include <drogon/drogon.h>
#include <mongocxx/client.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/uri.hpp>

#include "status.h"
#include "books_genre.h"

constexpr char kMongoDBUri[] = "0.0.0.0:27017";
constexpr char kDatabaseName[] = "Booking_service";
constexpr char kBooksCollection[] = "Books";
constexpr char kReviewCollection[] = "Reviews";

using namespace drogon;

namespace RestAPI{
    class MongoDBHandler{
    public:
        MongoDBHandler() : uri(mongocxx::uri(kMongoDBUri)),
                           client(mongocxx::client(uri)),
                           db(client[kDatabaseName]){}

        bool AddBooksToDB(const std::string &userName, const std::string &bookName,
                          const std::string &author, const int16_t &year, const BooksGenre &genre,
                          const Status &status, const int16_t &price = 0){
            mongocxx::collection book_collection = db[kBooksCollection];

            auto builder = bsoncxx::builder::stream::document{};

            bsoncxx::document::value book_to_add = builder << "username" << userName
                                                  << "book_name" << bookName
                                                  << "author" << author
                                                  << "year" << year
                                                  << "genre" << books_genre_to_string.find(genre)->second
                                                  << "status" << status_to_string.find(status)->second
                                                  << "price" << price
                                                  << bsoncxx::builder::stream::finalize;

            bsoncxx::stdx::optional<mongocxx::result::insert_one> maybe_result =
                    book_collection.insert_one(book_to_add.view());

            return true;
        };

        bool AddReview(const std::string &bookId, const std::string &review, const std::string &username){
            mongocxx::collection review_collection = db[kReviewCollection];
            auto builder = bsoncxx::builder::stream::document{};

            bsoncxx::document::value review_to_add = builder << "book_id" << bookId
                                                     << "review" << review
                                                     << "username" << username
                                                     << bsoncxx::builder::stream::finalize;

            bsoncxx::stdx::optional<mongocxx::result::insert_one> maybe_result =
                    review_collection.insert_one(review_to_add.view());
            return true;
        }

        Json::Value GetAllBooks(){
            mongocxx::collection book_collection = db[kBooksCollection];
            mongocxx::cursor cursor = book_collection.find({});

            Json::Value result;
            if (cursor.begin() != cursor.end()){
                for (auto  doc : cursor){
                    result["Books"].append(bsoncxx::to_json(doc));
                }
            }
            return result;
        }

        Json::Value GetBook(const std::string &book_id){
            mongocxx::collection book_collection = db[kBooksCollection];
            bsoncxx::oid doc_id(book_id);
            bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
                    book_collection.find_one(bsoncxx::builder::stream::document{}
                                                     << "_id" << doc_id
                                                     << bsoncxx::builder::stream::finalize);
            Json::Value result;
            if (maybe_result){

                return result = bsoncxx::to_json(*maybe_result);
            }
            return result;
        }

        Json::Value GetReviews(const std::string &book_id){
            mongocxx::collection review_collection = db[kReviewCollection];
            mongocxx::cursor maybe_result =
                    review_collection.find(bsoncxx::builder::stream::document{}
                                                     << "book_id" << book_id
                                                     << bsoncxx::builder::stream::finalize);
            Json::Value result;
            for (auto doc : maybe_result){
                result["Reviews"].append(bsoncxx::to_json(doc));
            }
            return result;
        }

        bool tradeBook(const std::string &bookId, const std::string &userName){
            mongocxx::collection book_collection = db[kBooksCollection];
            auto builder = bsoncxx::builder::stream::document{};
            bsoncxx::oid book_id(bookId);
            bsoncxx::document::value query_doc = builder << "_id" << book_id
                                                         << bsoncxx::builder::stream::finalize;

            bsoncxx::document::value update_doc = builder << "$set" << bsoncxx::builder::stream::open_document
                                                          << "user" << userName
                                                          << bsoncxx::builder::stream::close_document
                                                          << bsoncxx::builder::stream::finalize;
            bsoncxx::stdx::optional<mongocxx::result::update> maybe_result =
                    book_collection.update_one(query_doc.view(), update_doc.view());
            return true;
        }
    private:
        mongocxx::uri uri;
        mongocxx::client client;
        mongocxx::database db;
    };
}
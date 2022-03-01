#include <iostream>
#include <mongocxx/instance.hpp>

#include "books_genre.h"
#include "http_server.h"
#include "mongodb_handler.h"
#include "status.h"

using namespace RestAPI;

int main() {
    mongocxx::instance instance;
    /*MongoDBHandler mhandler;
    mhandler.AddBooksToDB("Onfame", "Гарри Поттер и философский камень", "Джоан Роулинг",
                              2016, BooksGenre::kFantasy, Status::kExchange
                              );
    mhandler.AddBooksToDB("Arthas", "Гарри Поттер и философский камень", "Джоан Роулинг",
                          2016, BooksGenre::kFantasy, Status::kPurchase, 1000
    );
    std::cout << mhandler.GetAllBooks()["Books"] << std::endl;*/
    RestAPI::StartServer();
    std::cout << "End programm" << std::endl;
    return 0;
}

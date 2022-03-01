#pragma once

#include <string>
#include <unordered_map>

namespace RestAPI {
    enum class Status{
        kExchange,
        kPurchase,
    };

    std::unordered_map<Status, const char*> status_to_string({
        {Status::kExchange, "exchange"},
        {Status::kPurchase, "purchase"},
    });

    std::unordered_map<std::string, Status> string_to_status({
        {"exchange", Status::kExchange},
        {"purchase", Status::kPurchase},
    });
}

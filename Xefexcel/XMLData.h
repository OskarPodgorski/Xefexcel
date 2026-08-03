#pragma once

#include <string>

struct XMLData
{
    std::string invoiceNumber;
    std::string date;
    std::string sellerName;
    std::string buyerName;
    std::string info;
    std::string vatPercent;
    std::string netto;
    std::string paymentDays;
};
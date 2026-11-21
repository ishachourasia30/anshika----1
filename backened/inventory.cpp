#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#ifdef _WIN32
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#include "httplib.h"

using namespace std;
using namespace httplib;

struct Product {
    int id;
    string name;
    string category;
    int quantity;
    int price;
};

struct Order {
    int orderId;
    int productId;
    int quantity;
    string status;
};

vector<Product> products = {
    {101, "Laptop", "Electronics", 10, 55000},
    {102, "Mouse", "Electronics", 25, 500},
    {103, "Keyboard", "Electronics", 15, 900},
    {104, "Notebook", "Stationery", 40, 60},
    {105, "Pen", "Stationery", 50, 20}
};

vector<Order> orders;


int main() {

    Server server;
server.set_mount_point("/", "./public");


    server.Get("/api/products",
        [](const Request& req, Response& res) {

        string json = "{\"products\":[";

        for (int i = 0; i < products.size(); i++) {

            json += "{";
            json += "\"id\":" + to_string(products[i].id) + ",";
            json += "\"name\":\"" + products[i].name + "\",";
            json += "\"category\":\"" + products[i].category + "\",";
            json += "\"quantity\":" + to_string(products[i].quantity) + ",";
            json += "\"price\":" + to_string(products[i].price);
            json += "}";

            if (i < products.size() - 1)
                json += ",";
        }

        json += "],";
        json += "\"orders\":" + to_string(orders.size());
        json += "}";

        res.set_content(json, "application/json");
    });


    server.Get("/api/search",
        [](const Request& req, Response& res) {

        int id = stoi(req.get_param_value("id"));

        for (auto &p : products) {

            if (p.id == id) {

                string json = "{";
                json += "\"found\":true,";
                json += "\"name\":\"" + p.name + "\",";
                json += "\"category\":\"" + p.category + "\",";
                json += "\"quantity\":" + to_string(p.quantity) + ",";
                json += "\"price\":" + to_string(p.price);
                json += "}";

                res.set_content(json, "application/json");
                return;
            }
        }

        res.set_content(
            "{\"found\":false}",
            "application/json"
        );
    });


    server.Get("/api/update",
        [](const Request& req, Response& res) {

        int id =
            stoi(req.get_param_value("id"));

        int quantity =
            stoi(req.get_param_value("quantity"));

        for (auto &p : products) {

            if (p.id == id) {

                p.quantity = quantity;

                res.set_content(
                    "{\"message\":\"Quantity updated successfully.\"}",
                    "application/json"
                );

                return;
            }
        }

        res.set_content(
            "{\"message\":\"Product not found.\"}",
            "application/json"
        );
    });


    server.Get("/api/order",
        [](const Request& req, Response& res) {

        int productId =
            stoi(req.get_param_value("productId"));

        int quantity =
            stoi(req.get_param_value("quantity"));

        for (auto &p : products) {

            if (p.id == productId) {

                if (p.quantity >= quantity) {

                    p.quantity -= quantity;

                    int orderId =
                        1001 + orders.size();

                    orders.push_back({
                        orderId,
                        productId,
                        quantity,
                        "Order Placed"
                    });

                    string json =
                        "{\"message\":\"Order placed successfully. Order ID: "
                        + to_string(orderId)
                        + "\"}";

                    res.set_content(
                        json,
                        "application/json"
                    );

                } else {

                    res.set_content(
                        "{\"message\":\"Not enough stock.\"}",
                        "application/json"
                    );
                }

                return;
            }
        }

        res.set_content(
            "{\"message\":\"Product not found.\"}",
            "application/json"
        );
    });


    server.Get("/api/track",
        [](const Request& req, Response& res) {

        int id =
            stoi(req.get_param_value("id"));

        for (auto &o : orders) {

            if (o.orderId == id) {

                string json = "{";
                json += "\"found\":true,";
                json += "\"orderId\":" + to_string(o.orderId) + ",";
                json += "\"productId\":" + to_string(o.productId) + ",";
                json += "\"quantity\":" + to_string(o.quantity) + ",";
                json += "\"status\":\"" + o.status + "\"";
                json += "}";

                res.set_content(
                    json,
                    "application/json"
                );

                return;
            }
        }

        res.set_content(
            "{\"found\":false}",
            "application/json"
        );
    });


   cout << "Inventory server started..." << endl;

const char* envPort = getenv("PORT");

int port = 8080;

if (envPort != nullptr) {
    port = stoi(envPort);
}

server.listen("0.0.0.0", port);

    return 0;
}

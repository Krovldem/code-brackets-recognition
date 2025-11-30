#include "CodePairsCheck.h"
#include "crow_all.h"
#include "json.hpp"

int main() {
    crow::SimpleApp app;
    CROW_ROUTE(app, "/check").methods("POST"_method)
            ([&](const crow::request &req) {
                auto body = req.body;
                if (body.empty()) {
                    crow::json::wvalue res;
                    res["error"] = "Empty request body";
                    return crow::response(400, res);
                }
                auto errors = checkCode(body);
                nlohmann::json resp = nlohmann::json::array();
                for (const auto &err: errors) {
                    resp.push_back({{"line", err.line},{"col", err.col},{"description", err.description}});
                }
                return crow::response(resp.dump());
            });
    app.port(18080).multithreaded().run();
}

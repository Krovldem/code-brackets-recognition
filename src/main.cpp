#include "mongoose.h"
#include "CodePairsCheck.h"
#include "json.hpp"
#include <cstring>

bool is_method(struct mg_str method, const char *s) {
    return (method.len == strlen(s) && strncmp(method.buf, s, method.len) == 0);
}

bool uri_match(const struct mg_str &uri, const char *pattern) {
    size_t patternLen = strlen(pattern);
    return (uri.len == patternLen && strncmp(uri.buf, pattern, patternLen) == 0);
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        auto *hm = (struct mg_http_message *) ev_data;
        if (is_method(hm->method, "OPTIONS")) {
            mg_http_reply(c, 200,
                          "Access-Control-Allow-Origin: *\r\n"
                          "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
                          "Access-Control-Allow-Headers: Content-Type\r\n",
                          "");
            return;
        }
        if (!uri_match(hm->uri, "/check")) return;
        std::string body(hm->body.buf, hm->body.len);
        if (body.empty()) {
            mg_http_reply(c, 400,
                          "Content-Type: application/json\r\n"
                          "Access-Control-Allow-Origin: *\r\n",
                          R"({"error":"Empty request body"})");
            return;
        }
        auto errors = checkCode(body);
        nlohmann::json resp = nlohmann::json::array();
        for (const auto &err : errors) {
            resp.push_back(nlohmann::json{
                    {"line", err.line},
                    {"col", err.col},
                    {"description", err.description}
            });
        }

        std::string out = resp.dump();
        mg_http_reply(c, 200,
                      "Content-Type: application/json\r\n"
                      "Access-Control-Allow-Origin: *\r\n"
                      "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
                      "Access-Control-Allow-Headers: Content-Type\r\n",
                      "%s", out.c_str());
    }
}

int main() {
    mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, "http://0.0.0.0:18080", fn, nullptr);

    printf("Server started on port 18080\n");
    for (;;) mg_mgr_poll(&mgr, 1000);

    mg_mgr_free(&mgr);
    return 0;
}

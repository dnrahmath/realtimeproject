#include <iostream>
#include <set>
#include <unordered_map>
#include <functional>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

typedef websocketpp::server<websocketpp::config::asio> server;

std::unordered_map<std::string, std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>>> rooms;

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    try {
        auto message = msg->get_payload();
        if (message.empty()) return;

        auto jmessage = json::parse(message);

        if (jmessage["type"] == "join") {
            std::string room = jmessage["room"];
            std::string name = jmessage["name"]; // Menambahkan permintaan masukkan nama
            rooms[room].insert(hdl);
            std::cout << "Client " << name << " joined room: " << room << std::endl;
        } else if (jmessage["type"] == "message") {
            std::string room = jmessage["room"];
            std::string text = jmessage["text"];

            for (auto h : rooms[room]) {
                s->send(h, message, msg->get_opcode());
            }
        }
    } catch (const json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}

int main() {
    server ws_server;

    try {
        ws_server.set_access_channels(websocketpp::log::alevel::all);
        ws_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        ws_server.init_asio();
        ws_server.set_message_handler(std::bind(&on_message, &ws_server, std::placeholders::_1, std::placeholders::_2));

        ws_server.listen(8080);
        ws_server.start_accept();

        std::cout << "WebSocket server started on port 8080" << std::endl;

        ws_server.run();
    } catch (websocketpp::exception const & e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Other exception" << std::endl;
    }

    return 0;
}

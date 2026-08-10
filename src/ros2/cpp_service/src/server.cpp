#include "my_server.hpp"


Server::Server():Node("cpp_server_node"){
        RCLCPP_INFO(this->get_logger(),"Server has been started.");
        server_ = this->create_service<AddTwoInts_msg>("server_test", std::bind(&Server::serverCallback, 
                                                                                  this, _1, _2));   
    }

void Server::serverCallback(const AddTwoInts_msg::Request::SharedPtr request, 
                            const AddTwoInts_msg::Response::SharedPtr response){
        response->sum = request->a + request->b;
        RCLCPP_INFO(this->get_logger(), "%ld + %ld = %ld", request->a, request->b, response->sum);
    }


int main(int argc, char ** args){
    rclcpp::init(argc, args);
    rclcpp::spin(std::make_shared<Server>());
    rclcpp::shutdown();
}
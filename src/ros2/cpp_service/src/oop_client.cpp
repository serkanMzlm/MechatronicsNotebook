#include "my_oop_client.hpp"

Client::Client(): Node("oop_client_node"){
  client_ = this->create_client<add2Int>("server_test");
  threads.push_back(std::thread(std::bind(&Client::clientCallback, this, 10, 23)));
  threads.push_back(std::thread(std::bind(&Client::clientCallback, this, 15, 23)));
  threads.push_back(std::thread(std::bind(&Client::clientCallback, this, 1, 23)));
  RCLCPP_DEBUG(this->get_logger(),"OOP Client has been started.");
  threads.push_back(std::thread(std::bind(&Client::clientCallback, this, 10, 223)));
}

Client::~Client(){
  RCLCPP_DEBUG(this->get_logger(), "OOP Client terminated...");
}

void Client::clientCallback(int a, int b){
  while(!client_->wait_for_service(std::chrono::seconds(1))){
    RCLCPP_WARN(this->get_logger(), "Wait for the server to be up...");
  }
  auto request = std::make_shared<add2Int::Request>();
  request->a = a;
  request->b = b;
  auto future = client_->async_send_request(request);
  try
  {
      // auto response = future.get();
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Sum: %ld", future.get()->sum);
  }
  catch(const std::exception& e)
  {
      RCLCPP_ERROR(this->get_logger(), "Failed to call service add_two_ints");
  }
}

int main(int argc, char ** args){
    rclcpp::init(argc, args);
    rclcpp::spin(std::make_shared<Client>());
    rclcpp::shutdown();
    return 0;
}
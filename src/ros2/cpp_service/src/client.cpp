#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

using add2Int = example_interfaces::srv::AddTwoInts;

int main(int argc, char** args){
 // rclcpp::init(0, nullptr);
    rclcpp::init(argc, args);
    auto node   = std::make_shared<rclcpp::Node>("cpp_client_node");
    auto client = node->create_client<add2Int>("server_test");

    while(!client->wait_for_service(std::chrono::seconds(1))){
      RCLCPP_WARN(node->get_logger(), "Wait for the server to be up...");
    }

    auto request = std::make_shared<add2Int::Request>();
    request->a = 10;
    request->b = 23;
    
    auto future = client->async_send_request(request);
    if(rclcpp::spin_until_future_complete(node, future) == rclcpp::FutureReturnCode::SUCCESS){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Sum: %ld", future.get()->sum);
    }else {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service add_two_ints");
    }
        rclcpp::shutdown();

    return 0;
}

#include "my_publisher.hpp"

PubNode::PubNode(): Node("publisher_node"), count_(0){
  RCLCPP_DEBUG(this->get_logger(),"Publisher has been started."); // 
  pub_int    = this->create_publisher<int16Msg>("cpp_topic_int", 10);
  pub_string = this->create_publisher<stringMsg>("cpp_topic_string", 10);
  timer_ = this->create_wall_timer(std::chrono::seconds(1), 
                          std::bind(&PubNode::pubCallbackInt, this));
}

PubNode::~PubNode(){
  RCLCPP_DEBUG(this->get_logger(), "Subscriber terminated...");
}

void PubNode::pubCallbackInt(){
  count_++;
  RCLCPP_INFO(this->get_logger(),"count: %d", count_);
  int16Msg msg;
  msg.data = count_;
  pubCallbackString();
  pub_int->publish(msg);
}

void PubNode::pubCallbackString(){
  stringMsg msg; // auto msg = stringMsg;
  RCLCPP_INFO(this->get_logger(),"%d) Hello", count_);
  msg.data = std::to_string(count_) + std::string(") Hello");
  pub_string->publish(msg);
}

int main(int argc, char ** args){
    rclcpp::init(argc, args);
    rclcpp::spin(std::make_shared<PubNode>());
    rclcpp::shutdown();
}
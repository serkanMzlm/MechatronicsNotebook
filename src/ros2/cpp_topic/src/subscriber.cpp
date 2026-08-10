#include "my_subscriber.hpp"

using namespace std::placeholders;

SubNode::SubNode(): Node("subscriber_node"){
  RCLCPP_DEBUG(this->get_logger(),"Subscriber has been started.");
  subs_int    = this->create_subscription<int16Msg>("cpp_topic_int", 10,
                std::bind(&SubNode::subsCallbackInt, this, _1));
  subs_string = this->create_subscription<stringMsg>("cpp_topic_string", 10,
                std::bind(&SubNode::subsCallbackString, this, _1));
}

SubNode::~SubNode(){
  RCLCPP_DEBUG(this->get_logger(), "Subscriber terminated...");
}

void SubNode::subsCallbackInt(const int16Msg msg){
  RCLCPP_INFO(this->get_logger(),"int: %d",msg.data);
}

void SubNode::subsCallbackString(const stringMsg msg){
  RCLCPP_INFO(this->get_logger(),"string: %s",msg.data.c_str());
}

int main(int argc, char ** args){
    rclcpp::init(argc, args);
    rclcpp::spin(std::make_shared<SubNode>());
    rclcpp::shutdown();
}
#ifndef __MY_SUBSCRIBER_HPP__
#define __MY_SUBSCRIBER_HPP__

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int16.hpp"
#include "std_msgs/msg/string.hpp"

using int16Msg = std_msgs::msg::Int16;
using stringMsg = std_msgs::msg::String;
// typedef std_msgs::msg::Int16 Int16Msg; şeklinde de yapılabilir. 

class SubNode: public rclcpp::Node{
public:
  SubNode();
  ~SubNode();
  void  subsCallbackInt(const int16Msg);
  void  subsCallbackString(const stringMsg);
private:
   rclcpp::Subscription<int16Msg>::SharedPtr subs_int;
   rclcpp::Subscription<stringMsg>::SharedPtr subs_string;
};



#endif
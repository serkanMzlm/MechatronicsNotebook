#include "rosnode.h"


rosNode::rosNode(QObject* param): QThread(param) /*QObject(param)*/, Node("TQ")
{
    stop = true;
    value = 0;
    status_publis     = this->create_publisher<example_interfaces::msg::String>("Status",10);
    text_publis       = this->create_publisher<example_interfaces::msg::String>("Text",10);
    timer_subs        = this->create_subscription<example_interfaces::msg::Int32>("Timer",10, std::bind(&rosNode::timerCallback,this, std::placeholders::_1) );
}

void rosNode::run()
{
    rclcpp::spin(this->get_node_base_interface());
/*
Bu kısım kendi içinde bir sayaçbaşlatıp bu sayaçtan bir sinyal üretiyordu
 while(stop)
   {
        
       qDebug() << value ;
       value++;
       auto msg = example_interfaces::msg::Int32();
       msg.data = value;
       _publis->publish(msg);
      

       this->msleep(100);
       emit startChanged(value);
   }
*/
  
}

// rclcpp spin_node  
// void rosNode::rosSpin()
// {
//     rclcpp::spin_some(this->get_node_base_interface());
// }

void rosNode::statusPublisher(std::string status)
{
    auto msg = example_interfaces::msg::String();
    msg.data = status;
    status_publis->publish(msg);

}

void rosNode::textPublisher(std::string text)
{
    auto msg = example_interfaces::msg::String();
    msg.data = text;
    text_publis->publish(msg);
}

void rosNode::timerCallback(const example_interfaces::msg::Int32::SharedPtr timer)
{
    value = timer->data;
    emit timerChanged(value);

}

void rosNode::resetClientCallback()
{
    reset_server    = this->create_client<example_interfaces::srv::SetBool>("Reset_Timer");
    while(!reset_server->wait_for_service(std::chrono::seconds(1)))
    {
        RCLCPP_INFO(this->get_logger(), "Wait for the server to be up...");
    }

    auto _request = std::make_shared<example_interfaces::srv::SetBool::Request>();

    _request->data = true;
    auto future   = reset_server->async_send_request(_request);
    
    
    try
    {
        auto response = future.get();
        RCLCPP_INFO(this->get_logger(),"%d",response->success);
    }
    catch(const std::exception &e)
    {
        RCLCPP_WARN(this->get_logger(), "WARN :  while calling service.");
    }

}

void rosNode::resetService()
{
   _thread.push_back(std::thread(std::bind(&rosNode::resetClientCallback, this)));
}
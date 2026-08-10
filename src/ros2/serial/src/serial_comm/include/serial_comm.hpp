#ifndef __SERIAL_COMM_HPP__
#define __SERIAL_COMM_HPP__

// #include <stdio.h>
// #include <string.h>

#include <iostream>
#include <cstring>

#include <fcntl.h>   // Dosya kontrollerini içerir (O_RDWR)
#include <errno.h> 
#include <termios.h> // POSIX terminal kontrolleri için kullanılır (Yapılandırma ayarları)
#include <unistd.h>  // read(), write(), close()

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.hpp"

#define HEADER_     27
#define FOOTHER_    71
#define HEADER_LEN  1
#define FOOTHER_LEN 1
#define PAYLOAD_LEN 5

using JoyMsg = sensor_msgs::msg::Joy;

double mapValues(double au32_IN, double au32_INmin, double au32_INmax, double au32_OUTmin, double au32_OUTmax)
{   
  return ((((au32_IN - au32_INmin)*(au32_OUTmax - au32_OUTmin))/(au32_INmax - au32_INmin)) + au32_OUTmin);
}

class SerialComm : public rclcpp::Node{
    private:
        std::string dev_;
        bool flag_ = false;
        int port_;
        int state = 0;
        int button[4];
        float joy[4];
        uint8_t buffer[7];
        uint8_t curr_byte , prev_byte ;
        JoyMsg msg;
        rclcpp::Publisher<JoyMsg>::SharedPtr joy_pub;
        rclcpp::TimerBase::SharedPtr timer_;  

    public:
        SerialComm(std::string dev_name = FILE_NAME);
        ~SerialComm();
        bool initPort();
        bool configure();
        void dataRead();
};

#endif
#ifndef ROSNODE_H
#define ROSNODE_H

#include <QObject>
#include <QThread>
// #include <QTimer>
// #include <QMutex>
#include <QDebug>
#include <QMessageBox>
#include <rclcpp/rclcpp.hpp>
#include <example_interfaces/msg/int32.hpp>
#include <example_interfaces/msg/string.hpp>
#include <example_interfaces/srv/set_bool.hpp>



class rosNode: public QThread, public rclcpp::Node
{
    Q_OBJECT
private:
    rclcpp::Publisher<example_interfaces::msg::String>::SharedPtr status_publis;
    rclcpp::Publisher<example_interfaces::msg::String>::SharedPtr text_publis;
    rclcpp::Subscription<example_interfaces::msg::Int32>::SharedPtr timer_subs;
    // rclcpp::Service<example_interfaces::srv::AddTwoInts>:.SharedPtr reset_server;
    rclcpp::Client<example_interfaces::srv::SetBool>::SharedPtr reset_server;


public:
    bool stop;
    uint16_t value;
    std::vector<std::thread> _thread;


    rosNode(QObject* param=0);
    ~rosNode() { rclcpp::shutdown();}
    void run();
    void timerCallback(const example_interfaces::msg::Int32::SharedPtr timer);
    void resetClientCallback();

signals:
    void timerChanged(int i);


public slots:
    void statusPublisher(std::string status);
    void textPublisher(std::string text);
    void resetService();
};




#endif // ROSNODE_H

#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>
#include <gz/msgs.hh>
#include <gz/transport.hh>


static std::atomic<bool> g_terminatePub(false);
void signal_handler(int _signal)
{
  if (_signal == SIGINT || _signal == SIGTERM)
    g_terminatePub = true;
}
int main(int argc, char **argv)
{
  // Install a signal handler for SIGINT and SIGTERM.
  std::signal(SIGINT,  signal_handler);
  std::signal(SIGTERM, signal_handler);
  // Create a transport node and advertise a topic.
  gz::transport::Node node;
  std::string topic = "/foo";
  std::string topic_2 = "/motor_pub_flags";
  auto pub = node.Advertise<gz::msgs::StringMsg>(topic);
  auto pub_2 = node.Advertise<gz::msgs::Int32>(topic_2);
  if (!pub || !pub_2)
  {
    std::cerr << "Error advertising topic [" << topic << "]" << std::endl;
    return -1;
  }
  // Prepare the message.
  gz::msgs::StringMsg msg;
  gz::msgs::Int32 data;
  data.set_data(1);
  msg.set_data("HELLO");
  // Publish messages at 1Hz.
  while (!g_terminatePub)
  {
    if (!pub.Publish(msg))
      break;
    if (!pub_2.Publish(data))
      break;
    std::cout << "Publishing hello on topic [" << topic << "]" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  return 0;
}
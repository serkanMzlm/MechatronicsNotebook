#include <iostream>
#include <string>
#include <gz/msgs.hh>
#include <gz/transport.hh>
void cb(const gz::msgs::StringMsg &_msg)
{
  std::cout << "Msg: " << _msg.data() << std::endl << std::endl;
}
void cb2(const gz::msgs::Int32 &_msg)
{
  std::cout << "Msg 2: " << _msg.data() << std::endl << std::endl;
}
int main(int argc, char **argv)
{
  gz::transport::Node node;
  std::string topic = "/foo";
  std::string topic_2 = "/motor_pub_flags";
  // Subscribe to a topic by registering a callback.
  if (!node.Subscribe(topic, cb) || !node.Subscribe(topic_2, cb2))
  {
    std::cerr << "Error subscribing to topic [" << topic << "]" << std::endl;
    return -1;
  }
  // Zzzzzz.
  gz::transport::waitForShutdown();
  return 0;
}
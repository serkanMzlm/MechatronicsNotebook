#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from example_interfaces.srv import AddTwoInts

class MyNode(Node):
    def __init__(self):
        super().__init__("py_server_node")
        self.get_logger().info("Server:")
        self.server_ = self.create_service(AddTwoInts, "add", self.addCallback)
    
    def addCallback(self, request, response):
        response.sum = request.a + request.b
        self.get_logger().info(str(request.b) + "+" + str(request.a) + "=" + str(response.sum)) 
        return response


def main(args=None):
    rclpy.init(args=args)
    rclpy.spin(MyNode())
    rclpy.shutdown()

if __name__ == "__main__":
    main()
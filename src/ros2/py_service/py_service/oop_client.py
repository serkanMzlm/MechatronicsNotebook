#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from example_interfaces.srv import AddTwoInts
from functools import partial


class MyNode(Node):
    def __init__(self):
        super().__init__("py_oop_client")
        self.get_logger().info("oop_client:")
        self.call_add_two_ints_server(12,20)
        self.call_add_two_ints_server(32,20)


    def call_add_two_ints_server(self, a, b):
        client_ = self.create_client(AddTwoInts, "add")
        # Service oluşana kadar bu kısımda bekler
        while not client_.wait_for_service(1.0):
            self.get_logger.warn("Waiting for Server ...")

        request = AddTwoInts.Request()
        request.a = a
        request.b = b
        # Veriyi gönderir
        future = client_.call_async(request)
        future.add_done_callback(partial(self.addCallback, a=a, b=b))

    def addCallback(self, future, a, b):

        try:
            response = future.result()
            self.get_logger().info(str(a)+"+"+str(b)+"="+str(response.sum))

        except Exception as e:
            self.get_logger.error("Server call failed %r" % (e,))


def main(args=None):
    rclpy.init(args=args)
    rclpy.spin(MyNode())
    rclpy.shutdown()


if __name__ == "__main__":
    main()
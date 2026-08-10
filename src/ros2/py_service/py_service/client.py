#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from example_interfaces.srv import AddTwoInts

def main(args=None):
    rclpy.init(args=args)
    node = Node("py_client_node")
    node.get_logger().info("Client:")
    client_ = node.create_client(AddTwoInts, "add")
    while not client_.wait_for_service(1.0):                           #Service oluşana kadar bu kısımda bekler
        node.get_logger.warn("Waiting for Server ...")

    request = AddTwoInts.Request()
    request.a = 12
    request.b = 23
    future = client_.call_async(request)                              # Veriyi gönderir 
    rclpy.spin_until_future_complete(node, future)                    # Cevap gelene kadar bekler.

    try:
        response = future.result()
        node.get_logger().info(str(request.a)+"+"+str(request.b)+"="+str(response.sum))

    except Exception as e:
        node.get_logger.error("Server call failed %r"%(e,))

    rclpy.shutdown()

if __name__ == "__main__":
    main()
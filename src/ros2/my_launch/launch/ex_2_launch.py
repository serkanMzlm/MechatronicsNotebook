from launch import LaunchDescription
from launch_ros.actions import Node 
from launch.actions import ExecuteProcess


def generate_launch_description():

    return LaunchDescription([
        Node(
            package="cpp_topic",
            executable="publisher_node",
            name=  "my_talker",    #Node isminide değiştirir.
            ros_arguments=[
                "--log-level", "my_talker:=debug",
                "--remap", "cpp_topic_int:=my_talker_cp"
            ],
            # remappings=[
            #     ("cpp_topic_int", "my_talker")   # Topic adını değiştirir. 
            # ],
            # ,respawn=True #Node kapanırsa tekrardan açılmasını sağlıyor
        ),

        Node(
            package="cpp_topic",
            executable="subscriber_node",
            name= "my_listener",
            remappings=[ ("cpp_topic_int", "my_talker") ]#Direkt değişkene atanıp da yapılabilir  topic_name = ("cpp_test_counter", "my_talker")
        ),
        ExecuteProcess(
            cmd=['ros2', 'topic', 'list'],
            output='screen'
        ),
        ExecuteProcess(
            cmd=['ls'],
            output='screen'
        )
])
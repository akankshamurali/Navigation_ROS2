import os

from pkg_resources import declare_namespace
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition, UnlessCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, LaunchConfiguration, PythonExpression
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # use_rviz = LaunchConfiguration('use_rviz')
    
    
    remappings = [('/tf', 'tf'),
                ('/tf_static', 'tf_static')]

    declare_turtle_cmd = Node(
        package='turtlesim',
        executable='turtlesim_node',
        name='sim'
    )
    
    declare__target_cmd = DeclareLaunchArgument(
            name = 'target_frame', 
            default_value='turtle1',
            description='Target frame name.'
        )
        
    declare_tf_cmd = Node(
            package='testing',
            executable='test_listener',
            name='listener',
            parameters=[
                {'target_frame': LaunchConfiguration('target_frame')}
            ]
        )

    start_rviz_file_cmd = Node(
    # condition=IfCondition(use_rviz),
    package='rviz2',
    executable='rviz2',
    name='rviz2',
    output='screen',
       )

    #declare_mimic_cmd = Node(
            #package='tf2_examples',
            #executable='test',
            #name='broadcaster1',
            #parameters=[
               # {'turtlename': 'turtle1'}
           # ]
    #)

    ld = LaunchDescription()
 

    ld.add_action(declare_turtle_cmd)
    ld.add_action(start_rviz_file_cmd)
    ld.add_action(declare__target_cmd)
    ld.add_action(declare_tf_cmd)
    #ld.add_action(declare_mimic_cmd)
   
  
    return ld
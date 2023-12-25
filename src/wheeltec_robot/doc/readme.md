# 串口通讯与ROS的话题交互

注意：如果您的工作空间已经有了`wheeltec_robot.h`和`wheeltec_robot.cpp`文件，请检查`wheeltec_robot.h`的第一行。如果不是`//Version: beta1.0`，那么请将您的工作空间下的`wheeltec_robot.h`和`wheeltec_robot.cpp`文件替换为本目录下的`wheeltec_robot.h`和`wheeltec_robot.cpp`文件。

注意，如果您要更改这个文件的相关内容，请加上包含修改时间在内的修改注释，并为原始代码以注释而非删除的形式取消其在文件中发挥的作用。

## 话题的更改

在`wheeltec_robot.h`中，定义了如下常字符集指针：

```c++
std::string const VELOSITY_CALL_BACK_TOPIC = "mcmd_vel";//2023/12/18. 17:48
std::string const VOLTAGE_TOPIC = "PowerVoltage";//2023/12/18. 17:53
std::string const ODOM_TOPIC = "odom"; //2023/12/18. 17:53
std::string const IMU_TOPIC = "imu"; //2023/12/18. 17:54
```

它们分别是速度发送话题（通过串口向小车发速度信息）、电压信息话题、ODOM话题、IMU话题的名称。可以在这里对其进行全局修改。

## 使用速度控制话题建立ROS与stm32间的通信

在`wheeltec_robot`工程中，定义了如下函数：

```c++
void Cmd_Vel_Callback(const geometry_msgs::Twist &twist_aux);      
```

该函数是一个callback函数。当有名称为`VELOSITY_CALL_BACK_TOPIC`（例如本样例中，该话题名称为"mcmd_vel"）的话题被发布（例如`key_example.py`中，`pub.publish(twist)`语句执行后）时，这个节点将会收到发布的话题，调用该函数并将该`twist`变量传递进去。

该函数将会获取`twist`变量下的：

```c++
twist.linear.x;
twist.linear.y;
twist.angular.z;
```

进行计算。

值得注意的是，相对于机器小车自己，y方向不会有速度，因此，实际被我们使用的变量仅有：

```C++
twist.linear.x;
twist.angular.z;
```

也就是说，向该节点发布话题时，修改上述两个变量，可以实现对小车的速度控制。
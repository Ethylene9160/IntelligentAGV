# Hints

## 修改端口位置

在`wheeltec_robot.cpp`中，修改` private_nh.param<std::string>("usart_port_name",  usart_port_name,  "/dev/wheeltec_controller");`中的`"/dev/wheeltec_controller"`，可以直接更改为诸如`ttyUSBx`。

其中, `inline ros::NodeHandle.param(const std::string&param_name, T&param_val,const T& default_val)` 的定义见：[roscpp: ros::NodeHandle Class Reference](https://docs.ros.org/en/noetic/api/roscpp/html/classros_1_1NodeHandle.html#a4d5ed8b983652e587c9fdfaf6c522f3f)

# Master Pieces

## 配置ip

```bash
nano ~/.bashrc
```

通过查找ifconfig中的ipv4地址或者ipv6地址，进行配置。

随后，输入指令：

```bash
source ~/.bashrc
```

## 执行roslaunch

在工作空间中，执行：

```bash
catkin_make
source ./devel/setup.bash
```

## 端口名

```bash
ttyACMO
```

启动更正的端口：在turn_on_wheeltec_robot文件夹的终端下，运行以下语句：

```bash
sudo ./wheeltec_udev.sh
```

然后记得拔插接口。

## 运行

在catkin_ws工作空间下，运行：

```BASH
catkin_make
source ./devel/setup.bash
```

运行节点：

串口通讯节点

```bash
roslaunch turn_on_wheeltec_robot turn_on_wheeltec_robot.launch
```

键盘控制

```bash
roslaunch wheeltec_robot_rc keyboard_teleop.launch
```

## 使用方法

### 控制小车

通过发布话题的方式，能够对小车进行速度控制。

* Python

`基于Twist传输实例变量`

首先，请确保您发布了相关的话题。例如：

```python
pub = rospy.Publisher('~cmd_vel', Twist, queue_size=5)
```

其中，`~cmd_vel`表示在，话题名为`cmd_vel`，但是转换为私有话题，所以订阅别的包下的时候，别加`~`。

然后，确保串口发布程序中，有相关话题的订阅。例如，例程`turn_on_wheeltec_robot`的文件中（即，确保turn_on_wheeltec在正常运行），有对`cmd_vel`话题的订阅。

接下来，创建Twist实例变量。

```python
# ...
Twist twist()
while not rospy.is_shutdown():
    # 例如，我希望发送的数据为，vx=0.1，vy=0.2
    twist.linear.x = 0.1
    twist.linear.y = 0.2
    # 发布
    pub.publish(twist)
    # 此时，将会
    rospy.sleep()
```

这样，就能通过发布话题的方式，给单片机发布话题了。

需要注意的是，单片机的通讯协议频率为20Hz。

* C++

## TODOs

* 机器小车控制部分

> 1. 机器小车的PID参数调节
> 2. 机器小车车轮转速的控制，控制到空转相同PWM控制时实现基本稳定。

* ROS部分

> 1. 实例上手建图、走路等，观察实际效果并进行调节。


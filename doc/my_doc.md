# spark基本操作

## 账户基本信息

* 账户名称：`spark`
* 密码：`spark`



* 自带WiFi：`spark-xxxx`, key: `12345678`



c++ can be written:

`Home/catkin_ws/src/test_pkg/src`·







# 更改日志

1. `Home/.bashrc` line 118-120 add `source ./devel/ setup.bash`

2. `test_pkg/CMakeLists.txt` before `##Install##` add:

   ```bash
   add_executable(步骤3的源文件名
     src/步骤3的源文件名.cpp
   )
   target_link_libraries(步骤3的源文件名
     ${catkin_LIBRARIES}
   )
   ```

   
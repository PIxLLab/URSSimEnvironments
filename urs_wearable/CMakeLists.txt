cmake_minimum_required(VERSION 2.8.3)
project(urs_wearable)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  actionlib
  actionlib_msgs
  geometry_msgs
  hector_quadrotor_interface
  message_generation
  roscpp
  sensor_msgs
  std_msgs
)

# Add Libcuckoo for concurrent hash table
add_subdirectory("../libcuckoo" libcuckoo)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
add_message_files(FILES
  Action.msg
  ActionAscend.msg
  ActionDescend.msg
  ActionGather.msg
  ActionLand.msg
  ActionMove.msg
  ActionScan.msg
  ActionTakeoff.msg
  Area.msg
  Feedback.msg
  Location.msg
  ObjectArea.msg
  ObjectDrone.msg
  ObjectLocation.msg
  Predicate.msg
  PredicateAbove.msg
  PredicateAligned.msg
  PredicateAt.msg
  PredicateCollided.msg
  PredicateHovered.msg
  PredicateIn.msg
  PredicateLowBattery.msg
  PredicateScanned.msg
  State.msg
)

## Generate services in the 'srv' folder
add_service_files(FILES
  AddArea.srv
  AddDroneGoal.srv
  AddLocation.srv
  GetDest.srv
  GetState.srv
  RemoveArea.srv
  RemoveDroneGoal.srv
  RemoveLocation.srv
  SetAltitude.srv
  SetDouble.srv
  SetDroneActionStatus.srv
  SetGoal.srv
  SetOrientation.srv
  SetPosition.srv
)

## Generate actions in the 'action' folder
add_action_files(FILES
  Drone.action
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES actionlib_msgs geometry_msgs sensor_msgs std_msgs
)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES urs_wearable
  CATKIN_DEPENDS hector_uav_msgs message_runtime roscpp sensor_msgs std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  include
  ../libcuckoo
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
add_library(knowledge_base src/knowledge_base.cpp)

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
#add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(knowledge_base ${PROJECT_NAME}_generate_messages_cpp ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
# add_executable(${PROJECT_NAME}_node src/urs_wearable_node.cpp)
add_executable(controller src/controller.cpp)
add_executable(drone_action src/drone_action.cpp)
add_executable(exec_monitor src/exec_monitor.cpp)
add_executable(fake_wearable src/fake_wearable.cpp)
add_executable(keyboard_teleop src/keyboard_teleop.cpp)
add_executable(try_hector src/try_hector.cpp)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(controller ${PROJECT_NAME}_generate_messages_cpp ${catkin_EXPORTED_TARGETS})
add_dependencies(drone_action ${PROJECT_NAME}_generate_messages_cpp ${catkin_EXPORTED_TARGETS})
add_dependencies(exec_monitor ${PROJECT_NAME}_generate_messages_cpp ${catkin_EXPORTED_TARGETS})
add_dependencies(fake_wearable ${PROJECT_NAME}_generate_messages_cpp ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
target_link_libraries(controller ${catkin_LIBRARIES})
target_link_libraries(drone_action ${catkin_LIBRARIES})
target_link_libraries(exec_monitor knowledge_base ${catkin_LIBRARIES})
target_link_libraries(fake_wearable ${catkin_LIBRARIES})
target_link_libraries(keyboard_teleop ${catkin_LIBRARIES})
target_link_libraries(try_hector ${catkin_LIBRARIES})

###########################
## Add external planners ##
###########################

## Update: It turns out the planners do not need to include header files generated from ROS.
## So, just add_subdirectory() is fine.
add_subdirectory(../cpa cpa)

## Using add_subdirectory() takes care of rebuilding when there has been a change.
## But there is no way to specify dependency, as a result it got built before its required headers are generated.
## To get over this, EXCLUDE_FROM_ALL is added so that it will not get built.
## You will need to manually remove EXCLUDE_FROM_ALL and catkin_make after the headers have been generated.
#add_subdirectory(cpa)# EXCLUDE_FROM_ALL)

## Using ExternalProject_Add() only builds once. It does not rebuild when there has been a change.
#include(ExternalProject)
#
#ExternalProject_Add(cpa
#  SOURCE_DIR "${CMAKE_SOURCE_DIR}/urs_wearable/cpa"
#  DEPENDS ${PROJECT_NAME}_generate_messages_cpp
#  CMAKE_ARGS -DCMAKE_PROJECT_cpa+_INCLUDE=${CMAKE_SOURCE_DIR}/catkin_include.cmake
#  INSTALL_COMMAND cmake -E echo "Skipping install step"
#)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_urs_wearable.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)

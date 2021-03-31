#添加boost include路径
INCLUDE_DIRECTORIES(BEFORE /home/chengan/boost/include/boost)

#添加boost lib路径
LINK_DIRECTORIES(/home/chengan/boost/lib)


ADD_EXECUTABLE(main main.cpp)

TARGET_LINK_LIBRARIES(main boost_system)
TARGET_LINK_LIBRARIES(main boost_thread)

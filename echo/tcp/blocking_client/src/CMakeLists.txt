#添加boost include路径
INCLUDE_DIRECTORIES(BEFORE /home/chengan/boost/include/boost)

#添加project include路径
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

#添加boost lib路径
LINK_DIRECTORIES(/home/chengan/boost/lib)




ADD_EXECUTABLE(main main.cpp)

TARGET_LINK_LIBRARIES(main boost_system)

# 否则会造成
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
TARGET_LINK_LIBRARIES(main pthread)
TARGET_LINK_LIBRARIES(main boost_thread)

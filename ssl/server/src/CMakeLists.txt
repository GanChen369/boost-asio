#添加boost include路径
INCLUDE_DIRECTORIES(BEFORE /home/chengan/boost/include/boost)

# /usr/local/include/openssl
INCLUDE_DIRECTORIES(BEFORE /home/chengan/openssl/openssl-1.1.1c/include/openssl)

#添加boost lib路径
LINK_DIRECTORIES(/home/chengan/boost/lib)

#/usr/lib/x86_64-linux-gnu/
LINK_DIRECTORIES(/home/chengan/openssl/openssl-1.1.1c)


ADD_EXECUTABLE(main main.cpp)

TARGET_LINK_LIBRARIES(main boost_system)

# 否则会造成
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
TARGET_LINK_LIBRARIES(main pthread)
TARGET_LINK_LIBRARIES(main boost_thread)
TARGET_LINK_LIBRARIES(main ssl)
TARGET_LINK_LIBRARIES(main crypto)
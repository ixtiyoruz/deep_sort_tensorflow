add_executable(yolo_console_dll yolo_console_dll.cpp)
LINK_DIRECTORIES('./')
target_link_libraries(yolo_console_dll PRIVATE deepsort)
target_link_libraries(yolo_console_dll PRIVATE darknet)

find_package( OpenCV REQUIRED )
list(APPEND OpenCV_LINKED_COMPONENTS ${OpenCV_LIBS})

target_link_libraries(yolo_console_dll PRIVATE ${OpenCV_LINKED_COMPONENTS})

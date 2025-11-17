include("D:/Innovate_course_B/InnovateB/build/Desktop_Qt_6_9_1_MinGW_64_bit-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/InnovateB-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE D:/Innovate_course_B/InnovateB/build/Desktop_Qt_6_9_1_MinGW_64_bit-Debug/InnovateB.exe
    GENERATE_QT_CONF
)

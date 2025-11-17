include("D:/Innovate_course_B/InnovateB/build/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/InnovateB-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE D:/Innovate_course_B/InnovateB/build/InnovateB.exe
    GENERATE_QT_CONF
)

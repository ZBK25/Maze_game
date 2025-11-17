# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\InnovateB_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\InnovateB_autogen.dir\\ParseCache.txt"
  "InnovateB_autogen"
  )
endif()

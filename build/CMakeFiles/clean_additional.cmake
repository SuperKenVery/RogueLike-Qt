# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/RogueLike-Qt_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/RogueLike-Qt_autogen.dir/ParseCache.txt"
  "RogueLike-Qt_autogen"
  )
endif()

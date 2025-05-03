# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/draft_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/draft_autogen.dir/ParseCache.txt"
  "draft_autogen"
  )
endif()

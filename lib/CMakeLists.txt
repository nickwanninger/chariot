# make sure the userspace programs and libraries use this
add_link_options(LINKER:-T ${USER_LINKER_SCRIPT} -z max-page-size=0x1000)
# add_link_options(-z max-page-size=0x1000)

include_directories(
	# C++ stuff
	../toolchain/src/gcc-${CONFIG_GCC_VERSION}/libstdc++-v3/include/
	../toolchain/src/gcc-${CONFIG_GCC_VERSION}/libstdc++-v3/include/c
	../toolchain/src/gcc-${CONFIG_GCC_VERSION}/libstdc++-v3/include/c_std
	../toolchain/src/gcc-${CONFIG_GCC_VERSION}/libstdc++-v3/include/c_global
	../toolchain/src/gcc-${CONFIG_GCC_VERSION}/libstdc++-v3/include/std
	../toolchain/src/gcc-${CONFIG_GCC_VERSION}/libstdc++-v3/libsupc++

	../include
)



MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()


SUBDIRLIST(USER_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/)
FOREACH(subdir ${USER_LIBS})
	add_subdirectory(${subdir})
ENDFOREACH()

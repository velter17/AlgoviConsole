add_executable ( testlib_hcmp "hcmp.cpp" ) # two signed huge integers
target_link_libraries( testlib_hcmp testlib)

add_executable ( testlib_icmp "icmp.cpp" ) # two signed integers
target_link_libraries( testlib_icmp testlib)

add_executable ( testlib_lcmp "lcmp.cpp" ) # file as seq of tokens in lines
target_link_libraries( testlib_lcmp testlib)

add_executable ( testlib_ncmp "ncmp.cpp" ) # ordered seq of signed integers
target_link_libraries( testlib_ncmp testlib)

add_executable ( testlib_rcmp "rcmp.cpp" ) # two doubles
target_link_libraries( testlib_rcmp testlib)

add_executable ( testlib_rcmp4 "rcmp4.cpp" ) # seq doubles .4
target_link_libraries( testlib_rcmp4 testlib)

add_executable ( testlib_rcmp6 "rcmp6.cpp" ) # seq doubles .6
target_link_libraries( testlib_rcmp6 testlib)

add_executable ( testlib_rcmp9 "rcmp9.cpp" ) # seq doubles .9
target_link_libraries( testlib_rcmp9 testlib)

add_executable ( testlib_wcmp "wcmp.cpp" ) # seq of tokens
target_link_libraries( testlib_wcmp testlib)

add_executable ( testlib_uncmp "uncmp.cpp" ) # unordered seq int
target_link_libraries( testlib_uncmp testlib)

add_executable ( testlib_yesno "yesno.cpp" )
target_link_libraries( testlib_yesno testlib)

install (
   TARGETS
   testlib_hcmp
   testlib_icmp
   testlib_lcmp
   testlib_ncmp
   testlib_rcmp
   testlib_wcmp
   testlib_rcmp4
   testlib_rcmp6
   testlib_rcmp9
   testlib_uncmp
   testlib_yesno
   RUNTIME DESTINATION ${BIN_DIR}/checkers/
)

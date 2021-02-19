add_executable( ego_test ego_test.cpp ) 
target_link_libraries(ego_test
  PRIVATE
    # Google Testをリンクする
    gtest_main
  )
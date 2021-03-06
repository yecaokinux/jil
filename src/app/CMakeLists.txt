if(UNIX)
    add_definitions(-D_GLIBCXX_USE_WCHAR_T -std=c++11)
endif()

# NOTE:
# You have to build libconfig and install to home directory (~/include and ~/lib).
# I don't add a cmake project for Mac because I don't know how to do it.
#
if(APPLE)
    set(LIBCONFIG_INCLUDE_DIR $ENV{HOME}/include)
    set(LIBCONFIG_LIBRARY_DIR $ENV{HOME}/lib)

    include_directories(${LIBCONFIG_INCLUDE_DIR})
    link_directories(${LIBCONFIG_LIBRARY_DIR})
endif()

set(PREF_SRCS
    pref/common.cc
    pref/common.h
    pref/dialog_base.cc
    pref/dialog_base.h
    pref/editor_comment_page.cc
    pref/editor_comment_page.h
    pref/editor_dialog.cc
    pref/editor_dialog.h
    pref/editor_general_page.cc
    pref/editor_general_page.h
    pref/editor_indent_page.cc
    pref/editor_indent_page.h
    pref/global_dialog.cc
    pref/global_dialog.h
    pref/global_font_page.cc
    pref/global_font_page.h
    pref/global_general_page.cc
    pref/global_general_page.h
    pref/global_theme_page.cc
    pref/global_theme_page.h
    pref/option_list_ctrl.cc
    pref/option_list_ctrl.h
    )

set(SRCS
    ${PREF_SRCS}
    about_dialog.cc
    about_dialog.h
    app.cc
    app.h
    binding_config.cc
    binding_config.h
    book_ctrl.cc
    book_ctrl.h
    book_page.h
    book_frame.cc
    book_frame.h
    compile_config.h
    config.cc
    config.h
    defs.cc
    defs.h
    find_result_page.cc
    find_result_page.h
    find_panel.cc
    find_panel.h
    find_thread.cc
    find_thread.h
    font_util.cc
    font_util.h
    ft_config.cc
    ft_config.h
    goto_dialog.cc
    goto_dialog.h
    id.h
    i18n_strings.h
    i18n_util.h
    lex_config.cc
    lex_config.h
    log.cc
    log.h
    navigation_dialog.cc
    navigation_dialog.h
    option.cc
    option.h
    option_config.cc
    option_config.h
    page_window.cc
    page_window.h
    popup_menu.cc
    popup_menu.h
    save.cc
    save.h
    session.cc
    session.h
    splitter.cc
    splitter.h
    status_bar.cc
    status_bar.h
    status_fields_config.cc
    status_fields_config.h
    text_book.cc
    text_book.h
    text_page.cc
    text_page.h
    theme_config.cc
    theme_config.h
    tool_book.cc
    tool_book.h
    util.cc
    util.h
    )

set(TARGET_NAME app)
set(TARGET_OUTPUT_NAME jil)

# Resource files
if(WIN32)
    set(JIL_RC ${CMAKE_CURRENT_SOURCE_DIR}/app.rc)
elseif(APPLE)
    set(JIL_RC
        ${CMAKE_CURRENT_SOURCE_DIR}/icon/editor.icns
        )
    # Add icons to bundle.
    set_source_files_properties(${JIL_RC} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

# Include resource files in the target.
set(SRCS ${SRCS} ${JIL_RC})

add_executable(${TARGET_NAME} WIN32 MACOSX_BUNDLE ${SRCS})

COMMON_TARGET_SETTING(${TARGET_NAME} SRCS)

set(EXTRA_LIBS
    base
    ui
    editor
    pugixml
    uchardet
    lua
    )

if(APPLE)
    # The static lib of libconfig on Mac is named as "libconfig.a".
    # the prefix "lib" doesn't have to be specified in the link option.
    set(EXTRA_LIBS ${EXTRA_LIBS} config)
else()
    set(EXTRA_LIBS ${EXTRA_LIBS} libconfig)
endif()

# NOTE: Put ${wxWidgets_LIBRARIES} after others, or there will be link errors in Linux/GTK+.
target_link_libraries(${TARGET_NAME} ${EXTRA_LIBS} ${wxWidgets_LIBRARIES})

set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_NAME ${TARGET_OUTPUT_NAME})
if(MSVC)
    set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_FLAGS "/wd4290")
endif()

# Installation

message(STATUS "[debug] Project binary dir: " ${PROJECT_BINARY_DIR})
message(STATUS "[debug] Project source dir: " ${PROJECT_SOURCE_DIR})
message(STATUS "[debug] CMake install prefix: " ${CMAKE_INSTALL_PREFIX})

if(WIN32)
    install(FILES ${PROJECT_SOURCE_DIR}/data/jilfiles/options.cfg DESTINATION .)
    install(FILES ${PROJECT_SOURCE_DIR}/data/jilfiles/binding.cfg DESTINATION .)
    install(FILES ${PROJECT_SOURCE_DIR}/data/jilfiles/file_types.cfg DESTINATION .)
    install(FILES ${PROJECT_SOURCE_DIR}/data/jilfiles/status_fields.cfg DESTINATION .)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/jilfiles/theme DESTINATION .)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/jilfiles/ftplugin DESTINATION .)

    if(JIL_ENABLE_VLD)
        install(FILES ${PROJECT_SOURCE_DIR}/third_party/win32/bin/dbghelp.dll DESTINATION .)
        install(FILES ${PROJECT_SOURCE_DIR}/third_party/win32/bin/Microsoft.DTfW.DHL.manifest DESTINATION .)
        install(FILES ${PROJECT_SOURCE_DIR}/third_party/win32/bin/vld_x86.dll DESTINATION .)
    endif()
elseif(APPLE)
    set(MAC_INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/${TARGET_OUTPUT_NAME}.app/Contents/MacOS)
    set(MAC_INSTALL_RES_DIR ${CMAKE_INSTALL_PREFIX}/${TARGET_OUTPUT_NAME}.app/Contents/Resources)

    message(STATUS "Mac install bin dir: " ${MAC_INSTALL_BIN_DIR})
    message(STATUS "Mac install res dir: " ${MAC_INSTALL_RES_DIR})

    # TODO: Is there any variable for the target output dir?
    #install(FILES ${CMAKE_INSTALL_PREFIX}/liblibconfig.dylib DESTINATION ${MAC_INSTALL_BIN_DIR})

    install(FILES ${PROJECT_SOURCE_DIR}/data/jilfiles/options.cfg DESTINATION ${MAC_INSTALL_RES_DIR})
    install(FILES ${PROJECT_SOURCE_DIR}/data/jilfiles/binding.cfg DESTINATION ${MAC_INSTALL_RES_DIR})
    install(FILES ${PROJECT_SOURCE_DIR}/data/jilfiles/file_types.cfg DESTINATION ${MAC_INSTALL_RES_DIR})
    install(FILES ${PROJECT_SOURCE_DIR}/data/jilfiles/status_fields.cfg DESTINATION ${MAC_INSTALL_RES_DIR})
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/jilfiles/theme DESTINATION ${MAC_INSTALL_RES_DIR})
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/jilfiles/ftplugin DESTINATION ${MAC_INSTALL_RES_DIR})
elseif(UNIX)
    # Install executable to ${CMAKE_INSTALL_PREFIX}.
    install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION .)

    # Install resource files to the same directory.
    install(FILES ${PROJECT_SOURCE_DIR}/data/jilfiles/options.cfg DESTINATION .)
    install(FILES ${PROJECT_SOURCE_DIR}/data/jilfiles/binding.cfg DESTINATION .)
    install(FILES ${PROJECT_SOURCE_DIR}/data/jilfiles/file_types.cfg DESTINATION .)
    install(FILES ${PROJECT_SOURCE_DIR}/data/jilfiles/status_fields.cfg DESTINATION .)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/jilfiles/theme DESTINATION .)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/jilfiles/ftplugin DESTINATION .)
endif()


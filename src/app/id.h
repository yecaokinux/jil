#ifndef JIL_ID_H_
#define JIL_ID_H_
#pragma once

// Window ID, menu ID, etc.

#include "wx/defs.h"
#include "app/compile_config.h"
#include "app/defs.h"

namespace jil {

enum {
  ID_JIL_BEGIN = wxID_HIGHEST + 100,

  ID_STATUS_BAR,
  ID_STATUS_MSG_TIMER,

  ID_TEXT_BOOK,
  ID_TOOL_BOOK,

  ID_TAB_EXPAND_BUTTON,

  ID_TEXT_WINDOW,

  ID_FIND_PANEL,
  ID_FIND_RESULT_PAGE,
  ID_FIND_THREAD_EVENT,

  ID_GOTO_DIALOG,
  ID_NAVIGATION_DIALOG,

  // Find panel
  ID_FP_FOLDER_LABEL,
  ID_FP_FOLDER_TEXTCTRL,
  ID_FP_ADD_FOLDER_BUTTON,
  ID_FP_FIND_TEXTCTRL,
  ID_FP_REPLACE_TEXTCTRL,
  ID_FP_LOCATION_BUTTON,
  ID_FP_USE_REGEX_TBUTTON,
  ID_FP_CASE_SENSITIVE_TBUTTON,
  ID_FP_MATCH_WORD_TBUTTON,
  ID_FP_REVERSELY_TBUTTON,
  ID_FP_FIND_BUTTON,
  ID_FP_FIND_ALL_BUTTON,
  ID_FP_REPLACE_BUTTON,
  ID_FP_REPLACE_ALL_BUTTON,
  // Location menu
  ID_FP_MENU_CURRENT_PAGE,
  ID_FP_MENU_ALL_PAGES,
  ID_FP_MENU_FOLDERS,

  // 32 themes at most.
  ID_MENU_THEME_BEGIN,
  ID_MENU_THEME_END = ID_MENU_THEME_BEGIN + kMaxThemes,

  ID_MENU_ICON_SMALL,
  ID_MENU_ICON_MEDIUM,
  ID_MENU_ICON_LARGE,

  // File menu
  ID_MENU_FILE_BEGIN,
  ID_MENU_FILE_NEW = ID_MENU_FILE_BEGIN,
  ID_MENU_FILE_OPEN,
  ID_MENU_FILE_CLOSE,
  ID_MENU_FILE_CLOSE_ALL,
  ID_MENU_FILE_CLOSE_ALL_BUT_THIS,
  ID_MENU_FILE_SAVE,
  ID_MENU_FILE_SAVE_AS,
  ID_MENU_FILE_SAVE_ALL,
  ID_MENU_FILE_COPY_PATH,
  ID_MENU_FILE_OPEN_FOLDER,
  ID_MENU_FILE_NEXT_STACK_PAGE,
  ID_MENU_FILE_PREV_STACK_PAGE,
  ID_MENU_FILE_END,

  // Recent Files sub menu
  // TODO: Add ID_MENU_FILE_CLEAR_RECENT_FILES
  ID_MENU_FILE_RECENT_FILE_0,
  // Recent file 1, 2, ..., 8.
  ID_MENU_FILE_RECENT_FILE_9 = ID_MENU_FILE_RECENT_FILE_0 + 9,

  // Edit menu
  ID_MENU_EDIT_BEGIN,
  ID_MENU_EDIT_UNDO = ID_MENU_EDIT_BEGIN,
  ID_MENU_EDIT_REDO,
  ID_MENU_EDIT_CUT,
  ID_MENU_EDIT_COPY,
  ID_MENU_EDIT_PASTE,
  ID_MENU_EDIT_INCREASE_INDENT,
  ID_MENU_EDIT_DECREASE_INDENT,
  ID_MENU_EDIT_AUTO_INDENT,
  ID_MENU_EDIT_COMMENT,
  ID_MENU_EDIT_UNCOMMENT,
  ID_MENU_EDIT_FIND,
  ID_MENU_EDIT_REPLACE,
  ID_MENU_EDIT_FIND_NEXT,
  ID_MENU_EDIT_FIND_PREV,
  ID_MENU_EDIT_GO_TO,

  // Items for find result page.
  ID_MENU_EDIT_CLEAR_ALL,
  ID_MENU_EDIT_GO_TO_LOCATION,

  ID_MENU_EDIT_END,

  // View menu
  ID_MENU_VIEW_BEGIN,
  ID_MENU_VIEW_WRAP = ID_MENU_VIEW_BEGIN,
  ID_MENU_VIEW_SHOW_NUMBER,
  ID_MENU_VIEW_SHOW_SPACE,
  ID_MENU_VIEW_FULL_SCREEN,
  ID_MENU_VIEW_END,

  // Tools menu
  ID_MENU_TOOLS_BEGIN,
  ID_MENU_TOOLS_RESCAN_LEX = ID_MENU_TOOLS_BEGIN,
  ID_MENU_TOOLS_ERRORS,
  ID_MENU_TOOLS_END,

  // Preferences menu
  ID_MENU_PREFS_EDITOR_BEGIN,
  ID_MENU_PREFS_EDITOR_END = ID_MENU_PREFS_EDITOR_BEGIN + kMaxFileTypes,

  // Help menu
  ID_MENU_HELP_BEGIN,
  ID_MENU_HELP_GITHUB,
  ID_MENU_HELP_END,

  // Tool book tab right click menu.
  // NOTE: Don't reuse the IDs for text book!
  ID_MENU_TB_CLOSE,
  ID_MENU_TB_CLOSE_ALL,
  ID_MENU_TB_CLOSE_ALL_BUT_THIS,

  // Tab related menu on status bar.
  ID_MENU_TAB_BEGIN,
  ID_MENU_EXPAND_TAB = ID_MENU_TAB_BEGIN,
  ID_MENU_TAB_STOP_0,
  ID_MENU_TAB_STOP_8 = ID_MENU_TAB_STOP_0 + 7,
  ID_MENU_GUESS_TAB_OPTIONS,
  ID_MENU_RETAB,
  ID_MENU_TAB_END,

  // Encoding menu on status bar.
  // NOTE: The order must be consistent with editor::EncId.
  ID_MENU_ENCODING_BEGIN,
  ID_MENU_ENCODING_UTF8 = ID_MENU_ENCODING_BEGIN,
  ID_MENU_ENCODING_UTF8_BOM,
  ID_MENU_ENCODING_UTF16_BE,
  ID_MENU_ENCODING_UTF16_LE,
  ID_MENU_ENCODING_GB18030,
  ID_MENU_ENCODING_BIG5,
  ID_MENU_ENCODING_SHIFT_JIS,
  ID_MENU_ENCODING_EUC_JP,
  ID_MENU_ENCODING_KOI8_R,
  ID_MENU_ENCODING_ISO_8859_1,
  ID_MENU_ENCODING_ISO_8859_2,
  ID_MENU_ENCODING_ISO_8859_5,
  ID_MENU_ENCODING_ISO_8859_7,
  ID_MENU_ENCODING_TIS_620,
  ID_MENU_ENCODING_WINDOWS_1250,
  ID_MENU_ENCODING_WINDOWS_1251,
  ID_MENU_ENCODING_WINDOWS_1253,
  ID_MENU_ENCODING_MAC_CYRILLIC,
  ID_MENU_ENCODING_END,

  // File format menu on status bar.
  ID_MENU_FILE_FORMAT_BEGIN,
  ID_MENU_FILE_FORMAT_CRLF = ID_MENU_FILE_FORMAT_BEGIN,
  ID_MENU_FILE_FORMAT_LF,
  ID_MENU_FILE_FORMAT_CR,
  ID_MENU_FILE_FORMAT_END,

  // File type menu on status bar.
  ID_MENU_FILE_TYPE_BEGIN,
  ID_MENU_FILE_TYPE_END = ID_MENU_FILE_TYPE_BEGIN + kMaxFileTypes,

  // History menu on find panel.
  ID_MENU_FIND_HISTORY_BEGIN,
  ID_MENU_FIND_HISTORY_END = ID_MENU_FIND_HISTORY_BEGIN + kFindHistoryLimit,
  ID_MENU_REPLACE_HISTORY_BEGIN,
  ID_MENU_REPLACE_HISTORY_END = ID_MENU_REPLACE_HISTORY_BEGIN + kFindHistoryLimit,
};

}  // namespace jil

#endif  // JIL_ID_H_

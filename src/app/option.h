#ifndef JIL_OPTION_H_
#define JIL_OPTION_H_
#pragma once

#include <string>
#include "wx/string.h"
#include "wx/font.h"
#include "editor/defs.h"

namespace jil {

// Line padding range.
const int kMinLinePadding = 1;
const int kMaxLinePadding = 9;

// Application options.
class Options {
public:
  Options() {
    Init();
  }

  void Init() {
    cjk_filters = 0;
    line_padding = 1;
    switch_cwd = false;
    restore_files = true;
    show_path = true;
  }

  int cjk_filters;

  // Encoding for new created file.
  editor::Encoding file_encoding;

  // Font for text editor.
  wxFont font;

  // Font for GUI components (tab area, status bar, etc.).
  wxFont gui_font;

  // Theme name.
  wxString theme;

  // Spacing at the top and bottom of a line.
  int line_padding;

  // Switch Current Working Dir when switch text page.
  bool switch_cwd;

  // Restore last open files.
  bool restore_files;

  // Show file path in the title bar.
  bool show_path;
};

}  // namespace jil

#endif  // JIL_OPTION_H_

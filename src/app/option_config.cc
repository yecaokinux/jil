#include "app/option_config.h"
#include <vector>
#include "boost/algorithm/string.hpp"
#include "uchardet/nscore.h"
#include "uchardet/nsUniversalDetector.h"
#include "wx/log.h"
#include "wx/filename.h"
#include "wx/settings.h"
#include "editor/util.h"
#include "editor/option.h"
#include "app/config.h"
#include "app/defs.h"
#include "app/font_util.h"

namespace jil {

////////////////////////////////////////////////////////////////////////////////
// Option names.
// Consistent with names in config file.

// Names of application options.
const char* const OPT_S_CJK = "cjk";
const char* const OPT_S_FILE_ENCODING = "file_encoding";
const char* const OPT_S_THEME = "theme";
const char* const OPT_S_ICON_SIZE = "icon_size";
const char* const OPT_G_FONTS = "fonts";
const char* const OPT_F_TEXT = "text";
const char* const OPT_F_TABS = "tabs";
const char* const OPT_F_STATUS_BAR = "status_bar";
const char* const OPT_F_FIND_PANEL = "find_panel";
const char* const OPT_I_LINE_PADDING = "line_padding";
const char* const OPT_B_SWITCH_CWD = "switch_cwd";
const char* const OPT_B_RESTORE_FILES = "restore_files";
const char* const OPT_B_SHOW_PATH = "show_path";

// Names of editor options.
const char* const OPT_B_WRAP = "wrap";
const char* const OPT_I_TAB_STOP = "tab_stop";
const char* const OPT_B_EXPAND_TAB = "expand_tab";
const char* const OPT_B_GUESS_TAB = "guess_tab";
const char* const OPT_B_SHOW_NUMBER = "show_number";
const char* const OPT_B_SHOW_SPACE = "show_space";
const char* const OPT_B_SHOW_HSCROLLBAR = "show_hscrollbar";
const char* const OPT_S_DELIMITERS = "delimiters";
const char* const OPT_IA_RULERS = "rulers";
const char* const OPT_SA_INDENT_KEYS = "indent_keys";
const char* const OPT_G_INDENT = "indent";
const char* const OPT_G_COMMENT = "comment";
const char* const OPT_B_ADD_SPACE = "add_space";
const char* const OPT_B_RESPECT_INDENT = "respect_indent";

////////////////////////////////////////////////////////////////////////////////
// Helper functions.

static int StrToCjkFilters(const std::string& cjk) {
  if (cjk.empty()) {
    return 0;
  }

  int cjk_filters = 0;

  std::vector<std::string> cjk_values;
  boost::split(cjk_values, cjk, boost::is_any_of(", "), boost::token_compress_on);

  for (size_t i = 0; i < cjk_values.size(); ++i) {
    if (cjk_values[i] == "cs") {
      cjk_filters |= NS_FILTER_CHINESE_SIMPLIFIED;
    } else if (cjk_values[i] == "ct") {
      cjk_filters |= NS_FILTER_CHINESE_TRADITIONAL;
    } else if (cjk_values[i] == "c") {
      cjk_filters |= NS_FILTER_CHINESE;
    } else if (cjk_values[i] == "j") {
      cjk_filters |= NS_FILTER_JAPANESE;
    } else if (cjk_values[i] == "k") {
      cjk_filters |= NS_FILTER_KOREAN;
    }
  }

  return cjk_filters;
}

static std::string CjkFiltersToStr(int cjk_filters) {
  std::string cjk_str;

  if ((cjk_filters & NS_FILTER_CHINESE) == NS_FILTER_CHINESE) {
    cjk_str += "c";
  }

  if ((cjk_filters & NS_FILTER_JAPANESE) == NS_FILTER_JAPANESE) {
    if (!cjk_str.empty()) {
      cjk_str += ",";
    }
    cjk_str += "j";
  }

  if ((cjk_filters & NS_FILTER_KOREAN) == NS_FILTER_KOREAN) {
    if (!cjk_str.empty()) {
      cjk_str += ",";
    }
    cjk_str += "k";
  }

  return cjk_str;
}

static int AdjustCjkByLocale(int cjk_filters) {
  wxLocale locale;
  locale.Init();
  int lang = locale.GetLanguage();

  if ((cjk_filters & NS_FILTER_CHINESE_SIMPLIFIED) == 0) {
    if (lang == wxLANGUAGE_CHINESE || lang == wxLANGUAGE_CHINESE_SIMPLIFIED) {
      cjk_filters |= NS_FILTER_CHINESE_SIMPLIFIED;
    }
  }

  if ((cjk_filters & NS_FILTER_CHINESE_TRADITIONAL) == 0) {
    if (lang == wxLANGUAGE_CHINESE ||
        lang == wxLANGUAGE_CHINESE_TRADITIONAL ||
        lang == wxLANGUAGE_CHINESE_HONGKONG ||
        lang == wxLANGUAGE_CHINESE_MACAU ||
        lang == wxLANGUAGE_CHINESE_SINGAPORE ||
        lang == wxLANGUAGE_CHINESE_TAIWAN) {
      cjk_filters |= NS_FILTER_CHINESE_TRADITIONAL;
    }
  }

  if ((cjk_filters & NS_FILTER_JAPANESE) == 0 && lang == wxLANGUAGE_JAPANESE) {
    cjk_filters |= NS_FILTER_JAPANESE;
  }

  if ((cjk_filters & NS_FILTER_KOREAN) == 0 && lang == wxLANGUAGE_KOREAN) {
    cjk_filters |= NS_FILTER_KOREAN;
  }

  return cjk_filters;
}

#define kNormalResolutionStr  "normal"
#define kHighResolutionStr    "high"

static Resolution StrToResolution(const std::string& str) {
  if (str == kHighResolutionStr) {
    return kHighResolution;
  } else {
    return kNormalResolution;
  }
}

std::string ResolutionToStr(Resolution resolution) {
  if (resolution == kHighResolution) {
    return kHighResolutionStr;
  } else {
    return kNormalResolutionStr;
  }
}

static Setting GetSetting(const SettingMap& settings, const char* key, Setting::Type type) {
  SettingMap::const_iterator it = settings.find(key);
  if (it != settings.end() && it->second.type() == type) {
    return it->second;
  }
  return Setting();
}

static bool GetString(const SettingMap& settings, const char* key, std::string* value) {
  Setting setting = GetSetting(settings, key, Setting::kString);
  if (setting) {
    *value = setting.GetString();
    return true;
  }
  return false;
}

static bool GetWString(const SettingMap& settings, const char* key, std::wstring* value) {
  Setting setting = GetSetting(settings, key, Setting::kString);
  if (setting) {
    const char* str = setting.GetString();
    // Assume that the string is pure ascii.
    *value = std::wstring(str, str + strlen(str));
    return true;
  }
  return false;
}

static bool GetWxString(const SettingMap& settings, const char* key, wxString* value) {
  Setting setting = GetSetting(settings, key, Setting::kString);
  if (setting) {
    *value = wxString::FromUTF8(setting.GetString());
    return true;
  }
  return false;
}

static bool GetBool(const SettingMap& settings, const char* key, bool* value) {
  Setting setting = GetSetting(settings, key, Setting::kBool);
  if (setting) {
    *value = setting.GetBool();
    return true;
  }
  return false;
}

static bool GetInt(const SettingMap& settings, const char* key, int* value) {
  Setting setting = GetSetting(settings, key, Setting::kInt);
  if (setting) {
    *value = setting.GetInt();
    return true;
  }
  return false;
}

// <fonts_setting> = {
//    <name> = { name = "Consolas"; size = 10; };
// };
static wxFont GetFont(Setting fonts_setting, const char* name) {
  assert(fonts_setting);

  wxString face_name;
  int point_size = 0;

  if (!fonts_setting.GetFont(name, &face_name, &point_size)) {
    return wxNullFont;
  }

  if (face_name.IsEmpty()) {
    return wxNullFont;
  }

  if (point_size == 0) {
    point_size = GetDefaultFontSize();
  } else if (point_size < kMinFontSize) {
    point_size = kMinFontSize;
  } else if (point_size > kMaxFontSize) {
    point_size = kMaxFontSize;
  }

  return GetGlobalFont(point_size, face_name);
}

static void GetOptionTable(Setting setting, editor::OptionTable* option_table) {
  if (!setting) {
    return;
  }

  int size = setting.size();

  editor::OptionPair option_pair;

  for (int i = 0; i < size; ++i) {
    option_pair.key = setting[i].name();

    int type = setting[i].type();

    if (type == Setting::kBool) {
      option_pair.value = editor::OptionValue::FromBool(setting[i].GetBool());
    } else if (type == Setting::kInt) {
      option_pair.value = editor::OptionValue::FromInt(setting[i].GetInt());
    } else if (type == Setting::kString) {
      option_pair.value = editor::OptionValue::FromString(std::string(setting[i].GetString()));
    }

    if (!option_pair.value.IsEmpty()) {
      option_table->push_back(option_pair);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

static void ValidateFont(wxFont fonts[FONT_COUNT], FontType font_type) {
  if (!fonts[font_type].IsOk()) {
    fonts[font_type] = GetDefaultFont(font_type);
  }
}

static void ValidateFonts(wxFont fonts[FONT_COUNT]) {
  for (int i = 0; i < FONT_COUNT; ++i) {
    ValidateFont(fonts, static_cast<FontType>(i));
  }
}

static void ParseGlobalOptions(const Setting& setting, Options* options) {
  SettingMap setting_map;
  setting.AsMap(&setting_map);

  // CJK
  std::string cjk_str;
  GetString(setting_map, OPT_S_CJK, &cjk_str);
  if (!cjk_str.empty()) {
    options->cjk_filters = StrToCjkFilters(cjk_str);
  }
  options->cjk_filters = AdjustCjkByLocale(options->cjk_filters);

  // File encoding
  std::string fenc_str;
  GetString(setting_map, OPT_S_FILE_ENCODING, &fenc_str);
  if (fenc_str.empty()) {
    fenc_str = "utf-8";
  }
  options->file_encoding = editor::GetEncodingByName(fenc_str);

  // Fonts
  Setting fonts_setting = GetSetting(setting_map, OPT_G_FONTS, Setting::kGroup);
  if (fonts_setting) {
    options->fonts[FONT_TEXT] = GetFont(fonts_setting, OPT_F_TEXT);
    options->fonts[FONT_TABS] = GetFont(fonts_setting, OPT_F_TABS);
    options->fonts[FONT_STATUS_BAR] = GetFont(fonts_setting, OPT_F_STATUS_BAR);
    options->fonts[FONT_FIND_PANEL] = GetFont(fonts_setting, OPT_F_FIND_PANEL);
  }
  ValidateFonts(options->fonts);

  GetInt(setting_map, OPT_I_LINE_PADDING, &options->line_padding);

  GetBool(setting_map, OPT_B_SWITCH_CWD, &options->switch_cwd);

  GetWxString(setting_map, OPT_S_THEME, &options->theme);

  std::string resolution_str;
  GetString(setting_map, OPT_S_ICON_SIZE, &resolution_str);
  options->icon_resolution = StrToResolution(resolution_str);

  GetBool(setting_map, OPT_B_RESTORE_FILES, &options->restore_files);
  GetBool(setting_map, OPT_B_SHOW_PATH, &options->show_path);
}

bool LoadGlobalOptionsFile(const wxString& file, Options* options) {
  if (!wxFileName::FileExists(file)) {
    wxLogInfo(wxT("Options file doesn't exist: %s"), file);
    return false;
  }

  Config config;
  if (!config.Load(file)) {
    wxLogError(wxT("Failed to parse options file: %s"), file);
    return false;
  }

  ParseGlobalOptions(config.Root(), options);

  return true;
}

bool SaveGlobalOptionsFile(const wxString& file, const Options& options) {
  Config config;
  Setting root_setting = config.Root();

  root_setting.SetString(OPT_S_CJK, CjkFiltersToStr(options.cjk_filters).c_str());

  root_setting.SetString(OPT_S_FILE_ENCODING, options.file_encoding.name.c_str());

  root_setting.SetString(OPT_S_THEME, options.theme.ToUTF8().data());

  root_setting.SetString(OPT_S_ICON_SIZE, ResolutionToStr(options.icon_resolution).c_str());

  Setting fonts_setting = root_setting.Add(OPT_G_FONTS, Setting::kGroup);
  fonts_setting.SetFont(OPT_F_TEXT, options.fonts[FONT_TEXT]);
  fonts_setting.SetFont(OPT_F_TABS, options.fonts[FONT_TABS]);
  fonts_setting.SetFont(OPT_F_STATUS_BAR, options.fonts[FONT_STATUS_BAR]);
  fonts_setting.SetFont(OPT_F_FIND_PANEL, options.fonts[FONT_FIND_PANEL]);

  root_setting.SetInt(OPT_I_LINE_PADDING, options.line_padding);

  root_setting.SetBool(OPT_B_SWITCH_CWD, options.switch_cwd);
  root_setting.SetBool(OPT_B_RESTORE_FILES, options.restore_files);
  root_setting.SetBool(OPT_B_SHOW_PATH, options.show_path);

  if (!config.Save(file)) {
    wxLogError(wxT("Failed to save options file: %s"), file);
    return false;
  }

  return true;
}

// NOTE: If a setting is not provided, don't set the related option.
static void ParseEditorOptions(const Setting& setting, editor::Options* options) {
  using namespace editor;

  SettingMap setting_map;
  setting.AsMap(&setting_map);

  //----------------------------------------------------------------------------
  // Text options

  GetInt(setting_map, OPT_I_TAB_STOP, &options->text.tab_stop);
  GetBool(setting_map, OPT_B_EXPAND_TAB, &options->text.expand_tab);
  GetBool(setting_map, OPT_S_DELIMITERS, &options->text.guess_tab);

  GetWString(setting_map, OPT_S_DELIMITERS, &options->text.delimiters);

  // Indent keys
  Setting ik_setting = GetSetting(setting_map, OPT_SA_INDENT_KEYS, Setting::kArray);
  if (ik_setting) {
    options->text.indent_keys.clear();  // Clear global setting.

    for (int i = 0; i < ik_setting.size(); ++i) {
      const char* str = ik_setting[i].GetString();
      // Assume that the string is pure ascii.
      options->text.indent_keys.push_back(std::wstring(str, str + strlen(str)));
    }
  }

  // Extra indent options
  GetOptionTable(setting_map[OPT_G_INDENT], &options->text.indent_options);

  // Comment options
  Setting comment_setting = setting_map[OPT_G_COMMENT];
  if (comment_setting) {
    options->text.comment_add_space = comment_setting.GetBool(OPT_B_ADD_SPACE);
    options->text.comment_respect_indent = comment_setting.GetBool(OPT_B_RESPECT_INDENT);
  }

  //----------------------------------------------------------------------------
  // View options

  GetBool(setting_map, OPT_B_WRAP, &options->view.wrap);
  GetBool(setting_map, OPT_B_SHOW_NUMBER, &options->view.show_number);
  GetBool(setting_map, OPT_B_SHOW_SPACE, &options->view.show_space);
  GetBool(setting_map, OPT_B_SHOW_HSCROLLBAR, &options->view.show_hscrollbar);

  Setting rulers_setting = GetSetting(setting_map, OPT_IA_RULERS, Setting::kArray);
  if (rulers_setting) {
    options->view.rulers.clear();  // Clear global setting.
    for (int i = 0; i < rulers_setting.size(); ++i) {
      options->view.rulers.push_back(rulers_setting[i].GetInt());
    }
  }
}

bool LoadEditorOptionsFile(const wxString& file, editor::Options* options) {
  if (!wxFileName::FileExists(file)) {
    wxLogInfo(wxT("Options file doesn't exist: %s"), file);
    return false;
  }

  Config config;
  if (!config.Load(file)) {
    wxLogError(wxT("Failed to parse options file: %s"), file);
    return false;
  }

  ParseEditorOptions(config.Root(), options);

  return true;
}

static void AddOptionPair(Setting& setting, const editor::OptionPair& option_pair) {
  using namespace editor;

  const char* key = option_pair.key.c_str();
  const OptionValue& value = option_pair.value;
  int type = value.type();

  if (type == OptionValue::kBool) {
    bool b = false;
    if (value.As<bool>(&b)) {
      setting.Add(key, Setting::kBool).SetBool(b);
    }
  } else if (type == OptionValue::kInt) {
    int i = false;
    if (value.As<int>(&i)) {
      setting.Add(key, Setting::kInt).SetInt(i);
    }
  } else if (type == OptionValue::kString) {
    std::string s;
    if (value.As<std::string>(&s)) {
      setting.Add(key, Setting::kString).SetString(s.c_str());
    }
  }
}

bool SaveEditorOptionsFile(const wxString& file, const editor::Options& options) {
  using namespace editor;

  Config config;
  Setting root_setting = config.Root();

  //----------------------------------------------------------------------------
  // Text options

  root_setting.SetInt(OPT_I_TAB_STOP, options.text.tab_stop);
  root_setting.SetBool(OPT_B_EXPAND_TAB, options.text.expand_tab);
  root_setting.SetBool(OPT_S_DELIMITERS, options.text.guess_tab);

  root_setting.SetString(OPT_S_DELIMITERS, wxString(options.text.delimiters).ToAscii().data());

  // Indent keys
  Setting ik_setting = root_setting.Add(OPT_SA_INDENT_KEYS, Setting::kArray);
  for (const std::wstring& indent_key : options.text.indent_keys) {
    ik_setting.Add(NULL, Setting::kString).SetString(wxString(indent_key).ToAscii().data());
  }

  // Extra indent options
  Setting indent_setting = root_setting.Add(OPT_G_INDENT, Setting::kGroup);
  for (const OptionPair& option_pair : options.text.indent_options) {
    AddOptionPair(indent_setting, option_pair);
  }

  // Comment options
  Setting comment_setting = root_setting.Add(OPT_G_COMMENT, Setting::kGroup);
  comment_setting.SetBool(OPT_B_ADD_SPACE, options.text.comment_add_space);
  comment_setting.SetBool(OPT_B_RESPECT_INDENT, options.text.comment_respect_indent);

  //----------------------------------------------------------------------------
  // View options

  root_setting.SetBool(OPT_B_WRAP, options.view.wrap);
  root_setting.SetBool(OPT_B_SHOW_NUMBER, options.view.show_number);
  root_setting.SetBool(OPT_B_SHOW_SPACE, options.view.show_space);
  root_setting.SetBool(OPT_B_SHOW_HSCROLLBAR, options.view.show_hscrollbar);

  // Rulers
  Setting rulers_setting = root_setting.Add(OPT_IA_RULERS, Setting::kArray);
  for (int ruler : options.view.rulers) {
    rulers_setting.Add(NULL, Setting::kInt).SetInt(ruler);
  }

  if (!config.Save(file)) {
    wxLogError(wxT("Failed to save options file: %s"), file);
    return false;
  }

  return true;
}

}  // namespace jil

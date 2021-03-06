#include "app/config.h"
#include "wx/log.h"

namespace jil {

Setting Setting::Get(const char* name, int type, bool recreate) {
  Setting child = Get(name);
  if (child && child.type() != type) {
    if (recreate) {
      Remove(name);
    }
    child.set_ref(NULL);
  }
  if (!child && recreate) {
    child = Add(name, type);
  }
  return child;
}


// For group setting.

int Setting::GetInt(const char* name) const {
  int value = 0;
  config_setting_lookup_int(ref_, name, &value);
  return value;
}

void Setting::SetInt(const char* name, int i) {
  Setting child_setting = Get(name, kInt, true);
  child_setting.SetInt(i);
}

bool Setting::GetBool(const char* name) const {
  int b = 0;
  config_setting_lookup_bool(ref_, name, &b);
  return b != 0;
}

void Setting::SetBool(const char* name, bool b) {
  Setting child_setting = Get(name, kBool, true);
  child_setting.SetBool(b);
}

double Setting::GetFloat(const char* name) const {
  double value = 0.0f;
  config_setting_lookup_float(ref_, name, &value);
  return value;
}

void Setting::SetFloat(const char* name, double f) {
  Setting child_setting = Get(name, kFloat, true);
  child_setting.SetFloat(f);
}

const char* Setting::GetString(const char* name) const {
  const char* str = "";
  config_setting_lookup_string(ref_, name, &str);
  return str;
}

void Setting::SetString(const char* name, const char* str) {
  Setting child_setting = Get(name, kString, true);
  child_setting.SetString(str);
}

wxColour Setting::GetColor(const char* name) const {
  wxColour color;
  color.Set(wxString::FromAscii(GetString(name)));
  return color;
}

bool Setting::GetFont(const char* name, wxString* face_name, int* point_size) const {
  Setting font_setting = Get(name);
  if (font_setting && font_setting.type() == kGroup) {
    *face_name = wxString::FromUTF8(font_setting.GetString("name"));
    *point_size = font_setting.GetInt("size");
    return true;
  }
  return false;
}

void Setting::SetFont(const char* name, const wxFont& font) {
  Setting font_setting = Get(name, kGroup, true);
  font_setting.SetString("name", font.GetFaceName().ToUTF8().data());
  font_setting.SetInt("size", font.GetPointSize());
}

void Setting::AsMap(SettingMap* setting_map) const {
  if (ref_ == NULL) {
    return;
  }

  int size = this->size();

  for (int i = 0; i < size; ++i) {
    Setting setting = Get(i);
    (*setting_map)[setting.name()] = setting;
  }
}

////////////////////////////////////////////////////////////////////////////////

bool Config::Load(const wxString& filename) {
  FILE* config_file = wxFopen(filename, wxT("r"));
  if (config_file == NULL) {
    wxLogInfo(wxT("Failed to open config file to read: %s"), filename.c_str());
    return false;
  }

  int read_code = config_read(&cfg_, config_file);
  fclose(config_file);

  if (read_code == 0) {
    wxLogDebug(wxT("Failed to read config: L%d, %s."),
               config_error_line(&cfg_),
               wxString::FromUTF8(config_error_text(&cfg_)));
    return false;
  }

  return true;
}

bool Config::Save(const wxString& filename) {
  FILE* config_file = wxFopen(filename, wxT("w"));
  if (config_file == NULL) {
    wxLogInfo(wxT("Failed to open config file to write: %s"), filename.c_str());
    return false;
  }

  config_write(&cfg_, config_file);
  fclose(config_file);
  return true;
}

}  // namespace jil

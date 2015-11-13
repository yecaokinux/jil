#ifndef JIL_STATUS_BAR_H_
#define JIL_STATUS_BAR_H_
#pragma once

#include <vector>
#include "wx/panel.h"
#include "editor/theme.h"

namespace jil {

class StatusBar : public wxPanel {
  DECLARE_EVENT_TABLE()

public:
  enum ColorId {
    FG = 0,
    BORDER_OUTER,
    BORDER_INNER,
    BG_TOP,
    BG_BOTTOM,
    COLORS,
  };

  enum FieldId {
    kField_Cwd = 0,
    kField_Encoding,
    kField_FileFormat,
    kField_FileType,
    kField_Caret,
#if JIL_ENABLE_LEADER_KEY
    kField_KeyStroke,
#endif
    kField_Space,

    kField_Count,
  };

  enum SizeType {
    kFit = 0,
    kFixedPixel,
    kFixedPercentage,
    kStretch,
  };

  struct FieldInfo {
    FieldId id;

    wxAlignment align;

    SizeType size_type;

    // For different size types, different meanings of size value:
    // kFit -> padding
    // kFixedPixel -> pixels
    // kFixedPercentage -> % x 100
    // kStretch -> stretch factor
    int size_value;

    // Actual size.
    int size;
  };

public:
  StatusBar();
  virtual ~StatusBar();

  bool Create(wxWindow* parent, wxWindowID id);

  void set_theme(const editor::SharedTheme& theme) {
    theme_ = theme;
  }
   
  virtual bool SetFont(const wxFont& font) override;

  void AddField(FieldId id, wxAlignment align, SizeType size_type, int size_value);

  void SetFields(const std::vector<FieldInfo>& field_infos);

  void UpdateFieldSizes();

  void SetFieldValue(FieldId id, const wxString& value, bool refresh);

  void ClearFieldValues();

protected:
  virtual wxSize DoGetBestSize() const override;

  void OnPaint(wxPaintEvent& evt);

  void OnSize(wxSizeEvent& evt);
  void OnMouseLeftDown(wxMouseEvent& evt);

private:
  void UpdateFontDetermined();

  wxString GetFieldValue(FieldId id);

  // Get field rect according to its size and the client rect.
  // If the field is not found, the rect will be empty.
  wxRect GetFieldRect(FieldId id) const;

  const FieldInfo* GetFieldByPos(int pos_x) const;

  const FieldInfo* GetFieldById(FieldId id) const;

private:
  editor::SharedTheme theme_;

  std::vector<FieldInfo> field_infos_;

  wxString field_values_[kField_Count];

  int char_height_;
  wxSize padding_;
};

////////////////////////////////////////////////////////////////////////////////

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(kEvtStatusFieldClick, 0)
END_DECLARE_EVENT_TYPES()

}  // namespace jil

#define EVT_STATUS_FIELD_CLICK(id, func)\
  DECLARE_EVENT_TABLE_ENTRY(::jil::kEvtStatusFieldClick, id, -1, \
  wxCommandEventHandler(func), (wxObject*)NULL),

#endif  // JIL_STATUS_BAR_H_
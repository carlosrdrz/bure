#pragma once

#include <memory>

#include "renderer.h"
#include "ui_manager.h"
#include "container.h"
#include "label.h"
#include "button.h"

namespace bure {
namespace ui {

class ui_renderer : public renderer {
 public:
  explicit ui_renderer(std::shared_ptr<bure::ui::ui_manager> ui);
  void render(int layer) override;

 private:
  void renderContainer(const ui::container &c);
  void renderLabel(const ui::label &l);
  void renderButton(const ui::button &b);

  bure::color _bgColor, _fontColor;
  std::shared_ptr<bure::ui::ui_manager> _uiManager;
};

}  // namespace ui
}  // namespace bure

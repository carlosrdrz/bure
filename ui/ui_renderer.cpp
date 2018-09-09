#include "ui_renderer.h"

namespace bure {
namespace ui {

ui_renderer::ui_renderer(std::shared_ptr<bure::ui::ui_manager> ui) {
  _uiManager = ui;

  _bgColor.r = 114;
  _bgColor.g = 107;
  _bgColor.b = 82;

  _fontColor.r = 255;
  _fontColor.g = 255;
  _fontColor.b = 255;
}

void ui_renderer::render() {
  for (bure::ui::container& c : _uiManager->getContainers()) {
    renderContainer(c);

    for (bure::ui::label& l : c.getLabels()) {
      renderLabel(l);
    }

    for (bure::ui::button& b : c.getButtons()) {
      renderButton(b);
    }
  }
}

void ui_renderer::renderContainer(const bure::ui::container &c) {
  bure::rect r = {c.getAbsoluteX(), c.getAbsoluteY(), c.getWidth(),
                  c.getHeight()};

  _graphics->drawRect(r, _bgColor);
}

void ui_renderer::renderLabel(const bure::ui::label &l) {
  _graphics->drawText(l.getText(), l.getAbsoluteX(), l.getAbsoluteY(),
                      l.getSize(), _fontColor);
}

void ui_renderer::renderButton(const bure::ui::button &b) {
  color color;
  bure::rect r = {b.getAbsoluteX(), b.getAbsoluteY(), b.getWidth(),
                  b.getHeight()};

  if (b.press == 0)
    color = { 63, 63, 63, SDL_ALPHA_OPAQUE };
  else
    color = { 0, 0, 0, SDL_ALPHA_OPAQUE };

  _graphics->drawRect(r, color);

  if (b.press == 1)
    color = { 63, 63, 63, SDL_ALPHA_OPAQUE };
  else
    color = { 0, 0, 0, SDL_ALPHA_OPAQUE };

  r.height = 1;
  _graphics->drawRect(r, color);

  r.y = b.getAbsoluteY() + b.getHeight();
  _graphics->drawRect(r, color);

  r.width = 1;
  r.height = b.getHeight();
  _graphics->drawRect(r, color);

  r.x = b.getAbsoluteX() + b.getWidth();
  r.y = b.getAbsoluteY();
  _graphics->drawRect(r, color);

  int x = b.getAbsoluteX() + (b.getWidth() / 2);
  int y = b.getAbsoluteY() + (b.getHeight() / 2);
  _graphics->drawTextCentered(b.getTitle(), x, y, 8, _fontColor);
}

}  // namespace ui
}  // namespace bure

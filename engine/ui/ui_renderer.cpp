#include "ui_renderer.h"

#include "utils/logger.h"

namespace bure {
namespace ui {

ui_renderer::ui_renderer(std::shared_ptr<bure::ui::ui_manager> ui) {
  _uiManager = ui;

  _bgColor.r = 114;
  _bgColor.g = 107;
  _bgColor.b = 82;
  _bgColor.alpha = SDL_ALPHA_OPAQUE;

  _fontColor.r = 255;
  _fontColor.g = 255;
  _fontColor.b = 255;
  _bgColor.alpha = SDL_ALPHA_OPAQUE;
}

void ui_renderer::render(int layer) {
  for (bure::ui::container& c : _uiManager->getContainers()) {
    if (c.getLayer() != layer) continue;

    renderContainer(c);

    for (auto& w : c.getChildrenByType(widget_type::label)) {
      auto label = static_cast<bure::ui::label*>(&w.get());
      renderLabel(*label);
    }

    for (auto& w : c.getChildrenByType(widget_type::button)) {
      auto button = static_cast<bure::ui::button*>(&w.get());
      renderButton(*button);
    }
  }
}

void ui_renderer::renderContainer(const bure::ui::container& c) {
  bure::rect src = {104, 0, 1, 26};
  bure::rect dst = {c.getAbsoluteX(), c.getAbsoluteY(), c.getWidth(),
                  c.getHeight()};

  _graphics->drawRect(dst, _bgColor);

  dst.y -= 12;
  dst.x--;
  dst.width = 1;
  dst.height = 26;

  for (int y = 0; y < c.getWidth(); y++) {
    dst.x++;
    _graphics->drawSprite("gui", src, dst);
  }

  dst.y += c.getHeight();
  dst.x = c.getAbsoluteX() - 1;
  for (int y = 0; y < c.getWidth(); y++) {
     dst.x++;
     _graphics->drawSprite("gui", src, dst);
  }

  src.x = 130;
  src.width = 26;
  src.height = 1;

  dst.x = c.getAbsoluteX() - 11;
  dst.y = c.getAbsoluteY();
  dst.height = 1;
  dst.width = 26;

  for (int y = 0; y < c.getHeight(); y++) {
    dst.y++;
    _graphics->drawSprite("gui", src, dst);
  }

  dst.x += c.getWidth() - 4;
  dst.y = c.getAbsoluteY();
  for (int y = 0; y < c.getHeight(); y++) {
    dst.y++;
    _graphics->drawSprite("gui", src, dst);
  }

  src.width = 26;
  src.height = 26;
  dst.width = 26;
  dst.height = 26;

  src.x = 0;
  src.y = 0;
  dst.x = c.getAbsoluteX() + c.getWidth() - 15;
  dst.y = c.getAbsoluteY() - 12;
  _graphics->drawSprite("gui", src, dst);

  src.x += 26;
  dst.x -= c.getWidth() - 4;
  _graphics->drawSprite("gui", src, dst);

  src.x += 26;
  dst.y += c.getHeight();
  dst.x = c.getAbsoluteX() + c.getWidth() - 15;
  _graphics->drawSprite("gui", src, dst);

  src.x += 26;
  dst.x -= c.getWidth() - 4;
  _graphics->drawSprite("gui", src, dst);
}

void ui_renderer::renderLabel(const bure::ui::label& l) {
  _graphics->drawText(l.getText(), l.getAbsoluteX(), l.getAbsoluteY(),
                      l.getSize(), _fontColor);
}

void ui_renderer::renderButton(const bure::ui::button& b) {
  color color;
  bure::rect r = {b.getAbsoluteX(), b.getAbsoluteY(), b.getWidth(),
                  b.getHeight()};

  if (b.press == 0)
    color = {63, 63, 63, SDL_ALPHA_OPAQUE};
  else
    color = {0, 0, 0, SDL_ALPHA_OPAQUE};

  _graphics->drawRect(r, color);

  if (b.press == 1)
    color = {63, 63, 63, SDL_ALPHA_OPAQUE};
  else
    color = {0, 0, 0, SDL_ALPHA_OPAQUE};

  r.height = 1;
  _graphics->drawRect(r, color);

  r.y = b.getAbsoluteY() + b.getHeight();
  _graphics->drawRect(r, color);

  r.y = b.getAbsoluteY();
  r.width = 1;
  r.height = b.getHeight() + 1;
  _graphics->drawRect(r, color);

  r.x = b.getAbsoluteX() + b.getWidth();
  _graphics->drawRect(r, color);

  int x = b.getAbsoluteX() + (b.getWidth() / 2);
  int y = b.getAbsoluteY() + (b.getHeight() / 2);
  _graphics->drawTextCentered(b.getTitle(), x, y, 8, _fontColor);
}

}  // namespace ui
}  // namespace bure

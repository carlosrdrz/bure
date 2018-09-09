#include "renderer.h"

#include "utils/logger.h"

namespace bure {

void renderer::init(std::shared_ptr<bure::graphics2> gr) {
  _graphics = gr;
}

}  // namespace bure

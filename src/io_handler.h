#include "gfx/composer.h"

struct io_handler {
  void run() {
    while (_composer.compose()) {
    }
  }

private:
  gfx::composer _composer{};
};

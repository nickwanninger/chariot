#pragma once

#include "bitmap.h"
#include <ck/string.h>

namespace gfx {

	// load any image style
	ck::ref<gfx::bitmap> load_image(ck::string path);
  // load .bmp files
  ck::ref<gfx::bitmap> load_bmp(ck::string path);

  // load .png files
  ck::ref<gfx::bitmap> load_png(ck::string path);
  ck::ref<gfx::shared_bitmap> load_png_shared(ck::string path);


  ck::ref<gfx::bitmap> load_png_from_res(ck::string res_name);
};  // namespace gfx

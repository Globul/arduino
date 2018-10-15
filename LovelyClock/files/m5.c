/* GIMP RGB C-Source image dump (m5.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[4 * 3 * 3 + 1];
} gimp_image = {
  4, 3, 3,
  "\000\000\000\000\000\000\027\320\377\027\320\377\027\320\377\027\320\377\027\320\377\000\000\000"
  "\027\320\377\000\000\000\000\000\000\000\000\000",
};


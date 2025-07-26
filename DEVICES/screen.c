#include "screen.h"

char buf[BUFFER_MAX_LEN];

void
screen_jump_to_page (char pageName[])
{
  sprintf (buf, "page %s\xff\xff\xff", pageName);
  tjc_send_string (buf);
}

void
screen_print_wave (uint8_t data[], int N)
{
  for (int i = 0; i < N; ++i)
  {
    sprintf (buf, "add s0.id,0,%d\xff\xff\xff", (int)data[i]);
    tjc_send_string (buf);
  }
}

#ifndef _STATUS_BAR_H_
#define _STATUS_BAR_H_

#include <stdint.h>
#include "../init.h"
#include "../../../lab1/rtc.h"

typedef struct {
  const rtc_date *date;
  const rtc_time_t *time;
  uint32_t fps;
} StatusBarInfo;

int ui_status_bar_draw(const Assets *assets, const StatusBarInfo *status);

#endif /* _STATUS_BAR_H_ */

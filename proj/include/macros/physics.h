#ifndef _MACROS_PHYSICS_H_
#define _MACROS_PHYSICS_H_

#define PHYSICS_SCALE 1000
#define SLOWMO_PHYSICS_SCALE 50

#define PHYSICS_FROM_PIXELS(value) ((value) * PHYSICS_SCALE)
#define SLOWMO_PHYSICS_FROM_PIXELS(value) ((value) * SLOWMO_PHYSICS_SCALE)
#define PHYSICS_FROM_THOUSANDTHS(value) (value)
#define PHYSICS_TO_PIXELS(value) ((value) / PHYSICS_SCALE)
#define PHYSICS_VALUE(integer, thousandths) \
  ((integer) * PHYSICS_SCALE + (thousandths))

#endif /* _MACROS_PHYSICS_H_ */

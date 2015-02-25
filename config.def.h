#define VI_KEYS 1

#if VI_KEYS
#  define RAT_LEFT  'h'
#  define RAT_DOWN  'j'
#  define RAT_UP    'k'
#  define RAT_RIGHT 'l'
#else
#  define RAT_LEFT  KEY_LEFT
#  define RAT_DOWN  KEY_DOWN
#  define RAT_UP    KEY_UP
#  define RAT_RIGHT KEY_RIGHT
#endif

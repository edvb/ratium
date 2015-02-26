/* settings for ratium */

/* quick settings */
#define RAT_KEYS 1 /* arrow keys = 0, vi keys = 1, num pad = 2*/


#if RAT_KEYS == 0
#  define RAT_LEFT  KEY_LEFT
#  define RAT_DOWN  KEY_DOWN
#  define RAT_UP    KEY_UP
#  define RAT_RIGHT KEY_RIGHT
#  define RAT_STAND '.'
#elif RAT_KEYS == 1
#  define RAT_LEFT  'h'
#  define RAT_DOWN  'j'
#  define RAT_UP    'k'
#  define RAT_RIGHT 'l'
#  define RAT_STAND '.'
#elif RAT_KEYS == 2
#  define RAT_LEFT  '4'
#  define RAT_DOWN  '2'
#  define RAT_UP    '8'
#  define RAT_RIGHT '6'
#  define RAT_STAND '5'
#endif

#define RAT_GET  'g'
#define RAT_INV  'i'
#define RAT_DROP 'd'
#define RAT_OPEN 'o'

#include <strukturen.h>
typedef void (*Key_Event_fp_t)(int key, t_bool down);

void KBD_close( void );
void KBD_update( void );
void KBD_init( Key_Event_fp_t fp);

typedef struct in_state {
	// Pointers to functions back in client, set by vid_so
	void (*IN_CenterView_fp)(void);
	Key_Event_fp_t Key_Event_fp;
	//t_vec *viewangles;
	int *in_strafe_state;
} t_in_state;



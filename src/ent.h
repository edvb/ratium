#ifndef ENT_H
#define ENT_H

bool  can_step(Ent *e, Pos pos);
void  move_entity(Ent *e, float x_0, float y_0);
void  attack(Ent *e, Ent *foe);
int   deal_damage(Ent *e);
void  take_damage(Ent *e, int damge);
bool  isalive(int hp);
float holding_x(Ent e, float val);
float holding_y(Ent e, float val);

#endif /* ENT_H */

#ifndef ENT_H
#define ENT_H

bool can_step(Ent *e, int x, int y);
void move_entity(Ent *e, int x_0, int y_0);
void attack(Ent *e, Ent *foe);
int  deal_damage(Ent *e);
void take_damage(Ent *e, int damge);
bool isalive(int hp);
int  holding_x(Ent e, int val);
int  holding_y(Ent e, int val);

#endif /* ENT_H */

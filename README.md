## Algorithm:
esp will look around to see if there is any fire

if not : do nothin

if yes : track the fire and tell arduino to go left/ right according to the position of fire in camera screen


while marching towards, arduino will avoid obstacle if faced

after getting close enough, flame sensor will detect fire and than robot will stop and turn pump on

after fire extinguished, esp will go back to its initial state.


if esp gets info from laptop that 'other room' in on fire, it will look for a door.

after finding , it will again direct arduino as it did before.

after crossing the door, it will look for next closest door and approach as before. 

after entering the room, it will look around (like initially) for fire. 

upon finding it will do the same as it did.

### Remark:
- Each feature were implemented in different directory

- All arduino uno code combined in <code>master_uno2</code>

- All esp32-cam code combined in <code>master_esp2</code>

- The code will hopefully  be rearranged nicely after Term Final :3
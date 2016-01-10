#9000
pneumatic thermo-grenade~
1 c 1
activate~
if (%actor.vnum% == -1) then
  if (%arg% != sphere) then
    osend %actor% Activate what?
  else
    wait 2s
    osend %actor% Sphere activated!
  end
end
~
#9001
Sleeping unholy avenger equip~
1 j 100
~
wait 2s
osend %actor% An uneasy feeling washes over you as your hand closes around the rusted hilt of the sword.
wait 2s
osend %actor% A sudden, massive jolt of pain shoots up your arm and into your chest! You feel weakened as the sword saps a large portion of your vitality!
oechoaround %actor% %actor.name% visibly tenses for several moments and nearly collapses after taking hold of their sword.
~
$~

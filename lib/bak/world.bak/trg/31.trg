#3100
m3115 border hedge patrol~
2 g 100
~
if (%actor.vnum% == -1) then
wait 5
eval pcrace %actor.race%
if (%pcrace% == Silvanesti) then
wsend %actor% Aspen leaves rain down from the treetops, as if to welcome you.
elseif (%pcrace% == Qualinesti) then
wsend %actor% The trees seem aware of your presence and part slightly to allow your passage.
elseif (%pcrace% == Kagonesti) then
wsend %actor% You sense that the forest itself is quite aware of your presence.
else
wsend %actor% A warning arrow flies by your ear.
wechoaround %actor% A warning arrow flies through the air.
wait 1s
wsend %actor% Another arrow is heading in your direction. This one appears to be more than just a warning.
wechoaround %actor% Another arrow flies through the air. This one appears to be more than just a warning.
wdamage %actor% 5
wait 6
wsend %actor% You are hit by the arrow!
wechoaround %actor% %actor.alias% is hit by the arrow!
end
end
~
#3102
r3341 & r3159 load mob & fight trigger~
2 g 75
~
if (%actor.vnum% == -1)
wait 1s
wecho A nasty-looking badger streaks out of the underbrush.
wload mob 3104
wait 2s
wecho A pack of coshee burst from the underbrush in pursuit of the badger.
wload mob 3102
wait 1s
wforce pack kill badger
end
~
#3147
r3147 down exit creation~
2 c 100
move~
if (%actor.vnum% == -1) then
   if (%arg% != debris) then
   wsend %actor% You do not see that here.
   else
   wait 2s
   wechoaround %actor% %actor.name% clears the debris revealing a circle of runes surrounding a narrow opening in the ground.
   wsend %actor% You clear the debris revealing a circle of runes surrounding a narrow opening in the ground.
wdoor 3147 down flags a
   wdoor 3147 down name pit
   wdoor 3147 down room 3352
   wait 30s
   wecho As if moved by some magical force, the debris moves to cover a small opening in the ground.
   wdoor 3147 down purge
   end
end
~
$~

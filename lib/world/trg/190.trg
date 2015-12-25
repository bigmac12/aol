#19008
Sheep~
2 dg 0
Sheep~
if (%actor.vnum% == -1) then
wait 1s
wsend %actor% A rubber sheep appears before your eyes!
wait 2s
wsend %actor% The inflatible sheep bleets,'I've been a baaaaad boy.'
wechoaround %actor% %actor.name% is staring at the wall, making bleeting sounds.
wait 3s
wsend %actor% The inflatible sheep turns around and flys away, its feet running in the air.
wechoaround %actor% %actor.name% Stares out the window, waving to something on the outside.
wait 1s
wechoaround %actor% %actor.name% blinks and seems to return to normal.
~
#19025
m19025 Geeves' Misery~
0 g 100
~
if (%actor.vnum%) == -1
wait 2s
emote looks up at you pitifully from his work.
wait 3s
emote regards you with cold eyes that are deep with endless sorrow.
wait 4s
say Kill a few children, and this is what they do to you...
wait 3s
say ...endless torment as a common butler.
wait 4s
emote sees that you are not here to end his suffering, so he resumes slicing carrots.
wait 2s
emote is the very picture of misery.
~
#19080
Avanhome~
2 d 100
Sla'Mori~
if (%actor.vnum% == -1)
wait 1s
wecho The sounds of the forest swells momentarily just before all noise falls away from your ears.
wait 2s
wecho A soft glow begins to fill the glade with gentle, pure light.
wdoor 19080 down room 19081 flags a
wait 1s
wecho The growing light flashes blindingly for the briefest of moments.
wait 2s
wecho When vision and sound returns, you find yourself within a slender tower of rose quartz.
wforce all down
wait 1s
wdoor 19080 down purge
~
#19081
Avanout~
2 d 100
N'Sla'Mori~
if (%actor.vnum% == -1)
wait 1s
wecho The usual sounds of the tower swell momentarily before all noise falls away from your ears.
wait 2s
wecho A soft glow begins to pulse forth from the moonstone orb.
wdoor 19081 down room 19080 flags a
wait 1s
wecho The growing light flashes blindingly for the briefest of moments.
wait 2s
wecho When vision and sound returns, you find yourself in a small, beautiful glade.
wforce all down
wait 1s
wdoor 19081 down purge
~
$~

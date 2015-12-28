#11600
r11627 exit down to 11628~
2 c 100
examine~
if (%actor.vnum% == -1) then
if ("%arg%" == "boulder")
wait 1s
wdoor 11627 down room 11628 flags a
wait 1s
wechoaround %actor% %actor.name% has dislodged the boulder causing it to topple over.
wsend %actor% You have knocked the boulder loose!
wait 4s
wforce all down
wait 10s
wdoor 11627 down purge
end
end
~
#11629
current nw~
2 g 90
~
if (%actor.vnum% == -1) then
wait 1s
wecho You are swept up in an odd current and dragged along the bottom of the lake.
wait 1s
wforce all nw
end
~
#11630
current ne~
2 g 90
~
if (%actor.vnum% == -1) then
wait 1s
wecho You are swept up in an odd current and dragged along the bottom of the lake.
wait 1s
wforce all ne
end
~
#11693
r11693 exit down to 5259~
2 c 100
examine~
if (%actor.vnum% == -1) then
if ("%arg%" == "eyesocket")
wait 1s
wdoor 11693 down room 5259 flags a
wait 1s
wechoaround %actor% %actor.name% has placed a shard of onyx into the empty eyesocket.
wsend %actor% You hear a loud click as the shard slips into place.
end
end
~
$~

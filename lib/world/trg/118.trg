#11800
r11862 down exit~
2 c 100
pray~
if (%actor.vnum% == -1) then
wait 1s
wsend %actor% You begin to pray.
wechoaround %actor% %actor.name% begins to pray.
wait 1s
wecho A soft blue light fills the chamber.
wait 2s
wecho As the light fades and your eyes readjust, you notice a stairway leading down.
wdoor 11862 down room 11898 flags a
wait 20s
wecho A soft blue light once again illuminates the temple.
wdoor 11862 down purge
end
~
$~

#4300
r4336 search to find down to r4396~
2 c 1
search~
If (%actor.vnum% == -1) then
wait 1s
wsend %actor% You search the ground and notice that the soil seems to have been recently disturbed.
wechoaround %actor% %actor.name% searches the ground.
wait 2s
wsend %actor% You begin the arduous task of digging through the soft and shifting soil.
wechoaround %actor% %actor.name% slowly starts to dig a hole in the ground here.
wait 4s
wsend %actor% You have found a passage leading down here!
wechoaround %actor% %actor.name% has found a passage leading down!
wdoor 4336 down room 4396 flags a
wait 15s
wecho Some soil slowly slides down filling a hole in the ground here.
wait 3s
wdoor 4336 down purge
end
~
$~

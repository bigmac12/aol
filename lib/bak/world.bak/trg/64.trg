#6400
r6468 dig down to 6471~
2 c 100
dig~
If (%actor.vnum% == -1) then
wait 3s
wsend %actor% You pick up one of the nearby shovels and start to dig through the soft soil.
wechoaround %actor% %actor.name% picks up a shovel and starts to dig.
wait 4s
wsend %actor% Paydirt!  You have dug a hole, large enough to crawl down.
wechoaround %actor% %actor.name% has dug a hole large enough to crawl down!
wdoor 6468 down room 6471 flags a
wait 30s
wecho Some soil slowly slides down, filling a hole in the ground.
wait 3s
wdoor 6468 down purge
end
~
#6401
m6499 Aggro on Sarcophagus~
0 c 100
open~
if (%actor.vnum% == -1) then
if ("%arg%" == "sarcophagus")
wait 1
msend %actor% As you push the lid on the sarcophagus, something swoops in from the shadows!
mechoaround %actor% Something stirs within the crypt, lunging at %actor.name%!
wait 1
mkill %actor%
end
end
~
#6413
r6413 Statue Pull Trigger~
2 c 100
pull~
if (%actor.vnum% == -1) then
if (%arg% != statue) then
wsend %actor% You do not see that here.
else
wait 2s
wechoaround %actor% %actor.name% slowly pulls the statue away from the wall, revealing a hidden passageway!
wsend %actor% You pull the statue away from the wall, discovering a hidden passageway!
wdoor 6413 south flags abcd
wdoor 6413 south key 6427
wdoor 6413 south name door
wdoor 6413 south room 6460
wait 30s
wecho Slowly the statue slides back into place, covering the door and leaking water all over the alcove.
wdoor 6413 south purge
end
end
~
#6465
r6465-6470 Vampires Load~
2 b 100
~
wait until 2200
wecho &mTwilight settles into the clearing, which grows very, very dark.&n
wait 5s
wecho Several sleek forms creep out of the darkness.
wload mob 6497
wload mob 6498
halt
~
#6466
r6465-6470 Vampires Fade~
2 b 100
~
wait until 0400
wecho As the sun begins to creep over the horizon, the restless dead vanish into thin air!
wpurge
halt
~
$~

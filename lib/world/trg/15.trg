#1500
library man~
0 g 100
~
if (%actor.vnum% == -1) then
wait 2
say Greetings! Welcome to the Palanthas' Library.
say Please remember that all books herein are the property of the Palanthas' Library.
say Feel free to review them as much as you want to here, but please do not take them outside of the library so that others can find and read them.
~
#1501
master scribe greet~
0 g 90
~
if (%actor.vnum% == -1) then
wait 10
emote looks up apparently shocked that you are here.
say Halt, noone is allowed to be back here!
say You will have to leave immediately.
~
#1502
renegade refusal trigger~
0 g 100
~
if (%actor.vnum% == -1) then
wait 3
say 'NO! You can not make go and take the Test!!
say 'I will die before i take the Test!!
~
#1503
library guards (bow)~
0 g 500
~
wait 3
bow
wait 1s
say Welcome to the great library of Palanthas.
wait 4
open door north
~
#1517
test for commands~
2 c 100
paladine~
wecho Paladine, god of all good, row 5 section c, shelf 3.
~
#1530
secret room (dust)~
2 g 100
(null)~
wait 9
wecho  Dust flies forth from the objects among the room once the panel has been opened.
wait 9
wait 9
wait 9
~
$~

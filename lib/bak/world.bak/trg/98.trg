#9800
(%has.x%)Test~
2 c 100
fish~
   if (%actor.vnum% == -1) && (%has.rod%)
      if (%arg != rod)
         wsend %actor% Nope
      else
            wsend %actor% has.x variable is working
   else
   wsend %actor% has.x variable is working
end
~
#9828
m9828 Inv Purge~
0 e 100
sells~
say Please excuse me a moment.
wait 4
emote carries his new item upstairs to add to his collection.
up
drop all
down
wait 6
~
#9899
r9872 Wave~
2 h 100
~
wait 1
wecho  A wave appears, washing debris out to sea.
~
$~

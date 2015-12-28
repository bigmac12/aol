#12158
m12158 Inv Purge~
0 e 100
sells~
   say Please excuse me a moment.
wait 4
emote carries her new item upstairs to add to her collection.
up
drop all
down
wait 6
~
#12169
ogre skull get trigger~
1 g 100
~
wait 2s
osend %actor% The skull is cool to the touch, but it seems to glimmer with an inner light.
wait 6s
osend %actor% Your hands tingle slightly wherever they come into contact with the skull.
wait 6s
osend %actor% It seems as if the skull is staring at you, delving into your mind.
wait 6s
osend %actor% It is not an uncomfortable feeling though, and indeed you feel somehow calmed.
eval pwearer %actor.alias%
global pwearer
~
#12170
skull wear trigger obj 12169~
1 j 100
~
wait 2s
osend %actor% The skull begins to float in lazy circles around you.
oechoaround %actor.alias% The skull begins to float in lazy circles around %actor.alias%.
eval pwearer %actor.alias%
global pwearer
~
#12171
ogre skull drop safety obj 12169~
1 h 100
~
eval pwearer 0
global pwearer
~
$~

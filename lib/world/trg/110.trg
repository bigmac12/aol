#11000
m11006 Mage Pass~
0 e 0
pair of disembodied eyes steps in front~
wait 2
if (%actor.vnum% == -1)
if ((%actor.class% == mage) & (%actor.align% < 1))
msend %actor% A pair of disembodied eyes pauses a moment, examining you with great interest.
mechoaround %actor% A pair of disembodied eyes pauses a moment, examining %actor.name% with great interest.
wait 4
msend %actor% You are magically transported to the interior of the tower.
mechoaround %actor% %actor.name% disappears suddenly.
mteleport %actor% 11005
wait 4
mforce %actor% look
~
#11001
r11006 tele to r6217~
2 d 100
Aeleck~
if (%actor.vnum% == -1)
if (%actor.class% == mage)
wait 1s
wechoaround %actor% The circle on the floor glows brightly enveloping %actor.name% in its light.
wsend %actor% The circle on the floor glows brightly enveloping you in its light.
wait 1s
wechoaround %actor% The light fades and %actor.name% is gone.
wteleport %actor% 6217
wsend %actor%  The light blurs your vision.  When it fades you feel momentarily disoriented and need a moment to adjust to your surroundings.
wait 1s
wforce %actor% look
wechoaround %actor% Light rises up from powdery circle on the floor, illuminating the area.
wait 1s
wechoaround %actor% When the light fades, %actor.name% is standing in the center of the circle.
else
wait 1s
wecho The words echo hollowly.
end
end
end
~
#11002
r6217 tele to r11002~
2 d 100
Horistal~
if (%actor.vnum% == -1)
if (%actor.class% == mage)
wait 1s
wechoaround %actor% The circle on the floor glows brightly enveloping %actor.name% in its light.
wsend %actor% The circle on the floor glows brightly enveloping you in its light.
wait 1s
wechoaround %actor% The light fades and %actor.name% is gone.
wteleport %actor% 11006
wsend %actor%  The light blurs your vision.  When it fades you feel momentarily disoriented and need a moment to adjust to your surroundings.
wait 1s
wforce %actor% look
wechoaround %actor% Light rises up from powdery circle on the floor, illuminating the area.
wait 1s
wechoaround %actor% When the light fades, %actor.name% is standing in the center of the circle.
else
wait 1s
wecho The words echo hollowly.
end
end
end
~
#11050
m11050 greet when academy entrance~
0 g 100
~
if (%direction% == west)
  if (%random.3% == 1)
wait 1s
    emote looks up from his work enough to give you a short nod.
wait 1s
    say Welcome to Delusen's Academy.  Feel free to tour it, if it pleases you.
wait 2s
    emote motions toward the southern door.
  end
end
~
#11051
Random trigger for kitchen food production~
0 b 18
~
if (%foodamt% > 0)
eval foodamt (%foodamt% - 1)
global foodamt
end
~
#11052
Bracelet drop (teleport)~
1 h 100
~
if !(%telep% == 0)
wait 3
oechoaround %actor% A silver bracelet marked with tiny runes along the center explodes with light!  
osend %actor% You feel a sudden rush as you are pushed through the folds of reality!
oteleport %actor% %telep%
oechoaround %actor% A stunning flash of light engulfs the area, and from it, a figure emerges!
oforce %actor% look
wait 2s
oload mob 11099
oteleport soulless %actor.room%
osend %actor% A soulless being steps from the light after you!
oechoaround %actor% A soulless being forms out of the light and steps from it!
end
~
#11053
Instructor's narcolepsy trig~
0 h 100
~
eval split %random.3%
if (%split% == 1)
wait 1s
say As we learn more and more about the nature of magics...
wait 6s
say Magics...  magics old and new...
wait 5s
emote blinks and rubs his eyes.
wait 10s
say What was I saying?...  Oh...  yes.  The nature of...
wait 2s
emote pauses mid-sentence.
wait 7s
say Er.. Oh dear.
wait 3s
emote sighs.
elseif (%split% == 2)
wait 1s
say I have a theory about the nature of abjuration.
wait 6s
say The theory states that while the factors surrounding abjuration, such as current environment states and such...
wait 3s
emote blinks.
wait 5s
say ...are generally always the same...  the, uh... the...
wait 11s
em sighs to himself quietly.
wait 2s
say Let's just resume the lecture tomorrow, shall we?
wait 3s
emote smiles faintly.
else
wait 1s
say If you understand the basic principle behind it all ...
wait 5s
say ...  then ...  you can ...
wait 10s
emote slumps over onto the desk and falls fast asleep.
wait 5s
snore
end
~
#11054
Bracelet wear (set location)~
1 j 100
~
set telep %actor.room%
global telep
wait 1s
osend %actor% The bracelet begins to hum faintly, clinging to your skin.
oechoaround %actor% A silver bracelet marked with tiny runes along the center begins to hum faintly.
~
#11055
m11062 cook greet~
0 g 100
~
wait 1s
if (%foodamt% > 2)
say Welcome to the kitchen.  At the moment, we're cooking for the next meal, but it should be up soon.
else
say Welcome to the kitchen.  If you're hungry, just say so, and I'm sure we can find something for you to eat.
end  
~
#11056
m11062 cook dish out food~
0 d 1
hungry meal food eat breakfast lunch dinner tray~
wait 1s
if (%foodamt% > 2)
say I'm sorry.  We've served all the food we have for now.
halt
end
eval dummy %random.4%
if (%dummy% == 1)
say Ah, you're in luck -- I have some fabulous stew for you.
mload obj 11097
eval foodamt (%foodamt% + 1)
global foodamt
wait 2s
give stew %actor.alias%
wait 1s
say There you go.  Enjoy!
wait 2s
em smiles warmly and turns to the next in line.
elseif (%dummy% == 2)
say You're just in time to try my latest creation -- the perfect sandwich.
mload obj 11096
mload obj 11098
eval foodamt (%foodamt% + 1)
global foodamt
wait 2s
put sandwich tray
wait 1s
give tray %actor.alias%
wait 1s
say There you are -- enjoy.
elseif (%dummy% == 3)
say I have some veal and potatoes for you.  You simply have to try this.
mload obj 11096
mload obj 11099
eval foodamt (%foodamt% + 1)
global foodamt
wait 2s
put veal tray
wait 1s
give tray %actor.alias%
wait 2s
say I hope you enjoy your meal.
wait 1s
emote smiles and returns to work.
else
say I'm sorry.  We've run out of food for now.  Please wait a few hours and I'm sure we'll have something ready.
wait 1s
eval foodamt 3
global foodamt
end
~
#11057
o11075 drop trigger safety~
1 h 100
~
eval pwearer 0
global pwearer
~
#11058
o11074 dice roll, defending~
1 c 111
rolldefend~
wait 1
osend %actor% You throw the dice for the defending roll.
oechoaround %actor% %actor.alias% throws the dice for the defending roll.
eval die1 %random.6%
eval die2 %random.6%
eval die3 %random.6%
if (%die1% == 1)
oecho The first die is:   Ghura  (Low)
elseif (%die1% == 2)
oecho The first die is:   Reja   (Low)
elseif (%die1% == 3)
oecho The first die is:   Marixa (Low)
elseif (%die1% == 4)
oecho The first die is:   Uia   (High)
elseif (%die1% == 5)
oecho The first die is:   Kav   (High)
elseif (%die1% == 6)
oecho The first die is:   Sen  (Ender)
end
if (%die2% == 1)
oecho The second die is:  Ghura  (Low)
elseif (%die2% == 2)
oecho The second die is:  Reja   (Low)
elseif (%die2% == 3)
oecho The second die is:  Marixa (Low)
elseif (%die2% == 4)
oecho The second die is:  Uia   (High)
elseif (%die2% == 5)
oecho The second die is:  Kav   (High)
elseif (%die2% == 6)
oecho The second die is:  Sen  (Ender)
end
if (%die3% == 1)
oecho The third die is:   Ghura  (Low)
elseif (%die3% == 2)
oecho The third die is:   Reja   (Low)
elseif (%die3% == 3)
oecho The third die is:   Marixa (Low)
elseif (%die3% == 4)
oecho The third die is:   Uia   (High)
elseif (%die3% == 5)
oecho The third die is:   Kav   (High)
elseif (%die3% == 6)
oecho The third die is:   Sen  (Ender)
end
~
#11059
o11074 dice trigger, attacking~
1 c 111
rollattack~
wait 1
osend %actor% You throw the dice for the attacking roll.
oechoaround %actor% %actor.alias% throws the dice for the attacking roll.
eval die1 %random.6%
eval die2 %random.6%
if (%die1% == 1)
oecho The first die is:   Ghura  (Low)
elseif (%die1% == 2)
oecho The first die is:   Reja   (Low)
elseif (%die1% == 3)
oecho The first die is:   Marixa (Low)
elseif (%die1% == 4)
oecho The first die is:   Uia   (High)
elseif (%die1% == 5)
oecho The first die is:   Kav   (High)
elseif (%die1% == 6)
oecho The first die is:   Sen  (Ender)
end
if (%die2% == 1)
oecho The second die is:  Ghura  (Low)
elseif (%die2% == 2)
oecho The second die is:  Reja   (Low)
elseif (%die2% == 3)
oecho The second die is:  Marixa (Low)
elseif (%die2% == 4)
oecho The second die is:  Uia   (High)
elseif (%die2% == 5)
oecho The second die is:  Kav   (High)
elseif (%die2% == 6)
oecho The second die is:  Sen  (Ender)
end
~
#11060
o11075 cursed wear trigger~
1 j 100
~
wait 5s
osend %actor% You notice the pendant seeming to weigh a bit heavily on you.
wait 40s
osend %actor% The silver pendant suddenly coils itself snugly about your neck.
wait 3s
osend %actor% It is not an uncomfortable feeling, however.
wait 2s
osend %actor% Warmth floods your body and your eyes start to droop.
oechoaround %actor% %actor.alias% begins to look droopy-eyed.
wait 18s
osend %actor% You begin to feel an urge to lie down...
wait 4s
oforce %actor% rest
wait 3s
osend %actor% You close your eyes for a moment, and when you open them again...
wait 2s
osend %actor% ...the world you once knew has changed somehow, irreversibly.
oechoaround %actor% %actor.alias%'s eyes suddenly glaze over with a strange stupor.
wait 4s
osend %actor% You feel that the laws of the universe no longer apply to you.
eval pwearer %actor.alias%
global pwearer
~
#11061
o11075 madness trigger, baby!  It's all about 20% random~
1 b 45
~
if !(%pwearer% == 0)
eval dummy %random.55%
if (%dummy% == 1)
oechoaround %pwearer% %pwearer% scowls at you, then looks away.
elseif (%dummy% == 1)
osend %pwearer% Your feet slowly lift off the ground, and you feel as though you are able to fly.
elseif (%dummy% == 2)
osend %pwearer% Suddenly a massive shadow jumps out and engulfs you!  You take off in full run!
oforce %pwearer% flee
oforce %pwearer% scream
oforce %pwearer% flee
oforce %pwearer% flee
oforce %pwearer% north
oforce %pwearer% west
oforce %pwearer% south
oforce %pwearer% west
oforce %pwearer% east
oforce %pwearer% scream
elseif (%dummy% == 3)
oechoaround %pwearer% %pwearer% screams loudly, and then slowly begins to weep.
osend %pwearer% A deep sadness overwhelms you, and you begin to cry.
elseif (%dummy% == 4)
oforce %pwearer% dance
elseif (%dummy% == 5)
oforce %pwearer% wig
elseif (%dummy% == 6)
oforce %pwearer% flee
elseif (%dummy% == 7)
oforce %pwearer% scream
elseif (%dummy% == 8)
oforce %pwearer% peer
elseif (%dummy% == 9)
oforce %pwearer% look
elseif (%dummy% == 10)
osend %pwearer% A loud explosion suddenly goes off in the distance.
elseif (%dummy% == 11)
osend %pwearer% a short, balding man has arrived from the south.
elseif (%dummy% == 12)
osend %pwearer% a tall elven man with twinkling eyes leaves west.
elseif (%dummy% == 13)
osend %pwearer% a beautiful young woman with flowing blonde hair has arrived from the east.
elseif (%dummy% == 14)
osend %pwearer% a courteous old gentleman with greying hair has arrived from the north.
elseif (%dummy% == 15)
osend %pwearer% Fertald introduces himself to you.
elseif (%dummy% == 16)
osend %pwearer% Argas introduces himself to you.
elseif (%dummy% == 71)
oechoaround %pwearer% %pwearer% spits at you, snarling.
elseif (%dummy% == 18)
oechoaround %pwearer% %pwearer% huddles up in a ball.
osend %pwearer% You suddenly feel the need to curl up into a ball.
elseif (%dummy% == 19)
oechoaround %pwearer% %pwearer% whispers to you, 'Please help me'
osend %pwearer% You begin whispering incoherently.
elseif (%dummy% == 20)
oechoaround %pwearer% %pwearer% whispers to you, 'I am not a man'
osend %pwearer% You begin whispering incoherently.
elseif (%dummy% == 21)
oechoaround %pwearer% %pwearer% whispers to you, 'Food, I must have food...'
osend %pwearer% You begin whispering incoherently.
elseif (%dummy% == 21)
oechoaround %pwearer% %pwearer% whispers to you, 'Do you have something to drink?'
osend %pwearer% You begin whispering incoherently.
elseif (%dummy% == 22)
osend %pwearer% You are very hungry.
elseif (%dummy% == 23)
osend %pwearer% You are very thirsty.
elseif (%dummy% == 24)
osend %pwearer% You feel light-headed.
elseif (%dummy% == 25)
osend %pwearer% The sky flashes a pale green and mocks you in its florescent hue.
oechoaround %pwearer% %pwearer% stares at the sky in horror.
elseif (%dummy% == 26)
osend %pwearer% You begin to think of your home.
elseif (%dummy% == 27)
osend %pwearer% The sun rises in the west.
elseif (%dummy% == 28)
osend %pwearer% The sun sets in the east.
elseif (%dummy% == 29)
osend %pwearer% a large, burly man of massive girth attacks a cheerful kender with a brown topknot!
elseif (%dummy% == 30)
osend %pwearer% Suddenly you are covered in water.
elseif (%dummy% == 31)
oechoaround %pwearer% %pwearer% slowly begins to strip off pieces of clothing.
osend %pwearer% You have a sudden urge to remove your clothing.
elseif (%dummy% == 32)
oechoaround %pwearer% %pwearer% kisses you.
elseif (%dummy% == 33)
oechoaround %pwearer% %pwearer% hugs you.
elseif (%dummy% == 34)
oechoaround %pwearer% %pwearer% embraces you warmly.
elseif (%dummy% == 35)
oechoaround %pwearer% %pwearer% laughs at you.
elseif (%dummy% == 36)
osend %pwearer% Everyone around you seems to stare down at you.
elseif (%dummy% == 37)
oechoaround %pwearer% %pwearer% nearly falls over.
osend %pwearer% Your head begins spinning, and you nearly fall unconscious.
elseif (%dummy% == 38)
osend %pwearer% You have a sudden urge to sleep...
oforce %pwearer% sleep
elseif (%dummy% == 39)
oforce %pwearer% north
elseif (%dummy% == 40)
oforce %pwearer% south
elseif (%dummy% == 41)
oforce %pwearer% lick
elseif (%dummy% == 42)
osend %pwearer% The area suddenly erupts in a beautiful rainbow of colors.
elseif (%dummy% == 43)
osend %pwearer% Music drifts in on the wind and fills your head with song.
elseif (%dummy% == 44)
osend %pwearer% Small birds flutter by in a beautiful mesh of colors and beating wings.
elseif (%dummy% == 45)
osend %pwearer% Tiny flames begin to dance in your vision, lighting everything around you.
elseif (%dummy% == 46)
osend %pwearer% The ground starts breathing, tilting up and down in time with your breaths.
elseif (%dummy% == 47)
osend %pwearer% All of a sudden, darkness surrounds you except for a bright, blinding light.
elseif (%dummy% == 48)
osend %pwearer% Someone whispers to you, 'I know who you really are!'
wait 2s
oforce %pwearer% shiver
elseif (%dummy% == 49)
osend %pwearer% A chill passes over your body, making you feel ill.
wait 2s
oforce %pwearer% puke
elseif (%dummy% == 50)
osend %pwearer% Images of death suddenly surround you, turning all into skeletal forms.
elseif (%dummy% == 51)
osend %pwearer% All around you, everything distorts horribly, reality moving like water.
elseif (%dummy% == 52)
osend %pwearer% You have a splitting headache.
elseif (%dummy% == 53)
osend %pwearer% You are having a very difficult time breathing.
oechoaround %pwearer% %pwearer% gasps and chokes for a breath.
elseif (%dummy% == 54)
oechoaround %pwearer% %pwearer% looks fearful of all in the area.
osend %pwearer% You look around nervously, deathly scared of all around you.
elseif (%dummy% == 55)
osend %pwearer% As you close your eyes, you develop an interesting spinning sensation.
end
end
~
#11062
o11075 don't give away~
1 i 100
~
osend %actor% You just can't seem to convince yourself to give such a thing away...
return 0
~
$~

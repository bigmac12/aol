#11704
trolls arm~
0 l 30
actor~
msend %actor% A troll's arm flings to the ground and attacks you!
wait 1
mload mob 11719
~
#11708
Zanthar~
0 h 95
none~
if (%actor.vnum% == -1)
wait 1s
say Greetings. I am Zanthar, High Priest of the Phoenix, and its sole protector.
wait 3s
say I have guarded the sacred chamber underneath this church for nearly thirty years now, and soon it shall take its place as our protector.
wait 2s
emote smiles contentedly, a certain light in his eyes.
~
#11715
m11715 Kaaba Greet~
0 h 95
actor~
if (%actor.vnum% == -1)
wait 1s
say Welcome to my humble town.
wait 2s
say I am Lord Kaaba, Mayor of this town.
wait 3s
say You have come at a time when heroes are needed.
wait 1s
emote hesitates before he continues.
wait 3s
say I have a suspicion that the Royal Wizard has been summoning beasts into this town.  We have the Great Phoenix to protect us, but alas he is still young!
wait 2s
sigh
wait 3s
emote looks squarely at you.
wait 1s
say Will you be our hero in our hour of need?
~
#11716
m11715 Kaaba Yes/No~
0 d 100
yes no~
if (%speech% /= Yes) then
wait 2s
emote smiles fondly, a tear nearly coming to his eye.
wait 3s
say You are... truly kind, adventurer.  Please, take this, and search the town for Tethathas.
wait 3s
drop hand
wait 2s
say That will unlock the statue in town, under which Tethathas is performing his evil.
wait 4s
say Be careful, for he has a creature that guards him!
wait 3s
emote smiles once more, patting you on the shoulder.
wait 2s
say May the gods speed you back...
elseif (%speech% /= No) then
wait 2s
emote hangs his head sadly.
wait 3s
say Very well... it is not your fight.
wait 3s
emote turns sadly back to his desk.
wait 4s
say We will keep fighting, and do as we must.
wait 3s
say We will wait for our heroes to present themselves.
end
~
#11726
m11726 load o11738 On Death~
0 f 100
~
if (%random.15% == 1)
mload obj 11738
end
~
#11732
m11732 load o11744 On Death~
0 f 100
~
if (%random.20% == 1)
mload obj 11744
end
~
$~

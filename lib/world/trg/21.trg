#2100
Say 'earakar' to open door~
2 d 0
earakar~
if (%actor.vnum% == -1) then
wait 3
wsend %actor% Machinery begins to whir as you speak the word.
wechoaround %actor% Machinery begins to whir as %actor.name% speaks the word.
wait 2 s
wecho The secret door grinds open.
wasound You hear the sound of grinding stone in the distance.
wdoor 2107 east flags a
wdoor 2108 west flags a
wait 15 s
wecho The wall begins to slide shut with slow grind.
wait 5 s
wecho Quietly the wall locks into place.
wdoor 2107 east flags abcd
wdoor 2108 west flags abcd
~
#2101
Guard's greet~
0 g 100
~
if (%direction% == east) and
   (%actor.vnum% == -1) then
  wait 1
  mechoaround guard The guard grins broadly at you, showing rows of sharp crooked teeth.
  wait 1s
  say Ahaaa!!  Outsider!  Hey-- youwannagetin, only a few coins?
else
  wait 1
  mechoaround guard The guard nods to you, a slight twinkle in his eyes.
  wait 5 s
  close door
  wait 5s
  lock door
end
~
#2102
Guard's bribe~
0 m 1
~
if %amount% < 50
  wait 3
  mechoaround guard It pockets the money.
  wait 1s
  say Not enough!
elseif %amount% > 199
  wait 3
  mechoaround guard It pockets the money.
  wait 6s
  mechoaround guard Suddenly, the guard bursts into a fit of high-pitched laughter.
  wait 2s
  'Okaaaaay -- haha, I letchooin, moneybags!
 
  wait 1s
  unlock door
  wait 1s
  open door
  wait 3s
  mechoaround guard The guard smirks to himself.
  wait 20s
  mechoaround guard The darkgnome guard twiddles its thumbs.
  wait 5s
  close door
  wait 1s
  lock door
 
else
  wait 3
  mechoaround guard It pockets the money.
  wait 2s
  unlock door
  wait 1s
  open door
 
  wait 20s
  mechoaround guard The darkgnome guard twiddles its thumbs.
 
  wait 5s
  close door
  wait 1s
  lock door
end
~
#2103
Chamber of Antiplane Guard's grab'n'sack attack~
0 g 100
~
if (%direction% == north) and
   (%actor.vnum% == -1) then
  wait 1s
  say Fiveseconds beforeIpoundyou.
  wait 2s
  say Five...
  wait 2s
  say Four...
  wait 1s
  say Three!
  wait 2
  say Two!
  wait 2
  say One!!
  wait 2
  mkill %actor%
else
  wait 1s
  mechoaround darkgnome The large darkgnome glares coldly at you as you pass.
end
~
#2104
Slavemaster's delayed aggie~
0 g 100
~
if (%actor.vnum% == -1) then
wait 1s
mecho %actor% The slavemaster eyes %actor.name% coldly.
msend %actor% The slavemaster eyes you coldly.
wait 2s
say How dare you come here!  You die!!
wait 1s
mkill %actor%
end
~
#2105
new trigger~
2 c 100
boo~
wait 1s
wecho boo
~
#2107
m1918 deathload of o2007~
0 f 100
~
eval objload %random.10%
if (%objload% == 1) then
mload obj 2007
end
~
#2196
m2065 Trainer; Disguise&Envenom~
0 d 1
disguise envenom~
if (%speech% /= disguise) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
say You are not in the Guild. I suggest you leave, or something bad may happen to you.
wait 2s
msend %actor% Rhanna's eyes flash dangerously, giving you the impression she means exactly what she says.
mechoaround %actor% Rhanna's eyes flash dangerously at %actor.alias%, giving the impression that she is quite serious.
else
wait 3s
nod
wait 2s
say Yes.. I can teach you how to use this skill, though it will remain your responsibility to use it well.
wait 4s
say You can disguise yourself as anyone you wish, any path of life as long as you act the part. You must make your audience believe that you are what you are pretending to be.
wait 6s
say There are a few general tools you must use.. facial putty, to sculpt the features. It won't do if you still look like yourself, simply dressed in a costume. Very few times you will not need the putty to aid in your disguise.
wait 7s
say Having a specific item recognizeable to the path of life you're attempting to mimic is a must. People won't believe you're a beggar if you're wearing expensive armor and gear.
wait 6s
say Some disguises may require a number of items in order to accurately portray the person you're trying to disguise yourself as. It is up to you to discover what you can use, and how you can use it.
wait 6s
say If you have disguised yourself accurately, you will look like any other person among the crowds. You will not be recognizeable to anyone, even those that know you personally. However, there are those that can see through the disguise, if they truly look.
wait 8s
say Mostly, these are our brethren, and pose little danger unless you have crossed them somehow. Be wary though, for certain monks have an annoying knack for seeing through a disguise, and they may alert others.
wait 6s
say See old Harkin after our lesson, if you wish to purchase a few items to get you started.
wait 3s
msend %actor% Rhaana whispers to you, "Just say the word 'shouldertap' and he'll show you to the wares."
mechoaround %actor% Rhaana whispers something to %actor.alias%.
wait 3s
mforce %actor% prac disguise
end
elseif (%speech% /= envenom) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
say You are not of the Guild, thus I will not teach you. I suggest you leave, or you can experience it's effects firsthand.
wait 4s
msend %actor% Rhanna's eyes flash dangerously, giving you the impression she means exactly what she says.
mechoaround %actor% Rhanna's eyes flash dangerously at %actor.alias%, giving the impression that she is quite serious.
else
wait 3s
think
wait 2s
say Yes.. I can teach you about poisoning your weapons.
wait 3s
emote moves to the desk at the center of the room, taking a vial from the rack.
wait 3s
say Watch closely. Poisons can be very dangerous to yourself if applied incorrectly.
wait 3s
emote unstoppers the vial carefully, and gets a dagger from the desk.
wait 4s
emote slowly, carefully applies a thin coating of the dark liquid along the length of the blade.
wait 5s
emote places the empty vial on the desk, and raises the dagger for you to examine.
wait 4s
say Note that I have not let the poison touch my skin. If it does, I will be affected.
wait 3s
say Assuming you have applied the toxin correctly, you have a short amount of time before it becomes ineffective. Thus, once applied you will want to strike your victim quickly. It is only good for one use.
wait 5s
say Once poisoned, your victim will become weak and nauseous. It will also hamper their ability to heal wounds while affected, as well as exhaust them physically, making them slow.
wait 5s
say You may practice here, on these weapons. Use caution, please. I don't wish to waste a large amount of anti-toxin curing a clumsy footpad.
wait 4s
say Once you've finished, see old Harkin for supplies if you wish.
wait 3s
msend %actor% Rhaana whispers to you, "Just say the word 'shouldertap' and he'll show you to the wares."
mechoaround %actor% Rhaana whispers something to %actor.alias%.
wait 3s
mforce %actor% prac envenom
end
end
~
#2197
m2080 Harkin Password~
0 d 1
shouldertap~
if (%speech% /= shouldertap) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
msend %actor% Harkin looks you over closely, frowning a bit.
mechoaround %actor% Harkin looks %actor.alias% over closely, frowning a bit.
wait 4s
say Hmm.. I dunno what yer talkin about, sorry.
wait 2s
say Maybe buy a cure-all instead? S'good fer what ails ye.
wait 3s
smirk
else
wait 3s
msend %actor% Harkin looks you over closely, nodding inwardly.
mechoaround %actor% Harkin looks %actor.alias% over closely, nodding inwardly.
wait 3s
say Go right on in, friend.
wait 2s
emote motions towards a door at the back of the shop.
wait 3s
mechoaround %actor% %actor.alias% disappears through the door at the back of the shop.
msend %actor% You exit west, into the back room of the Quota.
mteleport %actor% 2066
wait 1s
mforce %actor% look
end
end
~
#2198
m2080 Harkin Greet~
0 g 100
~
if (%direction% == east) then
wait 4s
msend %actor% Harkin ceases sorting through his inventory, and turns his attention to you.
mechoaround %actor% Harkin ceases sorting through his inventory, turning his attention to %actor.alias%.
wait 3s
say Welcome.. welcome. I buy an sell jist about anythin, so maybe we can work out a few deals hmm? Feel free ta look around. I give good coin fer yer unwanted items, jist lemme have a looksee.
wait 5s
grin
wait 3s
emote returns to work, keeping an eye on his shop.
elseif (%direction% == west) then
wait 3s
msend %actor% Harkin turns to look at you as you leave the back room.
mechoaround %actor% Harkin turns to look at %actor.alias% as they exit the back room.
wait 3s
say Ye find what ye were lookin for? Remember ta come back anytime ye need somethin.
wait 3s
emote returns to arranging his wares.
end
end
~
#2199
Tarnael speech trig unfinished~
0 d 1
pick locks forgery~
if (%speech% /= pick locks) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
say You aren't in the guild, shove off!
else
wait 3s
say Yes.. Lock Picking. I can teach ya the basics, sure.
wait 3s
think
wait 3s
say Right.. the art of lock picking is one the rogue's trademark skills. Any thief worth 'is salt should know this skill.
wait 4s
say Ye need a set of charms; that's lockpicks, kid. You can't open a lock without a key or a set 'a charms. Since the key usually isn't available, a set a charms becomes your best friend.
wait 5s
say Some locks you won't be able to pick. Those are usually magicked, and then it's no use.
wait 3s
say Anyhow, come over here an I'll show ye how to tell a joke. That's slang for picking a lock, kid.
wait 3s
say Oh yea.. ye can see Harkin about a set 'a charms when we're through here if ye want.
wait 3s
msend %actor% Tarnael whispers to you, "Just say the word 'shouldertap' and he'll let ya see the wares."
mechoaround %actor% Tarnael whispers something to %actor.alias%.
wait 3s
mforce %actor% prac pick
end
elseif (%speech% /= forgery) then
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
msend %actor% Tarnael pushes his spectacles up, looking at you closely.
mechoaround %actor% Tarnael pushes his spectacles up, looking at %actor.alias% closely.
wait 3s
cough
wait 3s
say Did I say forgery? That's odd.. I uh.. I don't know anything about that, sorry.
else
wait 3s
grin
wait 2s
say Aye, I can teach you about forgery. This is a special skill, kid. Somethin that's completely up to you and your skill with it.
wait 3s
say Everybody does it different, and the amount of success ye have depends on how well ye pay attention ta detail.
wait 4s
say The idea goes like this; say ye got someone you could set up real nice if some kind of dirty information got out about em... say a piece of mail was discovered with this person's name on it, talking about disgraceful deeds they were participatin in.
wait 6s
say Now, they didn't really write or recieve that letter.. it was forged! As long as you pay attention to detail, you can write up things like that. Mail, warrants, patence of nobility, you name it. If it's on parchment, ye can probably forge it up.
wait 6s
say See Harkin after our lesson for supplies if you want.
wait 2s
msend %actor% Tarnael whispers to you, "Just say the word 'shouldertap' and he'll let ya see the wares."
mechoaround %actor% Tarnael whispers something to %actor.alias%.
wait 3s
say Now.. come over here so I can show ye how ta do this.
wait 3s
emote proceeds to forge some documents for display.
end
end
~
$~

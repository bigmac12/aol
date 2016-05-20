#4600
m4600 Treant's Greetings~
0 g 100
~
if (%actor.vnum%) == -1
wait 7s
emote begins to shift, and something resembling a face forms in its boughs.
wait 2s
say Welcome, children, to my forest.
wait 2s
wait 2s
emote blinks a few times, as if awakened from a long slumber.
wait 2s
say My, my.  We get all sorts here, don't we?
wait 3s
emote looks at you a long moment, as if sizing you up, or perhaps trying to read something from you that is partially hidden.
wait 2s
wait 4s
say You may rest here as long as you wish, but bewarned.
wait 2s
emote frowns, its leaves rustling worriedly.
wait 1s
say Evil creatures have entered this wood, and I cannot guarantee your safety.
wait 3s
emote frowns thoughtfully.
wait 3s
say Alas, your kind does what it wants to anyway, but do not say I did not warn you.
wait 4s
emote yawns, its branches stretching to the sky.
wait 2s
say I grow weary...  so...
wait 3s
say very...
wait 4s
say tired...
wait 5s
emote Seems to have fallen asleep once again.  It sits motionless for a long time.
end
~
#4601
r4600 Enter and they Scatter Trigger~
2 g 100
~
if (%actor.vnum% == -1)
wait 1s
wecho Upon entering the clearing, you behold a gathering of woodland denizens of all sorts.
wait 3s
wecho Before you can look around to get a good look at any of them, they fade and vanish almost into thin air.
wait 60s
end
~
#4602
m4661 Greet Trigger~
0 g 100
~
if (%actor.vnum% == -1)
wait 10s
emote turns from the altar to stand before you, bowing with a small smile.
wait 3s
say Hail, seekers.  Thou art welcome in my sight, for a long and perilous journey must it have been.
wait 5s
say If it is enlightenment thou seekest, find it from mine lips.
wait 2s
:smiles warmly at you.
wait 4s
say I may speakest to thou of the Gods and Goddesses of Nature, for much knowledge hath I upon them.
wait 4s
:looks about the clearing, smiling.
wait 3s
say Thou must forgivest the creatures of the forest, for they are unused to thy kind.  Relaxeth thou amongst the grass, and make thyself at home, for thou mayest remain here as long as thou shalt wish.
wait 7s
:smiles at you, then returns to the altar.
end
~
#4604
r4606 Say Peace to get to 4605~
2 d 100
Peace~
if (%actor.vnum% == -1) then
wait 3
wecho &GThe soft breeze stops momentarily as if pondering your answer.&n
wait 2s
wecho The vines twist and writhe, revealing a gate!
wdoor 4606 north flags ab
wdoor 4606 north name gate
wdoor 4606 north room 4605
wait 15s
wecho Slowly, the vines close together, sealing the gate.
wdoor 4606 north purge
end
~
#4605
r4606 Before the Gate Greet Trigger~
2 g 100
~
if (%direction% != north) and (%actor.vnum% == -1) then
wait 1s
wecho &GA soft breeze begins to blow as you approach the vine gate.&n
wait 3s
wecho &WA soft voice on the breeze &Gsays&W, '&CYou stand, seeker, before the Gates of Tranquility.&n'
wait 3s
wecho &WThe voice &Gcontinues&W, '&CSpeak your wishes, and if what you seek is contained therein, the gates shall swing wide for you.&n'
wait 2s
wecho &gThe area falls silent, as though awaiting your reply.&n
end
~
#4606
o4690 load on corpse of m4606~
0 f 100
~
if (%random.50% == 1)
   mload obj 4690
   end
~
#4607
r4607 Crawling~
2 g 100
~
if (%actor.vnum% == -1)
wait 1s
wsend %actor% You crawl for what seems like hours through the narrow passageway.
wait 3s
wsend %actor% It is difficult to guess exactly how much distance you have travelled.
end
~
#4609
o4609 wear to load imp~
1 j 0
~
wait 2s
osend %actor% As you hold the orb, it grows warm and then flashes with a brilliant red light!
oechoaround %actor% %actor.alias% holds the crystal ball aloft, it flashes with a brilliant red light!
wait 3
oload mob 4622
oecho An ugly little quasit appears in a puff of foul-smelling yellow smoke.
wait 3
oforce quasit burp
wait 2s
osend %actor% The crystal ball becomes too hot to hold!
odamage %actor% 20
wait 30s
opurge quasit
oechoaround quasit An ugly little quasit makes a few rude noises and obscene gestures before vanishing into thin air.
~
#4611
UNUSED~
0 f 100
~
if (%random.3% == 1) then 
mload obj 4626
end
~
#4612
m4613 death load o4626/97~
0 f 100
~
if (%random.100% > 99) then
mload obj 4697
elseif (%random.100% < 11) then
mload obj 4626
elseif (%random.100% == 13) then
mload obj 4626
mload obj 4697
end
~
#4613
m4613 Pegasus Rider mounts the Pegasus (not that way, you sick bastard)~
0 g 100
~
if (%actor.vnum% == -1) then
   wait 7s
   emote eyes you up for a long moment.
   wait 4s
   emote nods at you solemnly.
   wait 3s
   say Welcome traveller.
   wait 6s
   ruff pegasus
   wait 3s
   say Aye, a magnificent creature, is it not?
   wait 4s
   emote climbs nimbly onto the pegasus' back.
wait 8s
   emote turns his mount around and faces you.
   wait 4s
   emote raises his hand in salute.
   wait 3s
   say A word of warning to you.
   wait 5s
   emote gets another good look at you before continuing, his face growing grim.
   wait 5s
   say Beware the eastern woods.
   wait 3s
   say There are evil things there, and the wise would do well to avoid them entirely.
   wait 6s
   say But perhaps, just perhaps you are brave or foolish enough to enter them.
   wait 4s
   say The entryway is barred.  You will need to find the keys to enter.
   wait 6s
say Were I you, I would head instead to the Gate of Tranquility, though you will have to prove you are worthy in order to pass it.
wait 4s
say When asked what you seek, speak of the peace in your heart, and you may enter.
wait 4s
say That is all the assistance I can offer.  Again, good luck to you, traveller.
   wait 3s
emote prepares to take off into the skies, having given his warning to you.
   wait 10s
   emote stares off into the sky for a long moment, leaving you to your own thoughts.
   end
~
#4617
o4610 black helm load on m4617~
0 f 100
~
if (%random.50% == 1)
   mload obj 4610
   end
~
#4625
m4625 Warning Trigger~
0 g 100
~
if (%direction% == east) and (%actor.vnum% == -1) then
wait 1s
emote looks up as you approach, nodding with satisfaction.
wait 2s
say So, I was right.  Travellers.
wait 3s
emote looks you over casually, then goes back to his observation.
wait 2s
say Careful if you're going down there.
wait 3s
emote narrows his eyes as he spots something in the distance.
wait 3s
say Pretty enough place, but the ankhegs will eat you alive if you're not careful.
wait 3s
emote reaches slowly for the bow on his back.
wait 3s
say That's what I hunt.
wait 2s
grin
wait 3s
say Well, you've had your warning.  Now off with you, you'll scare off my quarry.
wait 4s
emote turns his back to you, going back to his work.
end
~
#4627
r4627 Father of the Forest~
2 g 100
~
if (%actor.vnum% == -1)
wait 1s
wecho You feel a warmth upon your skin, as though the sun were shining directly upon you.
wait 4s
wecho After a long moment, suddenly the tree comes to life!
end
~
#4634
r4634 Move the Rock Trigger~
2 c 100
south~
If (%actor.vnum% == -1) then
wait 3s
wsend %actor% You approach the boulder and brace yourself to attempt to move it.
wechoaround %actor% %actor.name% approaches the boulder and prepares to try and move it.
wait 3s
wsend %actor% The boulder is surprisingly easy to move, despite its weight.
wechoaround %actor% %actor.name% moves the giant boulder with surprising ease!
wait 3s
wsend %actor% You have revealed a long passageway leading into the cliffside!
wechoaround %actor% A hidden passageway into the cliffside has been revealed.
wdoor 4634 south room 4637 flags a
wait 13s
wecho The boulder slides back into place, blocking the passageway.
wdoor 4634 south purge
end
~
#4635
r4635 Splash~
2 g 100
~
if (%direction% == up) and (%actor.vnum% == -1) then
wait 1s
wecho &BYou land in the water with a loud '&CSPLASH!&B'&n
wait 2s
wecho After a few seconds of thrashing about, you are able to pull yourself to shore.
wait 3s
wecho It takes a moment to dry off.
end
~
#4636
r4636 Fallin' More~
2 g 100
~
if (%actor.vnum% == -1)
wait 1s
wecho It is a long fall.
wait 1s
wteleport %actor% 4635
end
~
#4637
r4637 Exit Closes Behind!~
2 g 100
~
if (%direction% == north) and (%actor.vnum% == -1) then
wait 1s
wsend %actor% The passageway is a very tight fit.
wait 2s
wsend %actor% The boulder slides back into place behind you, sealing you in the passageway!
end
~
#4638
r4638 Slip and Fall~
2 g 100
~
if (%direction% == south) and (%actor.vnum% == -1) then
wait 2s
wecho The view from here is very enticing.
wait 3s
wecho You cannot resist taking a closer look, strolling out onto the rocks.
wait 2s
wecho The rocks are somewhat slippery...
wait 2s
wecho You lose your footing!
wforce %actor% down
end
~
#4641
m4641 Grand Centaur Gods Speech Trigger~
0 d 1
Habbakuk Chislev Zeboim Zivilyn~
if (%speech% /= Habbakuk) then
wait 2s
emote smiles warmly at you, a delighted twinkle in her deep green eyes.
wait 5s
say Habbakuk hath been also known as "The Fisher King", or "The Blue Phoenix".  T'was he that created and doth rule all creatures of land and sea.  He holdst dominion over animals, water, and the fires of passion.
wait 7s
say His hues art deepest blue, and purest white.  His symbol, the Blue Phoenix, doth represent rebirth and the cycle of life.
wait 5s
say Habbakuk is the middle son of Paladine and Mishakal, and is twin brother to Kiri-Jolith.  Rangers and Sailors doth serve him reverently, as doth the Knights of Solamnia, especially the Order of the Crown, of which he is Patron.
wait 7s
emote smiles warmly to you, bowing her head respectfully.
elseif (%speech% /= Chislev) then
wait 2s
emote combs a hand through her luxuriant golden locks, pausing for thought.
wait 5s
say Chislev is also known as "The Beast", or "The Wild One" by some minotaurs.  Her dominions art air, earth, florae and faunae.
wait 4s
say Chislev art nature incarnate.  In accordance to legend, the seasons doth change with the moods of her heart: her sorrow bringeth the fall, her despair the winter, her hope the spring, and her joy the summer.
wait 5s
emote looks down to the grass at her hooves, a small smile still holding upon her face.
wait 3s
say She hath no constellation, for instead she is represented in the night sky by the planet that doth bear her name.
wait 4s
say Chislev's colours art brown, yellow, and green.  A feather of these colours doth serve as her symbol.
wait 3s
say Her worshippers art the druids and farmers, although every natural creature and plant upon Krynn revereth her, as doth members of many humanoid races.
wait 4s
say Chislev's tenets of faith are thus:
wait 2s
say Nature hath no beginning nor end.  The balance of nature must be maintained at all costs.  Nature perforce must be protected and nurtured, not enslaved or despoiled.  There art places for technology and new developments, but these must be second always to the true gift that be nature.
wait 8s
emote smiles warmly to you, flipping her golden locks behind her shoulder.
elseif (%speech% /= Zeboim) then
wait 2s
emote frowns slightly at the mention of the name, crossing her hands in front of her abdomen.
wait 5s
say Zeboim is known also as "The Darkling Sea", and "Maelstrom" by the minotaurs, and as "Rann" by the Ergothians.  Amongst others she is commonly referred to as "The Sea Queen".
wait 6s
say As her titles doth suggest, she is goddess of seas, and of storms.  She is a tempestuous and most evil goddess!
wait 4s
emote looks almost subconsciously off to a blighted patch of forest a distance off to the east.
wait 3s
say Her colours art red and green, and her symbol is that of a turtle shell.
wait 4s
say Zeboim counteth sailors, pirates, and evil druids amongst her worshippers.
wait 6s
emote makes a small hand-sign meant to ward off the taint of evil, then smiles at you once more.
elseif (%speech% /= Zivilyn) then
wait 2s
emote pauses a moment to gather her thoughts.
wait 5s
say Zivilyn hath been known as "Wise One" by the minotaurs, and the "World Tree", or the "Tree of Life" by most others.  He is worshipped by philosophers and mediators.
wait 4s
say His colours art gold and green, and his symbol be that of a great gold and green tree.  He is represented in the night skies not by constellation, but by the planet which doth bear His name.
wait 4s
say Zivilyn worketh in harmony with his mate, Chislev, to maintain balance in the world of Krynn, though he be not a deity of nature Himself.
wait 6s
emote smiles warmly to you, bowing her head respectfully.
end
~
#4645
o4638 Cold Turkey~
1 h 100
~
osend %actor% You reluctantly let the black lotus drop to the ground.
oechoaround %actor% %actor.alias%'s eyes water as the lotus falls from nerveless fingers.
~
#4646
o4638 Black Lotus Madness!!!~
1 b 45
~
if !(%pwearer% == 0)
eval dummy %random.55%
if (%dummy% == 1)
oechoaround %pwearer% %pwearer% slowly peels a dewy black petal from the lotus and eats it.
elseif (%dummy% == 1)
osend %pwearer% The sky begins to rain fire!
elseif (%dummy% == 2)
osend %pwearer% A feeling of fear overwhelms you!
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
osend %pwearer% You start to realize just how many people are truly out to get you.
oechoaround %pwearer% %pwearer% mumbles softly, shivering slightly.
elseif (%dummy% == 4)
osend %pwearer% You notice how easily the petals come off in your hand.  And how delicious they look.
oechoaround %pwearer% %pwearer% pulls another petal off of the black lotus, looking at it wistfully.
elseif (%dummy% == 5)
oforce %pwearer% giggle
elseif (%dummy% == 6)
oforce %pwearer% shiver
elseif (%dummy% == 7)
oforce %pwearer% scream
elseif (%dummy% == 8)
oforce %pwearer% peer
elseif (%dummy% == 9)
oforce %pwearer% stare
elseif (%dummy% == 10)
osend %pwearer% The wind tastes rather funny today.
elseif (%dummy% == 11)
osend %pwearer% a short, balding man has arrived from the south.
elseif (%dummy% == 12)
osend %pwearer% a lean elven man with scarred forearms leaves west.
elseif (%dummy% == 13)
osend %pwearer% a tall, muscular man with dragon tattoos has arrived from the east.
elseif (%dummy% == 14)
osend %pwearer% a large-busted girl with twinkling green eyes has arrived from the north.
elseif (%dummy% == 15)
osend %pwearer% Almanas introduces himself to you.
elseif (%dummy% == 16)
osend %pwearer% Jendrani introduces herself to you.
elseif (%dummy% == 71)
oechoaround %pwearer% %pwearer% stares into space vacantly, drooling.
osend %pwearer% You just realized how the universe works.
wait 1s
osend %pwearer% Oops!  You forgot!  Maybe if you thought about it long enough...
elseif (%dummy% == 18)
oechoaround %pwearer% %pwearer% weeps softly and shivers uncontrollably, 
elseif (%dummy% == 19)
oechoaround %pwearer% %pwearer% whispers to you, 'I have seen the gods!'
osend %pwearer% You begin whispering incoherently.
elseif (%dummy% == 20)
oechoaround %pwearer% %pwearer% whispers to you, 'I need more lotus.  Immediately!'
osend %pwearer% You start to have hunger pangs, and only Black Lotus will make them go away.
elseif (%dummy% == 21)
oechoaround %pwearer% %pwearer% whispers to you, 'We are all dying.  We simply have not yet begun to rot...'
osend %pwearer% You begin whispering incoherently.
elseif (%dummy% == 21)
oechoaround %pwearer% %pwearer% whispers to you, 'I grow weary.  I need food.'
osend %pwearer% Your stomach growls.
elseif (%dummy% == 22)
osend %pwearer% You are very hungry.
elseif (%dummy% == 23)
osend %pwearer% You are very thirsty.
elseif (%dummy% == 24)
osend %pwearer% You feel light-headed.
elseif (%dummy% == 25)
osend %pwearer% The ground starts moving, like waves in an ocean.
oechoaround %pwearer% %pwearer% stares in fright at the ground.
elseif (%dummy% == 26)
osend %pwearer% Your skin starts buzzing.
elseif (%dummy% == 27)
osend %pwearer% The sun rises in the west.
elseif (%dummy% == 28)
osend %pwearer% The sun sets in the east.
elseif (%dummy% == 29)
oechoaround %pwearer% %pwearer% pulls a petal from the black lotus and eats it.
osend %pwearer% You never realized how tasty black lotus truly is.  You eat some more of it.
elseif (%dummy% == 30)
osend %pwearer% The sky begins to turn very, very dark.
elseif (%dummy% == 31)
oechoaround %pwearer% %pwearer% sticks out a black-coated tongue.
osend %pwearer% Your mouth feels dry.
elseif (%dummy% == 32)
oechoaround %pwearer% %pwearer% glares harshly at you.
elseif (%dummy% == 33)
oechoaround %pwearer% %pwearer% stares longingly at you.
elseif (%dummy% == 34)
oechoaround %pwearer% %pwearer% stares at you, drooling.
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
oforce %pwearer% lick self
elseif (%dummy% == 42)
osend %pwearer% It begins to rain insects.  Better get an umbrella...
elseif (%dummy% == 43)
osend %pwearer% Music drifts in on the wind and fills your head with song.
elseif (%dummy% == 44)
osend %pwearer% Squirrels on the ground advance upon you carrying tiny little weapons.
elseif (%dummy% == 45)
osend %pwearer% Your eyes feel like they are on fire!
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
osend %pwearer% Mold creeps across the ground, covering everything around except for you!
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
#4647
o4638 Becoming a Lotus Eater~
1 j 100
~
wait 4s
osend %actor% You feel an odd shiver in your hand as you clutch the lotus.
wait 20s
osend %actor% The shiver becomes an electric pulse, and moves up your arm.
wait 5s
osend %actor% From there, it does not take long for the pulse to spread throughout your entire body!
wait 3s
osend %actor% Warmth floods your body.  Your eyelids grow heavy.
oechoaround %actor% %actor.alias% begins to look rather dazed.
wait 15s
osend %actor% You begin to feel an urge to lie down.
wait 4s
oforce %actor% rest
wait 3s
osend %actor% You get the urge to eat some of the lotus petals.
wait 4s
osend %actor% You peel off one of the petals, and place it upon your tongue.
oechoaround %actor% %actor.alias% peels off one of the lotus petals and licks it.
wait 3s
oechoaround %actor% The petal disappears into %actor.alias%'s mouth.
osend %actor% Gulp!  Gulp!  Hmm.  That tasted rather pleasant.
wait 6s
osend %actor% You close your eyes for a moment.  The world starts to fall away...
wait 2s
osend %actor% ...eventually, you desire to open your eyes again.  You do.
oechoaround %actor% %actor.alias%'s eyes widen, pupils dilating.
wait 4s
osend %actor% A feeling of euphoria overcomes you.
eval pwearer %actor.alias%
global pwearer
~
#4648
o4638 Lotus Greed~
1 i 100
~
osend %actor% The idea of getting rid of the lotus brings tears to your eyes.
return 0
oforce %actor% cry self
end
~
#4649
m4649 Bleeding Tree~
0 g 100
~
   if (%actor.vnum% == -1) then
   wait 1s
   emote shakes its branches at your approach.
   wait 2s
   emote oozes with foul-smelling blood!
   end
~
#4654
o4692 load on death of m4654~
0 f 100
~
if (%random.3% == 1) then
   mload obj 4692
   end
~
#4657
o4609 wear trigger~
1 j 100
~
wait 2
osend %actor% you suck
wait 2s
odamage %actor% 20
wait 2s
odamage %actor% 20
wait 2s
odamage %actor% 20
wait 2s
odamage %actor% 20
~
#4658
r4658 xyz trigger to open doorway~
2 c 100
xyz~
wait 3
wecho The wall of force to the east dissipates!
wdoor 4658 east room 4659 flags a
wait 7s
wecho The wall of force slowly reforms, again blocking passage to the east.
wdoor 4658 east purge
~
#4660
r4660 Overcast~
2 g 100
~
   if (%actor.vnum% == -1) then
   wait 1s
   wecho The area seems to grow darker.  You hear the rustling of leaves.
   wait 1s
   wecho Looking up, the trees seem to have closed in around you.
   wait 2s
   wecho You can no longer see the sky, and all is dark.
end
~
#4661
m4662 Xirokquash~
0 d 1
Xirokquash~
wait 1s
if (%actor.room% == 4658) then
emote flies up to the wall of force.
wait 3
wave %actor%
wait 3
mforce %actor% xyz
wait 2
emo flies into the wall, resulting in a flash of light.
else
emote flies around in a small circle.
wait 3
emote makes a very rude gesture in your general direction.
end
~
#4679
r4679 No Exit Trigger~
2 g 100
~
if (%direction% == north) and (%actor.vnum% == -1) then
wait 1s
wsend %actor% The hidden passageway crumbles behind you.
wait 3s
wsend %actor% There is no way to get back in from this side.
end
~
#4681
o4691 load on death of m4681~
0 f 100
~
if (%random.3% == 1)
   mload obj 4691
   end
~
#4689
r4689 Swirling Down from the Sky...~
2 g 100
~
   if (%actor.vnum%==-1)
wait 4s
   wecho &CUp in the sky above, a white, winged shape flies about in wide circles.&n
wait 6s
   wecho &WAs the shape swoops lower, it is recognizeable as a pegasus, a beautiful winged horse famed in legend.&n
   wait 5s
   wecho The pegasus lands gracefully upon the tower a short distance away from you
wload mob 4609
wait 82s
wpurge rider
wpurge pegasus
   wecho The pegasus leaps off of the tower!
wait 4s
   wecho &CWithin seconds, the pegasus is little more than a dot upon the horizon.&n
   end
~
#4690
r4690 Ouch!  Landing!~
2 g 100
~
if (%direction% == up) and (%actor.vnum% == -1) then
wait 1s
wsend %actor% There is no way to slow your descent!
wechoaround %actor% %actor.alias% comes hurtling to the ground and lands heavily amongst the rocks!
wdamage %actor% 20
wait 1s
wsend %actor% OUCH!
end
~
#4696
r4658 When Serpents Attack!~
2 g 100
~
   if (%actor.vnum%==-1)
wpurge
   wait 1s
   wecho Your intrusion into this part of the woods seems to have awoken something from below.
   wait 3s
   wecho &KThere is an explosion of soil and dust as a gargantuan grey earth-serpent attacks!!&n
   wload mob 4696
   end
~
#4699
m4600 Protect the Forest Father!~
0 l 65
~
if (%actor.vnum% == -1)
wait 2s
mechoaround father The ground comes alive, threatening to swallow you whole!
mpurge
end
~
$~

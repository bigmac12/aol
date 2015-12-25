#10000
Drunk Newbie Help~
0 g 100
~
If (%actor.vnum% == -1) then
   if (%actor.level% <=2) then
      wait 1s
      bow
      wait 2s
      mecho beggar The beggar says, 'You look like youre new around here, gimme 1 copper piece and I might be able to help you out!'
      wait 3s
      mecho beggar The beggar says, 'I know lots of stuff about the city, lived in the alleys most of my life.'
      wait 2s
      mecho beggar The beggar holds out his hand and smiles.
   elseif (%actor.level% >=3) then
      if (%random.4% == 1) then
      wait 2s
         mecho beggar The beggar mumbles, 'Crazy?.......I gonna........MMrbbMrrmmmm'
         curse
         end
      else
      end
   else
   end
else
end
***substitute drunk for noble***
~
#10001
Newbie Help Info~
0 m 1
~
if (%amount% >=1) then
   set num %random.6%
      if (%num% == 1)
         wait 1s
         mecho beggar The beggar says, 'Sheard of Stanley? He rents rooms to people, but only if you can afford it.  Her inn is just east of Turbidus square on Huma Lane.'
      elseif (%num% == 2)
         wait 1s
         mecho beggar The beggar sighs, 'Look out for those mean city guards.  I got in a fight once with this dwarf and they beat me up real good for causing a disturbance.'
      elseif (%num% == 3)
         wait 1s
         mecho beggar The beggar says, 'Thirsty?  If you are just drink from the fountain in the square...but I gotta warn you, I bathe in it at night when no-ones around.'
         wait 2s
         mecho beggar The beggar laughs heartily.
      elseif (%num% == 4)
         wait 1s
         mecho beggar The beggar says, 'Some of those sailors on the east docks are real wimps, just   be careful of the officers.  They all stink anyway... like dead fish.'
      elseif (%num% == 5)
         wait 1s
         mecho beggar The beggar says, 'Youre gonna need skills around here.  I used to practice mine at this guys place just east of the square till I showed up drunk once.  Oh yeah, I think theres another to the north but they never let me in.'
      else
         wait 1s
         mecho beggar The beggar cringes and says, 'Watch out for animals on the roads outside of town!  I got bitten once and couldnt walk for a week!'
         wait 2s
         mecho beggar The beggar pulls up a pant leg and shows you the scar.
else
end
end
~
#10002
KenderFollow~
0 g 100
~
Nothing.
~
#10010
m10015 manners and such~
0 c 100
french~
If (%actor.vnum% == -1) then
return 0
wait 2s
frown
wait 3s
cough
wait 3s
say Umm..sirs, madams...perhaps ye seek the inn for such behaviours
end
~
#10011
knights !cursing~
0 e 0
swears an oath harsh enough to make a sailor blush~
If (%actor.vnum% == -1) then
return 1
wait 2s
frown
wait 3s
say Please, gentle folk, refrain from such language within earshot of ladies and the town's youth
end
~
#10012
knight !grope~
0 e 100
gropes~
If (%actor.vnum% == -1) then
return 1
wait 2s
blink
wait 2s
scowl
wait 3s
say Get ye to some place more private if ye wish to carry on in such a manner, this is a public place!
end
~
#10013
Kal rose knight aggro~
0 g 100
~
If (%actor.align% < 0) then
wait 12
mecho %actor% The commander looks up from the papers on his desk, eyeing you over carefully.
msend %actor% The commander looks up from the papers on his desk, eyeing you over carefully.
wait 15
say Are ye sure that you belong here?  Those of the dark paths are not allowed in Kalaman any longer.
wait 15
frown
wait 15
say Very well, ye were warned.....
wait 12
emote stands from behind his desk, draws his sword, and performs a precise solamnic sword salute.
wait 1s
mkill %actor%
end
~
#10014
kalaman knights delayed !evil~
0 g 100
~
If (%actor.align% < 0) then
mecho %actor% The knight comes up short, looking the one in question over carefully.
msend %actor% The knight comes up short, looking you over carefully.
wait 5
say Are ye sure you belong here?  Those of the dark paths are not allowed in Kalaman.
wait 5
frown
wait 5
say Very well, ye were warned.....
wait 5
emote performs a precise solamnic sword salute and prepares to attack.
wait 4
mkill %actor%
end
~
#10015
o10012 Send Prisoner to Jail~
1 i 100
~
if ((%victim.vnum% == -1) & (%actor.vnum% == -1))
if (%actor.clan% != 1)
wait 1s
elseif (%victim.level% > 30)
wait 1s
elseif (%victim.clan% == 1)
wait 1s
elseif ((%actor.clan% == 1) or (%actor.vnum% != -1))
wait 1s
osend %victim% You are under arrest and %actor.name% drags you off to prison.
oteleport %victim% 10150
oechoaround %actor% %victim.name% has been arrested by %actor.name% and taken to prison.
osend %actor% You have arrested %victim.name% please report this arrest, with details of the offenses, on the crime board.
osend %victim% You arrive in the prison.
oforce %victim% look
set %victim.prisoner% 1
end
end
~
#10016
r10042 bell pulley on prison door~
2 c 100
ring~
if ("%arg%" == "bell")
   wsend %actor% You pull on the chain.
   wechoaround %actor% %actor.name% pulls on the chain.
   wait 1s
   wecho A bell can be heard ringing within the prison.
   wait 5s
   wecho A small hatch in one of the doors is opened.
   wait 1s
   wechoaround %actor% A pair of eyes peer out the hatch at %actor.alias%.
   wsend %actor% A pair of eyes peer out the hatch at you.
   wait 1s
   wecho A dark voice says, 'I suppose you're here for a visit.  Very well.'
   wait 1s
   wechoaround %actor% Someone opens the doors and gives %actor.name% an appraising glance before ushering them into the gaol.
   wsend %actor% Someone opens the doors and gives you an appraising glance before ushering you into the gaol.
   wait 1s
   wteleport %actor% 10150
   wforce %actor% look
   wechoaround %actor% A visitor arrives in the gaol.
end
~
#10017
m10019 news~
0 b 15
~
     set num %random.7%
      if (%num% == 1)
      say Hear Ye, Hear Ye
      wait 20s
      say Hear Ye, Hear Ye
      wait 20s
      say The Conclave is looking for any information concerning a Renegade Mage, who is reported to be on the loose.
      elseif (%num% == 2)
      say Hear Ye, Hear Ye
      wait 20s
      say Hear Ye, Hear Ye
      wait 20s
      say In hopes of raising revenues for our fair city, the civic leaders have decided to offer a few townhouses for sale.
      wait 10s
      say The tax revenues should do wonders for keeping our city looking its loveliest.
      wait 10s
      say Though, I heard it Stanley was none to thrilled.
      elseif (%num% == 3)
      say Hear Ye, Hear Ye
      wait 20s
      say Hear Ye, Hear Ye
      wait 20s
      say A recent influx of outsiders has civic leaders concerned.
      wait 10s
      say Large groups of armed people wielding weapons have been reported loitering in this very square.
      wait 10s
      say The people of Kalaman would request that the outsiders refrain from this sort of loitering.
      wait 10s
      say The Knights have promised to look into the situation should the problem continue.
elseif (%num% == 4)
say Hear Ye, Hear ye
wait 20s
say Hear Ye, Hear ye
wait 20s
say There have been reports of unusual foul odors in and around the city at night.
wait 10s
say City officials believe them to be linked to the shadowy creature attacks in the outskirts of the city.
wait 10s
say If you see a suspicious figure, call for a knight immediately.
elseif (%num% == 5)
say Hear Ye, Hear ye
wait 20s
say Hear Ye, Hear ye
wait 20s
say A new barracks for the Kalaman Militia is currently being built this year.
wait 10s
say Thanks go out to the generous donators which made this possible.
elseif (%num% == 6)
say Hear Ye, Hear ye
wait 20s
say Hear Ye, Hear ye
wait 20s
say Poisoned food has been found lying about in the city streets and temples.
wait 10s
say Clerics ask that you contact them immediately upon infection.
elseif (%num% == 7)
say Hear Ye, Hear ye
wait 20s
say Hear Ye, Hear ye
wait 20s
say The refugees of Mista and Talenrock need your support.
wait 10s
say Please send donations to the temple of Mishakal as tokens of mercy.
end
~
#10018
m10194 Visitor Trigger~
0 c 100
ring~
if ("%arg%" == "bell")
wsend %actor% You pull on the chain.
wechoaround %actor% %actor.name% pulls on the chain.
wait 1s
if (%actor.prisoner% == 1)
say And where do you think you are going?
wait 6
emo chuckles slightly as he taps the weapon at his side and gestures toward your cell.
wait 4
say Back to your cell with you.
elseif (%actor.prisoner% != 1)
say I'll let you back out to the street if your visit is over.
wait 4
mechoaround %actor% Murton opens the doors and escorts %actor.name% back out to the street.
msend %actor%  Murton opens the doors and escorts you back out to the street.
wait 4
mteleport %actor% 10042
mforce %actor% look
mechoaround %actor% A visitor is escorted out of the gaol.
end
end
~
#10019
PRISONER Attack Trigger for Guards and KoS~
0 g 100
~
if (%actor.prisoner% == 1)
peer %actor%
wait 2
say You look a bit familiar.  I do believe I have seen your likeness back at the Keep.
wait 2
say I had best be taking you back to the gaol.
wait 1
mkill %actor%
end
~
#10020
PRISONER Re-arrest for KoS and Guards~
0 k 100
~
say Back to the gaol with you.
wait 2
mload obj 10012
give %actor% warrant
~
#10059
elven adept bribe~
0 m 1
~
set num %random.3%
   if (%num% == 1)
   emote smiles widely.
   wait 3s
   say Thank you kindly, friend.
   wait 3s
   say I'll sing you one of the songs I've learned on my travels.
   wait 3s
   emote ponders over something a moment, then nods as if decided.
   wait 4s
   emote clears his throat, turning his attention to you.
   wait 3s
   say The Old Barkeep.
   wait 4s
   emote &Gsings&n, '&CDrink once, and if you drink twice&n'
   wait 3s
   emote &Gsings&n, '&CPlease heed an old barkeep's advice&n'
   wait 4s
   emote &Gsings&n, '&CJust after you've finished a flagon&n'
   wait 3s
   emote &Gsings&n, '&CDon't bet you can harness a dragon,&n'
   wait 3s
   emote &Gsings&n, '&CIt will take it amiss&n'
   emote &Gsings&n, '&CAnd probably hiss&n'
   wait 3s
   emote &Gsings&n, '&CAnd will surely demolish the wagon.&n'
   wait 3s
   emote smiles brightly at the gathered patrons.
   wait 3s
   emote &Gsings&n, '&CIf bottles of wine make you tender&n'
   wait 3s
   emote &Gsings&n, '&CDo not try to cuddle a kender,&n'
   wait 3s
   emote &Gsings&n, '&CFor you'll lose all your gold,&n'
   emote &Gsings&n, '&CAnd also I'm told&n'
   wait 3s
   emote &Gsings&n, '&CIt is hard to distinguish their gender.&n'
   wait 3s
   emote grins, stifling a laugh.
   wait 3s
   emote &Gsings&n, '&CAfter drinking a glass of the foam,&n'
   wait 3s
   emote &Gsings&n, '&CDo not buy machines from a gnome,&n'
   wait 3s
   emote &Gsings&n, '&CFor the levers and gears&n'
   emote &Gsings&n, '&CCould drive you to tears&n'
   wait 3s
   emote &Gsings&n, '&CAnd just trust me -don't try this at home.&n'
   wait 3s
   emote chuckles politely.
   wait 3s
   emote &Gsings&n, '&CWhen you're downing a grog on the wharf&n'
   wait 3s
   emote &Gsings&n, '&CRefrain, sir, from drowning a dwarf,&n'
   wait 3s
   emote &Gsings&n, '&CHe will shout and resist&n'
   emote &Gsings&n, '&CAnd brandish his fist&n'
   wait 3s
   emote &Gsings&n, '&CAnd make sure that your features all morph.&n'
   wait 4s
   emote &Gsings&n, '&CDrink once, and if you drink twice&n'
  wait 3s
   emote &Gsings&n, '&CPlease heed an old barkeep's advice.&n'
   wait 4s
   emote smiles and takes a bow.
   wait 3s
   say Thank you, thank you my friends.
   wait 3s
   emote finds his seat and returns to playing a simple melody.
   elseif (%num% == 2)
   emote smiles widely.
   wait 3s
   say Thank you, thank you. It's greatly appreciated.
   wait 3s
   say I'll sing you one of the songs I've learned on my travels.
   wait 3s
   emote ponders over something a moment, then nods as if decided.
   wait 4s
   emote clears his throat, turning his attention to you.
   wait 3s
   say I haven't a name for this song.. I heard a kender singing it as he walked down the road.
   wait 4s
   emote &Gsings&n, '&CYour one true love's a sailing ship&n'
   wait 3s
   emote &Gsings&n, '&CThat anchors at our pier.&n'
   wait 3s
   emote &Gsings&n, '&CWe lift her sails, we man her decks,&n'
   wait 3s
   emote &Gsings&n, '&CWe scrub the portholes clear;&n'
   wait 4s
   emote &Gsings&n, '&CAnd yes, our lighthouse shines for her,&n'
   wait 3s
   emote &Gsings&n, '&CAnd yes, our shores are warm;&n'
   wait 3s
   emote &Gsings&n, '&CWe steer her into harbor -&n'
   wait 3s
   emote &Gsings&n, '&CAny port in a storm.&n'
   wait 3s
   emote quickly knocks back a mug of ale.
   wait 3s
   emote &Gsings&n, '&CThe sailors stand upon the docks,&n'
   wait 3s
   emote &Gsings&n, '&CThe sailors stand in line,&n'
   wait 3s
   emote &Gsings&n, '&CAs thirsty as a dwarf for gold&n'
   wait 3s
   emote &Gsings&n, '&COr centaurs for cheap wine.&n'
   wait 3s
   emote chuckles politely.
   wait 3s
   emote &Gsings&n, '&CFor all the sailors love her,&n'
   wait 3s
   emote &Gsings&n, '&CAnd flock to where she's moored,&n'
   wait 3s
   emote &Gsings&n, '&CEach man hoping that he might&n'
   wait 3s
   emote &Gsings&n, '&CGo down, all hands on board.&n'
   wait 4s
   emote smiles and takes a bow, chuckling to himself.
   wait 3s
   say Thank you, thank you my friends.
   wait 3s
   emote finds his seat and returns to playing a simple melody.
   elseif (%num% == 3)
   emote smiles widely.
   wait 3s
   say Thank you, friend. Very fine gesture of you. You're a noble and a scholar.
   wait 3s
   say Let me sing you one of the songs I've learned on my travels.
   wait 3s
   emote ponders over something a moment, then nods as if decided.
   wait 4s
   emote clears his throat, turning his attention to you.
   wait 3s
   say I hope you've got your ale handy, because it Three Sheets to the Wind.
   emote grins slyly.
   wait 4s
   emote &Gsings&n, '&CSing as the spirits move you,&n'
   wait 3s
   emote &Gsings&n, '&CSing to your doubling eye,&n'
   wait 3s
   emote &Gsings&n, '&CPlain Janes become Lovable Lindas&n'
   wait 3s
   emote &Gsings&n, '&CWhen six moons shine in the sky.&n'
   wait 3s
   emote chuckles politely.
   wait 3s
   emote &Gsings&n, '&CSing to a sailor's courage,&n'
   wait 3s
   emote &Gsings&n, '&CSing while the elbows bend,&n'
   wait 3s
   emote &Gsings&n, '&CA ruby port your harbor,&n'
   wait 3s
   emote &Gsings&n, '&CHoist three sheets to the wind!&n'
   wait 3s
   emote grins and drains his mug of ale.
   wait 4s
   emote &Gsings&n, '&CSing while the heart is cordial,&n'
   wait 3s
   emote &Gsings&n, '&CSing to the absinthe of cares,&n'
   wait 3s
   emote &Gsings&n, '&CSing to the one for the weaving road,&n'
   wait 3s
   emote &Gsings&n, '&CAnd the dog, and each of his hairs.&n'
   wait 3s
   emote orders a mug of ale from Gillian.
   wait 3s
   emote &Gsings&n, '&CAll of the waitresses love you,&n'
   wait 3s
   emote winks at Gillian.
   wait 3s
   emote &Gsings&n, '&CEvery dog is your friend,&n'
   wait 3s
   emote &Gsings&n, '&CWhatever you say is just what you mean,&n'
   wait 3s
   emote &Gsings&n, '&CSo hoist three sheets to the wind!&n'
   wait 3s
   emote grins and drains his mug of ale thirstily.
   wait 3s
   say Thank you friends, thank you.
   emote smiles and takes a bow, chuckling to himself.
   wait 3s
   emote finds his seat and returns to playing a simple melody.
   end
~
#10094
new trigger~
0 g 100
~
set num %random.2%
if ((%actor.vnum% == -1) && (%num% ==1))
wait 1s
oechoaround %actor% The machine stutters into life and the wheel spins above %actor.name%.
elseif ((%actor.vnum% == -1) && (%num% == 2))
wait 1s
end
~
$~

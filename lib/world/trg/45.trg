#4553
m4553 Mage Greet~
0 g 100
~
eval pcclass %actor.class%
eval pcalign %actor.align%
if (%pcclass% != Mage) then
wait 3s
emote raises his head, irritated at being disturbed.
wait 3s
emote marks the page in the book at his lap, closing it.
wait 4s
emote looks you over for a moment, frowning as his eyes harden.
wait 5s
say Why do you disturb me? You are not welcome here.
wait 3s
say Get out, and close the door behind you.
wait 5s
emote stands, placing the book upon the cot.
wait 3s
say You have five seconds.
wait 3s
emote turns to face you, beginning to chant the spidery language of magic.
wait 2s
mkill %actor%
elseif (%pcalign% >= 0) then
wait 3s
emote raises his head, irritated at being disturbed.
wait 3s
emote marks the page in the book at his lap, closing it.
wait 4s
emote examines you for a moment, nodding in acknowledgement.
wait 5s
say Greetings. I am studying just now, a matter of importance as you well know.
wait 3s
say I bid you leave me to my studies, please.
wait 4s
emote retrieves the book, opening it to the page where he left off.
wait 4s
emote glances at you meaningfully one last time, before continuing his studies.
else
wait 3s
emote raises his head, irritated at being disturbed.
wait 3s
emote marks the page in the book at his lap, closing it.
wait 4s
emote examines you for a moment, nodding in acknowledgement.
wait 5s
say Greetings. I am studying just now, a matter of importance as you well know.
wait 3s
emote ponders over you a moment before continuing.
wait 4s
say Currently I am studying somatic components, most important to our work.
wait 4s
say I have little time to spare, but as a colleague in the Order of Nuitari, I suppose I can spare a moment if you have questions concerning that topic.
wait 6s
say If you'd like to know something about somatic components, just ask.
wait 3s
say Otherwise I bid you leave me to my studies, please.
wait 5s
emote retrieves the book, opening it to the page where he left off.
wait 3s
end
end
end
~
#4554
m4553 Speech~
0 d 1
identify enchant charm somatic~
eval pcclass %actor.class%
eval pcalign %actor.align%
if (%speech% /= enchant) then
if (%pcclass% == Mage) and
(%pcalign% >= 0) then
wait 3s
emote stops a moment to gather his thoughts.
wait 3s
say The incantation to enchant weaponry requires a pinch of powdered lime, which luckily can be found at most mageware shops. The darkgnomes make use of it as well, for some nefarious purpose.
wait 6s
say It is to be sprinkled over the weapon being enchanted, while speaking the words of the spell.
end
elseif (%speech% /= identify) then
if (%pcclass% == Mage) and
(%pcalign% >= 0) then
wait 3s
emote pauses a moment to gather his thoughts.
wait 7s
say The spell to identify an object requires a perfectly cut diamond as it's focus. These can be found in the tower shops, as well as some mageware shops. Occasionally, you can find a diamond of suitable quality at the jewelers as well.
end
elseif (%speech% /= charm) then
if (%pcclass% == Mage) and
(%pcalign% >= 0) then
wait 3s
emote pauses a moment to gather his thoughts.
wait 4s
say The incantation for the charming of others requires a pendulum of crystal as it's focus. It is to be rocked before the eyes of victim, while reciting the words of the spell.
wait 5s
say These can be difficult to find, however I have seen them in mageware shops, as well as the tower shop from time to time.
end
elseif (%speech% /= somatic) then
if (%pcclass% == Mage) and
(%pcalign% >= 0) then
wait 3s
say Very well then. I can tell you alittle about somatic components, though I can only tell you about the ones I have studied so far.
wait 7s
say Currently, I can tell you about the components for Enchant Weapon, Identify, Charm, Sleep, Scry, and Prismatic Spray. Just ask about the spell by name, and then I'll tell you about it's components.
end
else
end
end
end
end
~
#4555
m4553 Component Speech~
0 d 1
sleep scry prismatic spray~
eval pcclass %actor.class%
eval pcalign %actor.align%
if (%speech% /= sleep) then
if (%pcclass% == Mage) and
(%pcalign% >= 0) then
wait 3s
emote pauses a moment to gather his thoughts.
wait 4s
say Ahh, sleep. I think most mages learn the component of this spell at a young age.
wait 2s
chuckle
wait 2s
say Nevertheless, the recital of the sleep spell requires a handful of rose petals, or sand as it's focus.
wait 4s
say They are to be sprinkled over the victim, or at least sprinkled in their direction as the words of the spell are spoken.
wait 4s
say I believe the tower shop carries a supply of them, and of course you can always get them yourself.
end
elseif (%speech% /= scry) then
if (%pcclass% == Mage) and
(%pcalign% >= 0) then
wait 3s
emote pauses a moment to gather his thoughts.
wait 4s
say Scry requires a fragment of map as it's focus, though not just any map will do. It must be an aged or even ancient map, making the component for this spell a bit of a pain.
wait 5s
emote muses to himself for a moment.
wait 3s
say I once saw a pirate with a suitable map..
wait 4s
chuckle
wait 2s
say Nevermind.
end
elseif (%speech% /= prismatic spray) then
if (%pcclass% == Mage) and
(%pcalign% >= 0) then
wait 3s
emote pauses a moment to gather his thoughts.
wait 4s
say The somatic component for the prismatic spray spell is a small prism. These can be found at the tower shop, among other places.
wait 4s
end
else
end
end
end
end
~
#4561
new trigger~
1 j 100
~
if (%actor.vnum% == -1) then
if ("%cmd%" == "wield") then
wait 2s
oecho I am singing.
else
end
~
#4563
m4563 Thief Greet~
0 g 100
~
eval pcclass %actor.class%
if (%pcclass% != Rogue) then
wait 3s
mechoaround %actor% Gueverra turns his attention from the other rogues, looking %actor.alias% over.
msend %actor% Gueverra turns his attention from the other rogues, looking you over.
wait 4s
say Hey, I got nothin to teach you. Go hit the bar and get outta my way.
wait 4s
emote turns back to his students, scolding one of the rogues for making mistakes.
else
wait 3s
mechoaround %actor% Gueverra turns his attention from the other rogues, looking %actor.alias% over.
msend %actor% Gueverra turns his attention from the other rogues, looking you over.
wait 4s
say Hey hey.. welcome in. You need some training? I can teach you the basics well enough.
wait 2s
grin
wait 4s
say If you want advanced training, you'd be better off heading out to Palanthas. The boys out there think they're real slick.
wait 4s
smirk
wait 2s
say Seriously though, those boys'll teach you the arts real well.
wait 3s
msend %actor% Gueverra leans in, whispering to you in a conspiratorial tone, "You can find the entrance to the place in an alley, between Penny and Wall."
mechoaround %actor% Gueverra leans in close to %actor.alias%, whispering something to them.
wait 5s
say Well, anyway.. As I said, I'll teach ya the basics if ya need.
wait 4s
emote stands here, waiting to instruct you.
end
~
$~

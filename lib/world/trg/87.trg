#8723
knightsroad1~
2 g 60
~
if (%actor.vnum%==-1)
wait 1s
wecho A knight clad in full plate charges past, mud flying from the hooves of his powerful steed.
end
~
#8779
arrestingknight~
2 g 60
~
if (%actor.vnum%==-1)
wait 1s
wecho A dangerous looking man leaps out from the side of the road, wildly waving his blade at all that pass.
wload mob 8779
wait 1s
wecho Hearing the commotion, a knight and his squire wheel their horses about, quickly intercepting the brigand.
wload mob 8778
wload mob 8777
wait 2s
wforce bandit curse heavily
wforce heavily say Stand down, brigand!  Surrender your arms and come along peacefully and you will not be harmed.
wforce squire frown bandit
wait 1s
wecho With a loud cry, the dark garbed brigand launches himself at you!
wecho Wheeling suddenly, the knight easily parries and turns the brigands blade, sending it flying from his hands.
wecho Driving in, the knight turns his charger's flanks hard into the man, sending him crashing to the ground, dazed.
wforce heavily say Quickly, squire!  Bind his wrists, remove his weapons!
wait 1s
wforce squire nod
wecho The squire quickly dismounts, rushing to bind the prisoner, gathering his weapons, and securing him for travel.
wait 1s
wforce squire nod
wforce squire say Aye, sir, the prisoner is secured and ready for transport.
wait 1s
wforce heavily nod squire
wforce heavily say Very well.  Let us be on our way.
wait 1s
wecho The knight turns in his saddle, nodding briefly in your direction.
wforce heavily say I thank ye good folks for your patience, travel in safety.
wait 1s
wecho Gathering their bearings once more, the knight and his squire ride off, their prisoner in tow.
wpurge bandit
wpurge heavily
wpurge squire
~
$~

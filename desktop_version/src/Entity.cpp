#include "Entity.h"
#include "Game.h"
#include "Map.h"
#include "UtilityClass.h"

bool entityclass::checktowerspikes(int t)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("checktowerspikes() out-of-bounds!");
		return false;
	}

	tempx = entities[t].xp + entities[t].cx;
	tempy = entities[t].yp + entities[t].cy;
	tempw = entities[t].w;
	temph = entities[t].h;
	rectset(tempx, tempy, tempw, temph);

	tempx = getgridpoint(temprect.x);
	tempy = getgridpoint(temprect.y);
	tempw = getgridpoint(temprect.x + temprect.w - 1);
	temph = getgridpoint(temprect.y + temprect.h - 1);
	if(map.spikecollide(tempx, tempy)) return true;
	if(map.spikecollide(tempw, tempy)) return true;
	if(map.spikecollide(tempx, temph)) return true;
	if(map.spikecollide(tempw, temph)) return true;
	if(temprect.h >= 12) {
		tpy1 = getgridpoint(temprect.y + 6);
		if(map.spikecollide(tempx, tpy1)) return true;
		if(map.spikecollide(tempw, tpy1)) return true;
		if(temprect.h >= 18) {
			tpy1 = getgridpoint(temprect.y + 12);
			if(map.spikecollide(tempx, tpy1)) return true;
			if(map.spikecollide(tempw, tpy1)) return true;
			if(temprect.h >= 24) {
				tpy1 = getgridpoint(temprect.y + 18);
				if(map.spikecollide(tempx, tpy1)) return true;
				if(map.spikecollide(tempw, tpy1)) return true;
			}
		}
	}
	return false;
}

void entityclass::init()
{
	skipblocks = false;
	skipdirblocks = false;
	platformtile = 0;
	customplatformtile = 0;
	vertplatforms = false;
	horplatforms = false;

	nearelephant = false;
	upsetmode = false;
	upset = 0;

	customenemy = 0;
	customwarpmode = false;
	customwarpmodevon = false;
	customwarpmodehon = false;
	trophytext = 0;
	oldtrophytext = 0;
	trophytype = 0;
	altstates = 0;

	SDL_memset(customcrewmoods, true, sizeof(customcrewmoods));

	resetallflags();
	SDL_memset(collect, false, sizeof(collect));
	SDL_memset(coincollect, false, sizeof(coincollect));
	SDL_memset(customcollect, false, sizeof(customcollect));
}

void entityclass::resetallflags()
{
	SDL_memset(flags, false, sizeof(flags));
}

int entityclass::swncolour(int t)
{
	//given colour t, return colour in setcol
	if(t == 0) return 11;
	if(t == 1) return 6;
	if(t == 2) return 8;
	if(t == 3) return 12;
	if(t == 4) return 9;
	if(t == 5) return 7;
	return 0;
}

void entityclass::swnenemiescol(int t)
{
	//change the colour of all SWN enemies to the current one
	for(size_t i = 0; i < entities.size(); i++) {
		if(entities[i].type == 23) {
			entities[i].colour = swncolour(t);
		}
	}
}

void entityclass::gravcreate(int ypos, int dir, int xoff /*= 0*/, int yoff /*= 0*/)
{
	if(dir == 0) {
		createentity(-150 - xoff, 58 + (ypos * 20) + yoff, 23, 0, 0);
	} else {
		createentity(320 + 150 + xoff, 58 + (ypos * 20) + yoff, 23, 1, 0);
	}
}

void entityclass::generateswnwave(int t)
{
	//generate a wave for the SWN game
	if(game.swndelay <= 0) {
		if(t == 0) //game 0, survive for 30 seconds
		{
			switch(game.swnstate) {
				case 0:
					//Decide on a wave here
					//default case
					game.swnstate = 1;
					game.swndelay = 5;

					if(game.swntimer <= 150) //less than 5 seconds
					{
						game.swnstate = 9;
						game.swndelay = 8;
					} else if(game.swntimer <= 300) //less than 10 seconds
					{
						game.swnstate = 6;
						game.swndelay = 12;
					} else if(game.swntimer <= 360) //less than 12 seconds
					{
						game.swnstate = 5 + game.swnstate2;
						game.swndelay = 15;
					} else if(game.swntimer <= 420) //less than 14 seconds
					{
						game.swnstate = 7 + game.swnstate2;
						game.swndelay = 15;
					} else if(game.swntimer <= 480) //less than 16 seconds
					{
						game.swnstate = 5 + game.swnstate2;
						game.swndelay = 15;
					} else if(game.swntimer <= 540) //less than 18 seconds
					{
						game.swnstate = 7 + game.swnstate2;
						game.swndelay = 15;
					} else if(game.swntimer <= 600) //less than 20 seconds
					{
						game.swnstate = 5 + game.swnstate2;
						game.swndelay = 15;
					} else if(game.swntimer <= 900) //less than 30 seconds
					{
						game.swnstate = 4;
						game.swndelay = 20;
					} else if(game.swntimer <= 1050) //less than 35 seconds
					{
						game.swnstate = 3;
						game.swndelay = 10;
					} else if(game.swntimer <= 1200) //less than 40 seconds
					{
						game.swnstate = 3;
						game.swndelay = 20;
					} else if(game.swntimer <= 1500) //less than 50 seconds
					{
						game.swnstate = 2;
						game.swndelay = 10;
					} else if(game.swntimer <= 1650) //less than 55 seconds
					{
						game.swnstate = 1;
						game.swndelay = 15;
					} else if(game.swntimer <= 1800) //less than 60 seconds
					{
						game.swnstate = 1;
						game.swndelay = 25;
					}

					if(game.deathcounts - game.swndeaths > 7) game.swndelay += 2;
					if(game.deathcounts - game.swndeaths > 15) game.swndelay += 2;
					if(game.deathcounts - game.swndeaths > 25) game.swndelay += 4;
					break;
				case 1:
					createentity(-150, 58 + (int(fRandom() * 6) * 20), 23, 0, 0);
					game.swnstate = 0;
					game.swndelay = 0; //return to decision state
					break;
				case 2:
					if(game.swnstate3 == 0) {
						game.swnstate2++;
						if(game.swnstate2 >= 6) {
							game.swnstate3 = 1;
							game.swnstate2--;
						}
					} else {
						game.swnstate2--;
						if(game.swnstate2 < 0) {
							game.swnstate3 = 0;
							game.swnstate2++;
						}
					}
					createentity(-150, 58 + (int(game.swnstate2) * 20), 23, 0, 0);
					game.swnstate = 0;
					game.swndelay = 0; //return to decision state
					break;
				case 3:
					createentity(320 + 150, 58 + (int(fRandom() * 6) * 20), 23, 1, 0);
					game.swnstate = 0;
					game.swndelay = 0; //return to decision state
					break;
				case 4:
					//left and right compliments
					game.swnstate2 = int(fRandom() * 6);
					createentity(-150, 58 + (game.swnstate2 * 20), 23, 0, 0);
					createentity(320 + 150, 58 + ((5 - game.swnstate2) * 20), 23, 1, 0);
					game.swnstate = 0;
					game.swndelay = 0; //return to decision state
					game.swnstate2 = 0;
					break;
				case 5:
					//Top and bottom
					createentity(-150, 58, 23, 0, 0);
					createentity(-150, 58 + (5 * 20), 23, 0, 0);
					game.swnstate = 0;
					game.swndelay = 0; //return to decision state
					game.swnstate2 = 1;
					break;
				case 6:
					//Middle
					createentity(-150, 58 + (2 * 20), 23, 0, 0);
					createentity(-150, 58 + (3 * 20), 23, 0, 0);
					game.swnstate = 0;
					game.swndelay = 0; //return to decision state
					game.swnstate2 = 0;
					break;
				case 7:
					//Top and bottom
					createentity(320 + 150, 58, 23, 1, 0);
					createentity(320 + 150, 58 + (5 * 20), 23, 1, 0);
					game.swnstate = 0;
					game.swndelay = 0; //return to decision state
					game.swnstate2 = 1;
					break;
				case 8:
					//Middle
					createentity(320 + 150, 58 + (2 * 20), 23, 1, 0);
					createentity(320 + 150, 58 + (3 * 20), 23, 1, 0);
					game.swnstate = 0;
					game.swndelay = 0; //return to decision state
					game.swnstate2 = 0;
					break;
				case 9:
					if(game.swnstate3 == 0) {
						game.swnstate2++;
						if(game.swnstate2 >= 6) {
							game.swnstate3 = 1;
							game.swnstate2--;
						}
					} else {
						game.swnstate2--;
						if(game.swnstate2 < 0) {
							game.swnstate3 = 0;
							game.swnstate2++;
						}
					}
					createentity(320 + 150, 58 + (int(game.swnstate2) * 20), 23, 1, 0);
					game.swnstate = 0;
					game.swndelay = 0; //return to decision state
					break;
			}
		} else if(t == 1) {
			//Game 2, super gravitron
			switch(game.swnstate) {
				case 0:
					//Choose either simple or filler
					game.swnstate2 = 0;
					game.swnstate3 = 0;
					game.swnstate4 = 0;

					game.swnstate2 = int(fRandom() * 100);
					if(game.swnstate2 < 25) {
						//simple
						game.swnstate = 2;
						game.swndelay = 0;
					} else {
						//filler
						game.swnstate = 4;
						game.swndelay = 0;
					}
					game.swnstate2 = 0;
					break;
				case 1:
					//complex chain
					game.swnstate2 = int(fRandom() * 8);
					if(game.swnstate2 == 0) {
						game.swnstate = 10;
						game.swndelay = 0;
					} else if(game.swnstate2 == 1) {
						game.swnstate = 12;
						game.swndelay = 0;
					} else if(game.swnstate2 == 2) {
						game.swnstate = 14;
						game.swndelay = 0;
					} else if(game.swnstate2 == 3) {
						game.swnstate = 20;
						game.swndelay = 0;
					} else if(game.swnstate2 == 4) {
						game.swnstate = 21;
						game.swndelay = 0;
					} else if(game.swnstate2 == 5) {
						game.swnstate = 22;
						game.swndelay = 0;
					} else if(game.swnstate2 == 6) {
						game.swnstate = 22;
						game.swndelay = 0;
					} else if(game.swnstate2 == 7) {
						game.swnstate = 14;
						game.swndelay = 0;
					}

					game.swnstate2 = 0;
					break;
				case 2:
					//simple chain
					game.swnstate2 = int(fRandom() * 6);
					if(game.swnstate2 == 0) {
						game.swnstate = 23;
						game.swndelay = 0;
					} else if(game.swnstate2 == 1) {
						game.swnstate = 24;
						game.swndelay = 0;
					} else if(game.swnstate2 == 2) {
						game.swnstate = 25;
						game.swndelay = 0;
					} else if(game.swnstate2 == 3) {
						game.swnstate = 26;
						game.swndelay = 0;
					} else if(game.swnstate2 == 4) {
						game.swnstate = 27;
						game.swndelay = 0;
					} else if(game.swnstate2 == 5) {
						game.swnstate = 14;
						game.swndelay = 0;
					}

					game.swnstate2 = 0;
					break;
				case 3:
					//Choose a major action
					game.swnstate2 = int(fRandom() * 100);
					if(game.swnstate2 < 25) {
						//complex
						game.swnstate = 1;
						game.swndelay = 0;
					} else {
						//simple
						game.swnstate = 2;
						game.swndelay = 0;
					}
					break;
				case 4:
					//filler chain
					game.swnstate2 = int(fRandom() * 6);
					if(game.swnstate2 == 0) {
						game.swnstate = 28;
						game.swndelay = 0;
					} else if(game.swnstate2 == 1) {
						game.swnstate = 29;
						game.swndelay = 0;
					} else if(game.swnstate2 == 2) {
						game.swnstate = 28;
						game.swndelay = 0;
					} else if(game.swnstate2 == 3) {
						game.swnstate = 29;
						game.swndelay = 0;
					} else if(game.swnstate2 == 4) {
						game.swnstate = 30;
						game.swndelay = 0;
					} else if(game.swnstate2 == 5) {
						game.swnstate = 31;
						game.swndelay = 0;
					}

					game.swnstate2 = 0;
					break;
				case 10:
					gravcreate(0, 0);
					gravcreate(1, 0);
					gravcreate(2, 0);
					game.swnstate++;
					game.swndelay = 10; //return to decision state
					break;
				case 11:
					gravcreate(3, 0);
					gravcreate(4, 0);
					gravcreate(5, 0);
					game.swnstate2++;
					if(game.swnstate2 == 3) {
						game.swnstate = 0;
						game.swndelay = 30; //return to decision state
					} else {
						game.swnstate--;
						game.swndelay = 10; //return to decision state
					}
					break;
				case 12:
					gravcreate(0, 1);
					gravcreate(1, 1);
					gravcreate(2, 1);
					game.swnstate++;
					game.swndelay = 10; //return to decision state
					break;
				case 13:
					gravcreate(3, 1);
					gravcreate(4, 1);
					gravcreate(5, 1);
					game.swnstate2++;
					if(game.swnstate2 == 3) {
						game.swnstate = 0;
						game.swndelay = 30; //return to decision state
					} else {
						game.swnstate--;
						game.swndelay = 10; //return to decision state
					}
					break;
				case 14:
					gravcreate(0, 0, 0);
					gravcreate(5, 1, 0);

					game.swnstate++;
					game.swndelay = 20; //return to decision state
					break;
				case 15:
					gravcreate(1, 0);
					gravcreate(4, 1);

					game.swnstate++;
					game.swndelay = 20; //return to decision state
					break;
				case 16:
					gravcreate(2, 0);
					gravcreate(3, 1);

					game.swnstate++;
					game.swndelay = 20; //return to decision state
					break;
				case 17:
					gravcreate(3, 0);
					gravcreate(2, 1);

					game.swnstate++;
					game.swndelay = 20; //return to decision state
					break;
				case 18:
					gravcreate(4, 0);
					gravcreate(1, 1);

					game.swnstate++;
					game.swndelay = 20; //return to decision state
					break;
				case 19:
					gravcreate(5, 0);
					gravcreate(0, 1);

					game.swnstate = 0;
					game.swndelay = 20; //return to decision state
					break;
				case 20:
					game.swnstate4++;
					if(game.swnstate3 == 0) {
						game.swnstate2++;
						if(game.swnstate2 >= 6) {
							game.swnstate3 = 1;
							game.swnstate2--;
						}
					} else {
						game.swnstate2--;
						if(game.swnstate2 < 0) {
							game.swnstate3 = 0;
							game.swnstate2++;
						}
					}
					createentity(-150, 58 + (int(game.swnstate2) * 20), 23, 0, 0);
					if(game.swnstate4 <= 6) {
						game.swnstate = 20;
						game.swndelay = 10; //return to decision state
					} else {
						game.swnstate = 0;
						game.swndelay = 10; //return to decision state
					}
					break;
				case 21:
					game.swnstate4++;
					if(game.swnstate3 == 0) {
						game.swnstate2++;
						if(game.swnstate2 >= 6) {
							game.swnstate3 = 1;
							game.swnstate2--;
						}
					} else {
						game.swnstate2--;
						if(game.swnstate2 < 0) {
							game.swnstate3 = 0;
							game.swnstate2++;
						}
					}
					createentity(320 + 150, 58 + (int(game.swnstate2) * 20), 23, 1, 0);
					if(game.swnstate4 <= 6) {
						game.swnstate = 21;
						game.swndelay = 10; //return to decision state
					} else {
						game.swnstate = 0;
						game.swndelay = 10; //return to decision state
					}
					break;
				case 22:
					game.swnstate4++;
					//left and right compliments
					game.swnstate2 = int(fRandom() * 6);
					createentity(-150, 58 + (game.swnstate2 * 20), 23, 0, 0);
					createentity(320 + 150, 58 + ((5 - game.swnstate2) * 20), 23, 1, 0);
					if(game.swnstate4 <= 12) {
						game.swnstate = 22;
						game.swndelay = 18; //return to decision state
					} else {
						game.swnstate = 0;
						game.swndelay = 18; //return to decision state
					}
					game.swnstate2 = 0;
					break;
				case 23:
					gravcreate(1, 0);
					gravcreate(2, 0, 15);
					gravcreate(2, 0, -15);
					gravcreate(3, 0, 15);
					gravcreate(3, 0, -15);
					gravcreate(4, 0);
					game.swnstate = 0;
					game.swndelay = 15; //return to decision state
					break;
				case 24:
					gravcreate(1, 1);
					gravcreate(2, 1, 15);
					gravcreate(2, 1, -15);
					gravcreate(3, 1, 15);
					gravcreate(3, 1, -15);
					gravcreate(4, 1);
					game.swnstate = 0;
					game.swndelay = 15; //return to decision state
					break;
				case 25:
					gravcreate(0, 0);
					gravcreate(1, 1, 0, 10);
					gravcreate(4, 1, 0, -10);
					gravcreate(5, 0);
					game.swnstate = 0;
					game.swndelay = 20; //return to decision state
					break;
				case 26:
					gravcreate(0, 1, 0);
					gravcreate(1, 1, 10);
					gravcreate(4, 1, 40);
					gravcreate(5, 1, 50);
					game.swnstate = 0;
					game.swndelay = 20; //return to decision state
					break;
				case 27:
					gravcreate(0, 0, 0);
					gravcreate(1, 0, 10);
					gravcreate(4, 0, 40);
					gravcreate(5, 0, 50);
					game.swnstate = 0;
					game.swndelay = 20; //return to decision state
					break;
				case 28:
					game.swnstate4++;
					game.swnstate2 = int(fRandom() * 6);
					createentity(-150, 58 + (game.swnstate2 * 20), 23, 0, 0);
					if(game.swnstate4 <= 6) {
						game.swnstate = 28;
						game.swndelay = 8; //return to decision state
					} else {
						game.swnstate = 3;
						game.swndelay = 15; //return to decision state
					}
					game.swnstate2 = 0;
					break;
				case 29:
					game.swnstate4++;
					game.swnstate2 = int(fRandom() * 6);
					gravcreate(game.swnstate2, 1);
					if(game.swnstate4 <= 6) {
						game.swnstate = 29;
						game.swndelay = 8; //return to decision state
					} else {
						game.swnstate = 3;
						game.swndelay = 15; //return to decision state
					}
					game.swnstate2 = 0;
					break;
				case 30:
					game.swnstate4++;
					game.swnstate2 = int(fRandom() * 3);
					gravcreate(game.swnstate2, 0);
					gravcreate(5 - game.swnstate2, 0);
					if(game.swnstate4 <= 2) {
						game.swnstate = 30;
						game.swndelay = 14; //return to decision state
					} else {
						game.swnstate = 3;
						game.swndelay = 15; //return to decision state
					}
					game.swnstate2 = 0;
					break;
				case 31:
					game.swnstate4++;
					game.swnstate2 = int(fRandom() * 3);
					gravcreate(game.swnstate2, 1);
					gravcreate(5 - game.swnstate2, 1);
					if(game.swnstate4 <= 2) {
						game.swnstate = 31;
						game.swndelay = 14; //return to decision state
					} else {
						game.swnstate = 3;
						game.swndelay = 15; //return to decision state
					}
					game.swnstate2 = 0;
					break;
			}
		}
	} else {
		game.swndelay--;
	}
}

void entityclass::createblock(int t, int xp, int yp, int w, int h, int trig /*= 0*/)
{
	k = blocks.size();

	blockclass block;
	switch(t) {
		case BLOCK: //Block
			block.type = BLOCK;
			block.xp = xp;
			block.yp = yp;
			block.wp = w;
			block.hp = h;
			block.rectset(xp, yp, w, h);
			break;
		case TRIGGER: //Trigger
			// Are we creating a script box with a one-time script?
			// Only abort creation if the script is in the list of already-run one-time scripts
			// AND the script box is a one-time script box
			if(kludgeonetimescript && trig >= 300)
				for(size_t i = 0; i < game.onetimescripts.size(); i++)
					if(game.onetimescripts[i] == game.customscript[trig - 300]) {
						kludgeonetimescript = false;
						return;
					}

			block.type = TRIGGER;
			block.x = xp;
			block.y = yp;
			block.wp = w;
			block.hp = h;
			block.rectset(xp, yp, w, h);
			block.trigger = trig;

			if(kludgeonetimescript) {
				block.onetime = true;
				kludgeonetimescript = false;
			}
			break;
		case DAMAGE: //Damage
			block.type = DAMAGE;
			block.x = xp;
			block.y = yp;
			block.wp = w;
			block.hp = h;
			block.rectset(xp, yp, w, h);
			break;
		case DIRECTIONAL: //Directional
			block.type = DIRECTIONAL;
			block.x = xp;
			block.y = yp;
			block.wp = w;
			block.hp = h;
			block.rectset(xp, yp, w, h);
			block.trigger = trig;
			break;
		case SAFE: //Safe block
			block.type = SAFE;
			block.xp = xp;
			block.yp = yp;
			block.wp = w;
			block.hp = h;
			block.rectset(xp, yp, w, h);
			break;
		case ACTIVITY: //Activity Zone
			block.type = ACTIVITY;
			block.x = xp;
			block.y = yp;
			block.wp = w;
			block.hp = h;
			block.rectset(xp, yp, w, h);

			//Ok, each and every activity zone in the game is initilised here. "Trig" in this case is a variable that
			//assigns all the details.
			switch(trig) {
				case 0: //testing zone
					block.prompt = "Press ENTER to explode";
					block.script = "intro";
					block.setblockcolour("orange");
					trig = 1;
					break;
				case 1:
					block.prompt = "Press ENTER to talk to Violet";
					block.script = "talkpurple";
					block.setblockcolour("purple");
					trig = 0;
					break;
				case 2:
					block.prompt = "Press ENTER to talk to Vitellary";
					block.script = "talkyellow";
					block.setblockcolour("yellow");
					trig = 0;
					break;
				case 3:
					block.prompt = "Press ENTER to talk to Vermilion";
					block.script = "talkred";
					block.setblockcolour("red");
					trig = 0;
					break;
				case 4:
					block.prompt = "Press ENTER to talk to Verdigris";
					block.script = "talkgreen";
					block.setblockcolour("green");
					trig = 0;
					break;
				case 5:
					block.prompt = "Press ENTER to talk to Victoria";
					block.script = "talkblue";
					block.setblockcolour("blue");
					trig = 0;
					break;
				case 6:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_station_1";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 7:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_outside_1";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 8:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_outside_2";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 9:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_outside_3";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 10:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_outside_4";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 11:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_outside_5";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 12:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_outside_6";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 13:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_finallevel";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 14:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_station_2";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 15:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_station_3";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 16:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_station_4";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 17:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_warp_1";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 18:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_warp_2";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 19:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_lab_1";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 20:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_lab_2";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 21:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_secretlab";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 22:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_shipcomputer";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 23:
					block.prompt = "Press ENTER to activate terminals";
					block.script = "terminal_radio";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 24:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "terminal_jukebox";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 25:
					block.prompt = "Passion for Exploring";
					block.script = "terminal_juke1";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 26:
					block.prompt = "Pushing Onwards";
					block.script = "terminal_juke2";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 27:
					block.prompt = "Positive Force";
					block.script = "terminal_juke3";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 28:
					block.prompt = "Presenting VVVVVV";
					block.script = "terminal_juke4";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 29:
					block.prompt = "Potential for Anything";
					block.script = "terminal_juke5";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 30:
					block.prompt = "Predestined Fate";
					block.script = "terminal_juke6";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 31:
					block.prompt = "Pipe Dream";
					block.script = "terminal_juke7";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 32:
					block.prompt = "Popular Potpourri";
					block.script = "terminal_juke8";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 33:
					block.prompt = "Pressure Cooker";
					block.script = "terminal_juke9";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 34:
					block.prompt = "ecroF evitisoP";
					block.script = "terminal_juke10";
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 35:
					block.prompt = "Press ENTER to activate terminal";
					block.script = "custom_" + customscript;
					block.setblockcolour("orange");
					trig = 0;
					break;
				case 100: // Used by customactivityzone() with 1 color argument for predefined colors
					block.prompt = customprompt;
					block.script = "custom_" + customscript;
					block.setblockcolour(customcolour);
					trig = 0;
					break;
				case 101: // Used by customactivityzone() with 3 color arguments for RGB
					block.prompt = customprompt;
					block.script = "custom_" + customscript;
					block.r = customr;
					block.g = customg;
					block.b = customb;
					trig = 0;
					break;
			}
			break;
	}

	kludgeonetimescript = false;
	blocks.push_back(block);
}

// Remove entity, and return true if entity was successfully removed
bool entityclass::removeentity(int t)
{
	if(t < 0 || t > (int) entities.size()) {
		puts("removeentity() out-of-bounds!");
		return true;
	}
	if(entities[t].rule == 0 && t == getplayer()) {
		// Don't remove the player entity!
		return false;
	}
	entities.erase(entities.begin() + t);
	return true;
}

void entityclass::removeallblocks()
{
	blocks.clear();
}

void entityclass::removeallresurrectblocks()
{
	resurrectblocks.clear();
}

void entityclass::removeblock(int t)
{
	if(t < 0 || t > (int) blocks.size()) {
		puts("removeblock() out-of-bounds!");
		return;
	}
	if(blocks[t].type == TRIGGER || blocks[t].type == ACTIVITY) {
		resurrectblocks.push_back(blockclass(blocks[t]));
	}

	blocks.erase(blocks.begin() + t);
}

void entityclass::removeblockat(int x, int y)
{
	for(size_t i = 0; i < blocks.size(); i++) {
		if(blocks[i].xp == int(x) && blocks[i].yp == int(y)) removeblock_iter(i);
	}
}

void entityclass::removetrigger(int t)
{
	bool actuallyonetime = false;
	for(size_t i = 0; i < blocks.size(); i++) {
		if(blocks[i].type == TRIGGER && blocks[i].trigger == t) {
			if(blocks[i].onetime && t >= 300)
				game.onetimescripts.push_back(game.customscript[t - 300]);
			actuallyonetime = blocks[i].onetime;
			removeblock_iter(i);
		}
	}

	// If it is a one-time script box,
	// remove all other script boxes in the room with the same script
	// that are also one-time
	if(kludgeonetimescript && actuallyonetime && t >= 300)
		for(size_t i = 0; i < blocks.size(); i++)
			if(blocks[i].type == TRIGGER && blocks[i].onetime && blocks[i].trigger >= 300 && game.customscript[blocks[i].trigger - 300] == game.customscript[t - 300])
				removeblock(i);

	kludgeonetimescript = false;
}

void entityclass::copylinecross(int t)
{
	if(t < 0 || t > (int) entities.size()) {
		puts("copylinecross() out-of-bounds!");
		return;
	}
	//Copy entity t into the first free linecrosskludge entity
	linecrosskludge.push_back(entities[t]);
}

void entityclass::revertlinecross(int t, int s)
{
	if(t < 0 || t > (int) entities.size() || s < 0 || s > (int) linecrosskludge.size()) {
		puts("revertlinecross() out-of-bounds!");
		return;
	}
	//Restore entity t info from linecrossing s
	entities[t].onentity = linecrosskludge[s].onentity;
	entities[t].state = linecrosskludge[s].state;
	entities[t].life = linecrosskludge[s].life;
}

bool entityclass::gridmatch(int p1, int p2, int p3, int p4, int p11, int p21, int p31, int p41)
{
	if(p1 == p11 && p2 == p21 && p3 == p31 && p4 == p41) return true;
	return false;
}

int entityclass::crewcolour(int t)
{
	//Return the colour of the indexed crewmate
	switch(t) {
		case 0:
			return 0;
			break;
		case 1:
			return 20;
			break;
		case 2:
			return 14;
			break;
		case 3:
			return 15;
			break;
		case 4:
			return 13;
			break;
		case 5:
			return 16;
			break;
	}
	return 0;
}

int entityclass::createentity(float xp, float yp, int t, float vx /*= 0*/, float vy /*= 0*/, int p1 /*= 0*/, int p2 /*= 0*/, int p3 /*= 320*/, int p4 /*= 240 */)
{
	k = entities.size();

	//Size 0 is a sprite
	//Size 1 is a tile
	//Beyond that are special cases (to do)
	//Size 2 is a moving platform of width 4 (32)
	//Size 3 is apparently a "bug chunky pixel"
	//Size 4 is a coin/small pickup
	//Size 5 is a horizontal line, 6 is vertical

	//Rule 0 is the playable character
	//Rule 1 is anything harmful
	//Rule 2 is anything decorative (no collisions)
	//Rule 3 is anything that results in an entity to entity collision and state change
	//Rule 4 is a horizontal line, 5 is vertical
	//Rule 6 is a crew member

#if !defined(NO_CUSTOM_LEVELS)
	// Special case for gray Warp Zone tileset!
	int room = game.roomx - 100 + (game.roomy - 100) * ed.maxwidth;
	bool custom_gray = room >= 0 && room < 400 && ed.level[room].tileset == 3 && ed.level[room].tilecol == 6;
#else
	bool custom_gray = false;
#endif

	entclass entity;
	entity.xp = xp;
	entity.yp = yp;
	entity.oldxp = xp;
	entity.oldyp = yp;
	entity.type = t;
	int thetile = customplatformtile;
	int theroomnum = game.roomx - 100 + ed.maxwidth * (game.roomy - 100);
	// Kludge for platforms/conveyors/quicksand in towers and tower hallways...
	if(theroomnum >= 0 && map.custommode && ed.level[theroomnum].tileset == 5) {
		thetile = ed.gettowerplattile(ed.level[theroomnum].tilecol);

		thetile *= 12;
	}

	switch(t) {
		case 0:              //Player
			entity.rule = 0; //Playable character
			entity.tile = 0;
			entity.colour = 0;
			entity.cx = 6;
			entity.cy = 2;
			entity.w = 12;
			entity.h = 21;
			entity.dir = 1;

			if((vx) == 1) entity.invis = true;

			entity.gravity = true;
			break;
		case 1: //Simple enemy, bouncing off the walls
			entity.rule = 1;
			entity.behave = vx;
			entity.para = vy;
			entity.w = 16;
			entity.h = 16;
			entity.cx = 0;
			entity.cy = 0;

			entity.x1 = p1;
			entity.y1 = p2;
			entity.x2 = p3;
			entity.y2 = p4;

			entity.harmful = true;
			entity.tile = 24;
			entity.animate = 0;
			entity.colour = 8;

			if(game.roomy == 111 && (game.roomx >= 113 && game.roomx <= 117)) {
				entity.setenemy(0);
				entity.setenemyroom(game.roomx, game.roomy); //For colour
			} else if(game.roomx == 113 && (game.roomy <= 110 && game.roomy >= 108)) {
				entity.setenemy(1);
				entity.setenemyroom(game.roomx, game.roomy); //For colour
			} else {
				entity.setenemyroom(game.roomx, game.roomy);
			}
			break;
		case 2: //A moving platform
			entity.rule = 2;
			entity.type = 1;
			entity.size = 2;
			entity.tile = 1;

			if(customplatformtile > 0) {
				entity.tile = thetile;
			} else if(platformtile > 0) {
				entity.tile = platformtile;
			} else {
				//appearance again depends on location
				if(gridmatch(p1, p2, p3, p4, 100, 70, 320, 160)) entity.tile = 616;
				if(gridmatch(p1, p2, p3, p4, 72, 0, 248, 240)) entity.tile = 610;
				if(gridmatch(p1, p2, p3, p4, -20, 0, 320, 240)) entity.tile = 413;

				if(gridmatch(p1, p2, p3, p4, -96, -72, 400, 312)) entity.tile = 26;
				if(gridmatch(p1, p2, p3, p4, -32, -40, 352, 264)) entity.tile = 27;
			}

			entity.w = 32;
			entity.h = 8;

			if(int(vx) <= 1) vertplatforms = true;
			if(int(vx) >= 2 && int(vx) <= 5) horplatforms = true;
			if(int(vx) == 14 || int(vx) == 15) horplatforms = true; //special case for last part of Space Station
			if(int(vx) >= 6 && int(vx) <= 7) vertplatforms = true;

			if(int(vx) >= 10 && int(vx) <= 11) {
				//Double sized threadmills
				entity.w = 64;
				entity.h = 8;
				vx -= 2;
				entity.size = 8;
			}

			entity.behave = vx;
			entity.para = vy;

			if(int(vx) >= 8 && int(vx) <= 9) {
				horplatforms = true; //threadmill!
				entity.animate = 10;
				if(customplatformtile > 0) {
					entity.tile = thetile + 4;
					if(int(vx) == 8) entity.tile += 4;
					if(int(vx) == 9) entity.animate = 11;
				} else {
					entity.settreadmillcolour(game.roomx, game.roomy);
					if(int(vx) == 8) entity.tile += 40;
					if(int(vx) == 9) entity.animate = 11;
				}
			} else {
				entity.animate = 100;
			}

			entity.x1 = p1;
			entity.y1 = p2;
			entity.x2 = p3;
			entity.y2 = p4;

			entity.isplatform = true;

			createblock(0, xp, yp, 32, 8);
			break;
		case 3: //Disappearing platforms
			entity.rule = 3;
			entity.type = 2;
			entity.size = 2;
			entity.tile = 2;
			//appearance again depends on location
			if(customplatformtile > 0) {
				entity.tile = thetile;
			} else if(vx > 0) {
				entity.tile = int(vx);
			} else {
				if(game.roomx == 49 && game.roomy == 52) entity.tile = 18;
				if(game.roomx == 50 && game.roomy == 52) entity.tile = 22;
			}

			entity.cy = -1;
			entity.w = 32;
			entity.h = 10;
			entity.behave = vx;
			entity.para = vy;
			entity.onentity = 1;
			entity.animate = 100;

			createblock(0, xp, yp, 32, 8);
			break;
		case 4: //Breakable blocks
			entity.rule = 6;
			entity.type = 3;
			entity.size = 1;
			entity.tile = 10;
			entity.cy = -1;
			entity.w = 8;
			entity.h = 10;
			entity.behave = vx;
			entity.para = vy;
			entity.onentity = 1;
			entity.animate = 100;

			createblock(0, xp, yp, 8, 8);
			break;
		case 5: //Gravity Tokens
			entity.rule = 3;
			entity.type = 4;
			entity.size = 0;
			entity.tile = 11 + vx;
			entity.w = 16;
			entity.h = 16;
			entity.colour = 0;
			entity.behave = vx;
			entity.onentity = 1;
			entity.animate = 100;
			break;
		case 6: //Decorative particles
			entity.rule = 2;
			entity.type = 5; //Particles
			entity.colour = 1;
			entity.size = 3;
			entity.vx = vx;
			entity.vy = vy;

			entity.life = 12;
			break;
		case 7: //Decorative particles
			entity.rule = 2;
			entity.type = 5; //Particles
			entity.colour = 2;
			entity.size = 3;
			entity.vx = vx;
			entity.vy = vy;

			entity.life = 12;
			break;
		case 8: //Small collectibles
		{
			entity.rule = 3;
			entity.type = 6;
			entity.size = 4;
			entity.tile = 48;
			int temp_size = 8;
			if(vy == 1 || vy == 2) temp_size = 16;
			if(vy == 3 || vy == 4) temp_size = 24;
			entity.w = temp_size;
			entity.h = temp_size;
			entity.onentity = 1;
			entity.animate = 100;
			entity.colour = 201;
			//Check if it's already been collected
			entity.para = vx;
			entity.behave = vy;
			if(!INBOUNDS_ARR(vx, coincollect) || coincollect[(int) vx]) return -1;
		} break;
		case 9: //Something Shiny
			entity.rule = 3;
			entity.type = 7;
			entity.size = 0;
			entity.tile = 22;
			entity.w = 16;
			entity.h = 16;
			entity.colour = 3;
			entity.onentity = 1;
			entity.animate = 100;

			//Check if it's already been collected
			entity.para = vx;
			if(collect[(int) vx]) return -1;
			break;
		case 10: //Savepoint
			entity.rule = 3;
			entity.type = 8;
			entity.size = 0;
			if(vx == 0 || vx == 1)
				entity.tile = 20 + vx;
			else
				entity.tile = 188 + vx;
			entity.w = 16;
			entity.h = 16;
			entity.colour = 4;
			entity.onentity = 1;
			entity.animate = 100;
			entity.para = vy;

			if(game.savepoint == (vy)) {
				entity.colour = 5;
				entity.onentity = 0;
			}

			if(game.nodeathmode) {
				return -1;
			}
			break;
		case 11: //Horizontal Gravity Line
			entity.rule = 4;
			entity.type = 9;
			entity.size = 5;
			entity.life = 0;
			entity.w = vx;
			entity.h = 1;
			entity.onentity = 1;
			break;
		case 12: //Vertical Gravity Line
			entity.rule = 5;
			entity.type = 10;
			entity.size = 6;
			entity.life = 0;
			entity.w = 1;
			entity.h = vx;
			//entity.colour = 0;
			entity.onentity = 1;
			break;
		case 13: //Warp token
			entity.rule = 3;
			entity.type = 11;
			entity.size = 0;
			entity.tile = 18;
			entity.w = 16;
			entity.h = 16;
			entity.colour = 10;
			entity.onentity = 1;
			entity.animate = 2;
			//Added in port, hope it doesn't break anything
			entity.behave = vx;
			entity.para = vy;
			entity.life = p1;
			break;
		case 14: // Teleporter
			entity.rule = 3;
			entity.type = 100;
			entity.size = 7;
			entity.tile = 1; //inactive
			entity.w = 96;
			entity.h = 96;
			entity.colour = 100;
			entity.onentity = 1;
			entity.animate = 100;
			entity.para = vy;
			break;
		case 15: // Crew Member (warp zone)
			entity.rule = 6;
			entity.type = 12; //A special case!
			entity.tile = 144;
			entity.colour = 13; //144 for sad :(
			entity.cx = 6;
			entity.cy = 2;
			entity.w = 12;
			entity.h = 21;
			entity.dir = 0;

			entity.state = vx;

			entity.gravity = true;
			break;
		case 16: // Crew Member, upside down (space station)
			entity.rule = 7;
			entity.type = 12; //A special case!
			entity.tile = 144 + 6;
			entity.colour = 14; //144 for sad (upside down+12):(
			entity.cx = 6;
			entity.cy = 2;
			entity.w = 12;
			entity.h = 21;
			entity.dir = 1;

			entity.state = vx;

			entity.gravity = true;
			break;
		case 17: // Crew Member (Lab)
			entity.rule = 6;
			entity.type = 12; //A special case!
			entity.tile = 144;
			entity.colour = 16; //144 for sad :(
			entity.cx = 6;
			entity.cy = 2;
			entity.w = 12;
			entity.h = 21;
			entity.dir = 1;

			entity.state = vx;

			entity.gravity = true;
			break;
		case 18: // Crew Member (Ship)
		case 57: // Crew Member (Flipped)
			//This is the scriping crewmember
			entity.rule = t == 57 ? 7 : 6;
			entity.type = 12; //A special case!
			entity.colour = vx;
			if(int(vy) == 0) {
				entity.tile = 0;
			} else {
				entity.tile = 144;
			}
			entity.cx = 6;
			entity.cy = 2;
			entity.w = 12;
			entity.h = 21;
			entity.dir = 0;

			entity.state = p1;
			entity.para = p2;

			if(p1 == 17) {
				entity.dir = p2;
			}

			if(entity.rule == 7) entity.tile += 6;

			entity.gravity = true;
			break;
		case 19: // Crew Member (Ship) More tests!
			entity.rule = 6;
			entity.type = 12; //A special case!
			entity.tile = 0;
			entity.colour = 6; //54 for sad :(
			entity.cx = 6;
			entity.cy = 2;
			entity.w = 12;
			entity.h = 21;
			entity.dir = 1;

			entity.state = vx;

			entity.gravity = true;
			break;
		case 20: //Terminal
			entity.rule = 3;
			entity.type = 13;
			entity.size = 0;
			entity.tile = 16 + vx;
			entity.w = 16;
			entity.h = 16;
			entity.colour = 4;
			entity.onentity = 1;
			entity.animate = 100;
			entity.para = vy;
			break;
		case 21: //as above, except doesn't highlight
			entity.rule = 3;
			entity.type = 13;
			entity.size = 0;
			entity.tile = 16 + vx;
			entity.w = 16;
			entity.h = 16;
			entity.colour = 4;
			entity.onentity = 0;
			entity.animate = 100;
			entity.para = vy;
			break;
		case 22: //Fake trinkets, only appear if you've collected them
			entity.rule = 3;
			entity.type = 7;
			entity.size = 0;
			entity.tile = 22;
			entity.w = 16;
			entity.h = 16;
			entity.colour = 3;
			entity.onentity = 0;
			entity.animate = 100;

			//Check if it's already been collected
			entity.para = vx;
			if(INBOUNDS_ARR(vx, collect) && !collect[(int) vx]) return -1;
			break;
		case 23: //SWN Enemies
			//Given a different behavior, these enemies are especially for SWN mode and disappear outside the screen.
			entity.rule = 1;
			entity.type = 23;
			entity.behave = vx;
			entity.para = vy;
			entity.w = 16;
			entity.h = 16;
			entity.cx = 0;
			entity.cy = 0;

			entity.x1 = -2000;
			entity.y1 = -100;
			entity.x2 = 5200;
			entity.y2 = 340;

			entity.harmful = true;

			//initilise tiles here based on behavior
			entity.size = 12; //don't wrap around
			entity.colour = 21;
			entity.tile = 78; //default case
			entity.animate = 1;
			if(game.swngame == 1) {
				//set colour based on current state
				entity.colour = swncolour(game.swncolstate);
			}
			break;
		case 24: // Super Crew Member
			//This special crewmember is way more advanced than the usual kind, and can interact with game objects
			entity.rule = 6;
			entity.type = 14; //A special case!
			entity.colour = vx;
			if((vx) == 16) {
				//victoria is sad!
				if(int(vy) == 2) vy = 1;
			} else {
				if(int(vy) == 2) vy = 0;
			}
			if(int(vy) == 0) {
				entity.tile = 0;
			} else {
				entity.tile = 144;
			}
			entity.cx = 6;
			entity.cy = 2;
			entity.w = 12;
			entity.h = 21;
			entity.dir = 1;

			entity.x1 = -2000;
			entity.y1 = -100;
			entity.x2 = 5200;
			entity.y2 = 340;

			entity.state = p1;
			entity.para = p2;

			if(p1 == 17) {
				entity.dir = p2;
			}

			entity.gravity = true;
			break;
		case 25: //Trophies
			entity.rule = 3;
			entity.type = 15;
			entity.size = 0;
			entity.w = 16;
			entity.h = 16;
			entity.colour = 4;
			entity.onentity = 1;
			entity.animate = 100;
			entity.para = vy;

			//Decide tile here based on given achievement: both whether you have them and what they are
			//default is just a trophy base:
			entity.tile = 180 + vx;
			switch(int(vy)) {
				case 1:
					if(game.bestrank[0] >= 3) {
						entity.tile = 184 + vx;
						entity.colour = 31;
					}
					break;
				case 2:
					if(game.bestrank[1] >= 3) {
						entity.tile = 186 + vx;
						entity.colour = 35;
					}
					break;
				case 3:
					if(game.bestrank[2] >= 3) {
						entity.tile = 184 + vx;
						entity.colour = 33;
					}
					break;
				case 4:
					if(game.bestrank[3] >= 3) {
						entity.tile = 184 + vx;
						entity.colour = 32;
					}
					break;
				case 5:
					if(game.bestrank[4] >= 3) {
						entity.tile = 184 + vx;
						entity.colour = 34;
					}
					break;
				case 6:
					if(game.bestrank[5] >= 3) {
						entity.tile = 184 + vx;
						entity.colour = 30;
					}
					break;

				case 7:
					if(game.unlock[5]) {
						entity.tile = 188 + vx;
						entity.colour = 37;
						entity.h += 3;
					}
					break;
				case 8:
					if(game.unlock[19]) {
						entity.tile = 188 + vx;
						entity.colour = 37;
						entity.h += 3;
					}
					break;

				case 9:
					if(game.bestgamedeaths > -1) {
						if(game.bestgamedeaths <= 50) {
							entity.tile = 182 + vx;
							entity.colour = 40;
						}
					}
					break;
				case 10:
					if(game.bestgamedeaths > -1) {
						if(game.bestgamedeaths <= 100) {
							entity.tile = 182 + vx;
							entity.colour = 36;
						}
					}
					break;
				case 11:
					if(game.bestgamedeaths > -1) {
						if(game.bestgamedeaths <= 250) {
							entity.tile = 182 + vx;
							entity.colour = 38;
						}
					}
					break;
				case 12:
					if(game.bestgamedeaths > -1) {
						if(game.bestgamedeaths <= 500) {
							entity.tile = 182 + vx;
							entity.colour = 39;
						}
					}
					break;

				case 13:
					if(game.swnbestrank >= 1) {
						entity.tile = 182 + vx;
						entity.colour = 39;
					}
					break;
				case 14:
					if(game.swnbestrank >= 2) {
						entity.tile = (182 + vx);
						entity.colour = 39;
					}
					break;
				case 15:
					if(game.swnbestrank >= 3) {
						entity.tile = (182 + vx);
						entity.colour = 39;
					}
					break;
				case 16:
					if(game.swnbestrank >= 4) {
						entity.tile = (182 + vx);
						entity.colour = 38;
					}
					break;
				case 17:
					if(game.swnbestrank >= 5) {
						entity.tile = (182 + vx);
						entity.colour = 36;
					}
					break;
				case 18:
					if(game.swnbestrank >= 6) {
						entity.tile = (182 + vx);
						entity.colour = 40;
					}
					break;

				case 19:
					if(game.unlock[20]) {
						entity.tile = 3;
						entity.colour = 102;
						entity.size = 13;
						entity.xp -= 64;
						entity.yp -= 128;
					}
					break;
			}

			break;
		case 26: //Epilogue super warp token
			entity.rule = 3;
			entity.type = 11;
			entity.size = 0;
			entity.tile = 18;
			entity.w = 16;
			entity.h = 16;
			entity.colour = 3;
			entity.onentity = 0;
			entity.animate = 100;
			entity.para = vy;
			entity.size = 13;
			break;

		case 51: //Vertical Warp Line
			entity.rule = 5;
			entity.type = 51;
			entity.size = 6;
			entity.life = 0;
			entity.w = 1;
			entity.h = vx;
			//entity.colour = 0;
			entity.onentity = 1;
			entity.invis = true;
			if(map.custommode) customwarpmode = true;
			break;
		case 52: //Vertical Warp Line
			entity.rule = 5;
			entity.type = 52;
			entity.size = 6;
			entity.life = 0;
			entity.w = 1;
			entity.h = vx;
			//entity.colour = 0;
			entity.onentity = 1;
			entity.invis = true;
			if(map.custommode) customwarpmode = true;
			break;
		case 53: //Horizontal Warp Line
			entity.rule = 7;
			entity.type = 53;
			entity.size = 5;
			entity.life = 0;
			entity.w = vx;
			entity.h = 1;
			entity.onentity = 1;
			entity.invis = true;
			if(map.custommode) customwarpmode = true;
			break;
		case 54: //Horizontal Warp Line
			entity.rule = 7;
			entity.type = 54;
			entity.size = 5;
			entity.life = 0;
			entity.w = vx;
			entity.h = 1;
			entity.onentity = 1;
			entity.invis = true;
			if(map.custommode) customwarpmode = true;
			break;
		case 55: // Crew Member (custom, collectable)
			//1 - position in array
			//2 - colour
			entity.rule = 3;
			entity.type = 55;
			if(customcrewmoods[int(vy)] == 1) {
				entity.tile = 144;
			} else {
				entity.tile = 0;
			}
			entity.colour = crewcolour(int(vy));
			entity.cx = 6;
			entity.cy = 2;
			entity.w = 12;
			entity.h = 21;
			entity.dir = 0;

			entity.state = 0;
			entity.onentity = 1;
			//entity.state = vx;

			entity.gravity = true;

			//Check if it's already been collected
			entity.para = vx;
			if(!INBOUNDS_ARR(vx, customcollect) || customcollect[(int) vx]) return -1;
			break;
		case 56: //Custom enemy
			entity.rule = 1;
			entity.type = 1;
			entity.behave = vx;
			entity.para = vy;
			entity.w = 16;
			entity.h = 16;
			entity.cx = 0;
			entity.cy = 0;

			entity.x1 = p1;
			entity.y1 = p2;
			entity.x2 = p3;
			entity.y2 = p4;

			entity.harmful = true;

			switch(customenemy) {
				case 0:
					entity.setenemyroom(4 + 100, 0 + 100);
					break;
				case 1:
					entity.setenemyroom(2 + 100, 0 + 100);
					break;
				case 2:
					entity.setenemyroom(12 + 100, 3 + 100);
					break;
				case 3:
					entity.setenemyroom(13 + 100, 12 + 100);
					break;
				case 4:
					entity.setenemyroom(16 + 100, 9 + 100);
					break;
				case 5:
					entity.setenemyroom(19 + 100, 1 + 100);
					break;
				case 6:
					entity.setenemyroom(19 + 100, 2 + 100);
					break;
				case 7:
					entity.setenemyroom(18 + 100, 3 + 100);
					break;
				case 8:
					entity.setenemyroom(16 + 100, 0 + 100);
					break;
				case 9:
					entity.setenemyroom(14 + 100, 2 + 100);
					break;
				case 10:
					entity.setenemyroom(10 + 100, 7 + 100);
					break;

				case 11:
					entity.setenemyroom(12 + 100, 5 + 100);
					break; // yes man
				case 12:
					entity.setenemyroom(15 + 100, 3 + 100);
					break; // STOP
				case 13:
					entity.setenemyroom(13 + 100, 3 + 100);
					break; // wave duude
				case 14:
					entity.setenemyroom(15 + 100, 2 + 100);
					break; // numbers
				case 15:
					entity.setenemyroom(16 + 100, 2 + 100);
					break; // the dudes that walk
				case 16:
					entity.setenemyroom(18 + 100, 2 + 100);
					break; // boob
				case 17:
					entity.setenemyroom(18 + 100, 0 + 100);
					break; // OBEY
				case 18:
					entity.setenemyroom(17 + 100, 3 + 100);
					break; // edge games
				case 19:
					entity.setenemyroom(13 + 100, 6 + 100);
					break; // sent over the bottom gottem lmao
				case 20:
					entity.setenemyroom(16 + 100, 7 + 100);
					break; // ghos
				case 21:
					entity.setenemyroom(17 + 100, 7 + 100);
					break; // they b walkin 2.0
				case 22:
					entity.setenemyroom(14 + 100, 8 + 100);
					break; // what the fuck is this
				case 23:
					entity.setenemyroom(11 + 100, 13 + 100);
					break; // TRUTH
				case 24:
					entity.setenemyroom(14 + 100, 13 + 100);
					break; // DABBING SKELETON
				case 25:
					entity.setenemyroom(44, 51);
					break; // enemy in vertigo
				case 26:
					entity.tile = 24;
					entity.animate = 0;
					break;
				case 27:
					entity.setenemyroom(13 + 100, 7 + 100);
					break; // bus
				default:
					entity.setenemyroom(4 + 100, 0 + 100);
					break;
			}

			//Set colour based on room tile
			//Set custom colours
			if(customplatformtile > 0) {
				int entcol = (customplatformtile / 12);
				switch(entcol) {
					//RED
					case 3:
					case 7:
					case 12:
					case 23:
					case 28:
					case 34:
					case 42:
					case 48:
					case 58:
					case 59:
						entity.colour = 6;
						break;
					//GREEN
					case 5:
					case 9:
					case 22:
					case 25:
					case 29:
					case 31:
					case 38:
					case 46:
					case 52:
					case 53:
					case 61:
						entity.colour = 7;
						break;
					//BLUE
					case 1:
					case 6:
					case 14:
					case 27:
					case 33:
					case 44:
					case 50:
					case 57:
						entity.colour = 12;
						break;
					//YELLOW
					case 4:
					case 17:
					case 24:
					case 30:
					case 37:
					case 45:
					case 51:
					case 55:
					case 60:
						entity.colour = 9;
						break;
					//PURPLE
					case 2:
					case 11:
					case 15:
					case 19:
					case 32:
					case 36:
					case 49:
					case 63:
						entity.colour = 20;
						break;
					//CYAN
					case 8:
					case 10:
					case 13:
					case 18:
					case 26:
					case 35:
					case 41:
					case 47:
					case 54:
					case 62:
						entity.colour = 11;
						break;
					//PINK
					case 16:
					case 20:
					case 39:
					case 43:
					case 56:
					case 64:
						entity.colour = 8;
						break;
					//ORANGE
					case 21:
					case 40:
						entity.colour = 17;
						break;
					default:
						entity.colour = 6;
						break;
				}
			}

			if(custom_gray) {
				entity.colour = 18;
			}

			break;
	}

	entity.drawframe = entity.tile;
	if(!entity.invis) {
		entity.updatecolour();
	}
	if(entity.type == 1) {
		switch(entity.animate) {
			case 0:  // Simple Loop
			case 1:  // Simple Loop
			case 2:  // Simpler Loop (just two frames)
			case 5:  // Simpler Loop (just two frames) (slower)
			case 7:  // Simpler Loop (just two frames) (slower) (with directions!)
			case 11: // Conveyor right
				entity.drawframe++;
				break;
			case 3: // Simpler Loop (just two frames, but double sized)
			case 4: // Simpler Loop (just two frames, but double sized) (as above, but slower)
			case 6: // Normal Loop (four frames, double sized)
				entity.drawframe += 2;
				break;
			case 10: // Conveyor left
				entity.drawframe += 3;
				break;
			default:
				break;
		}
	} else if(entity.type == 2 && entity.animate == 2) {
		entity.drawframe++;
	}

	entities.push_back(entity);
	return k;
}

//Returns true if entity is removed
bool entityclass::updateentities(int i)
{
	if(i < 0 || i >= (int) entities.size()) {
		puts("updateentities() out-of-bounds!");
		return true;
	}

	if(entities[i].statedelay <= 0) {
		switch(entities[i].type) {
			case 0: //Player
				break;
			case 1: //Movement behaviors
				//Enemies can have a number of different behaviors:
				switch(entities[i].behave) {
					case 0:                        //Bounce, Start moving down
						if(entities[i].state == 0) //Init
						{
							entities[i].state = 3;
							bool entitygone = updateentities(i);
							if(entitygone) return true;
						} else if(entities[i].state == 1) {
							if(entities[i].outside()) entities[i].state = entities[i].onwall;
						} else if(entities[i].state == 2) {
							entities[i].vy = -entities[i].para;
							entities[i].onwall = 3;
							entities[i].state = 1;
						} else if(entities[i].state == 3) {
							entities[i].vy = entities[i].para;
							entities[i].onwall = 2;
							entities[i].state = 1;
						}
						break;
					case 1:                        //Bounce, Start moving up
						if(entities[i].state == 0) //Init
						{
							entities[i].state = 2;
							bool entitygone = updateentities(i);
							if(entitygone) return true;
						} else if(entities[i].state == 1) {
							if(entities[i].outside()) entities[i].state = entities[i].onwall;
						} else if(entities[i].state == 2) {
							entities[i].vy = -entities[i].para;
							entities[i].onwall = 3;
							entities[i].state = 1;
						} else if(entities[i].state == 3) {
							entities[i].vy = entities[i].para;
							entities[i].onwall = 2;
							entities[i].state = 1;
						}
						break;
					case 2:                        //Bounce, Start moving left
						if(entities[i].state == 0) //Init
						{
							entities[i].state = 3;
							bool entitygone = updateentities(i);
							if(entitygone) return true;
						} else if(entities[i].state == 1) {
							if(entities[i].outside()) entities[i].state = entities[i].onwall;
						} else if(entities[i].state == 2) {
							entities[i].vx = entities[i].para;
							entities[i].onwall = 3;
							entities[i].state = 1;
						} else if(entities[i].state == 3) {
							entities[i].vx = -entities[i].para;
							entities[i].onwall = 2;
							entities[i].state = 1;
						}
						break;
					case 3:                        //Bounce, Start moving right
						if(entities[i].state == 0) //Init
						{
							entities[i].state = 3;
							bool entitygone = updateentities(i);
							if(entitygone) return true;
						} else if(entities[i].state == 1) {
							if(entities[i].outside()) entities[i].state = entities[i].onwall;
						} else if(entities[i].state == 2) {
							entities[i].vx = -entities[i].para;
							entities[i].onwall = 3;
							entities[i].state = 1;
						} else if(entities[i].state == 3) {
							entities[i].vx = entities[i].para;
							entities[i].onwall = 2;
							entities[i].state = 1;
						}
						break;
					case 4:                        //Always move left
						if(entities[i].state == 0) //Init
						{
							entities[i].vx = entities[i].para;
						}
						break;
					case 5: //Always move right
						if(entities[i].state == 0) {
							//Init
							entities[i].vx = static_cast<int>(entities[i].para);
							entities[i].state = 1;
							entities[i].onwall = 2;
						} else if(entities[i].state == 2) {
							entities[i].vx = 0;
							entities[i].onwall = 0;
							entities[i].xp -= static_cast<int>(entities[i].para);
							entities[i].statedelay = 8;
							entities[i].state = 0;
						}
						break;
					case 6:                        //Always move up
						if(entities[i].state == 0) //Init
						{
							entities[i].vy = static_cast<int>(entities[i].para);
							entities[i].state = 1;
							entities[i].onwall = 2;
						} else if(entities[i].state == 2) {
							entities[i].vy = static_cast<int>(-entities[i].para);
							entities[i].onwall = 0;
							entities[i].yp -= (entities[i].para);
							entities[i].statedelay = 8;
							entities[i].state = 0;
						}
						break;
					case 7:                        //Always move down
						if(entities[i].state == 0) //Init
						{
							entities[i].vx = static_cast<int>(entities[i].para);
						}
						break;
					case 8:
					case 9:
						//Threadmill: don't move, just impart velocity
						if(entities[i].state == 0) //Init
						{
							entities[i].vx = 0;
							entities[i].state = 1;
							entities[i].onwall = 0;
						}
						break;
					case 10:
						//Emitter: shoot an enemy every so often
						if(entities[i].state == 0) {
							createentity(entities[i].xp + 28, entities[i].yp, 1, 10, 1);
							entities[i].state = 1;
							entities[i].statedelay = 12;
						} else if(entities[i].state == 1) {
							entities[i].state = 0;
						}
						break;
					case 11:                       //Always move right, destroy when outside screen
						if(entities[i].state == 0) //Init
						{
							entities[i].vx = entities[i].para;
							entities[i].state = 1;
						} else if(entities[i].state == 1) {
							if(entities[i].xp >= 335) {
								return removeentity(i);
							}
							if(game.roomx == 117) {
								if(entities[i].xp >= (33 * 8) - 32) {
									return removeentity(i);
								}
								//collector for LIES
							}
						}
						break;
					case 12:
						//Emitter: shoot an enemy every so often (up)
						if(entities[i].state == 0) {
							createentity(entities[i].xp, entities[i].yp, 1, 12, 1);
							entities[i].state = 1;
							entities[i].statedelay = 16;
						} else if(entities[i].state == 1) {
							entities[i].state = 0;
						}
						break;
					case 13:                       //Always move up, destroy when outside screen
						if(entities[i].state == 0) //Init
						{
							entities[i].vy = entities[i].para;
							entities[i].state = 1;
						} else if(entities[i].state == 1) {
							if(entities[i].yp <= -60) {
								return removeentity(i);
							}
							if(game.roomy == 108) {
								if(entities[i].yp <= 60) {
									return removeentity(i);
								}
								//collector for factory
							}
						}
						break;
					case 14:                       //Very special hack: as two, but doesn't move in specific circumstances
						if(entities[i].state == 0) //Init
						{
							for(size_t j = 0; j < entities.size(); j++) {
								if(entities[j].type == 2 && entities[j].state == 3 && entities[j].xp == (entities[i].xp - 32)) {
									entities[i].state = 3;
									bool entitygone = updateentities(i);
									if(entitygone) return true;
								}
							}
						} else if(entities[i].state == 1) {
							if(entities[i].outside()) entities[i].state = entities[i].onwall;
						} else if(entities[i].state == 2) {
							entities[i].vx = entities[i].para;
							entities[i].onwall = 3;
							entities[i].state = 1;
						} else if(entities[i].state == 3) {
							entities[i].vx = -entities[i].para;
							entities[i].onwall = 2;
							entities[i].state = 1;
						}
						break;
					case 15:                       //As above, but for 3!
						if(entities[i].state == 0) //Init
						{
							for(size_t j = 0; j < entities.size(); j++) {
								if(entities[j].type == 2 && entities[j].state == 3 && entities[j].xp == entities[i].xp + 32) {
									entities[i].state = 3;
									bool entitygone = updateentities(i);
									if(entitygone) return true;
								}
							}
						} else if(entities[i].state == 1) {
							if(entities[i].outside()) entities[i].state = entities[i].onwall;
						} else if(entities[i].state == 2) {
							entities[i].vx = -entities[i].para;
							entities[i].onwall = 3;
							entities[i].state = 1;
						} else if(entities[i].state == 3) {
							entities[i].vx = entities[i].para;
							entities[i].onwall = 2;
							entities[i].state = 1;
						}
						break;
					case 16:                       //MAVERICK BUS FOLLOWS HIS OWN RULES
						if(entities[i].state == 0) //Init
						{
							int player = getplayer();
							//first, y position
							if(player > -1 && entities[player].yp > 14 * 8) {
								entities[i].tile = 120;
								entities[i].yp = (28 * 8) - 62;
								entities[i].oldyp = (28 * 8) - 62;
							} else {
								entities[i].tile = 96;
								entities[i].yp = 24;
								entities[i].oldyp = 24;
							}
							//now, x position
							if(player > -1 && entities[player].xp > 20 * 8) {
								//approach from the left
								entities[i].xp = -64;
								entities[i].oldxp = -64;
								entities[i].state = 2;
								bool entitygone = updateentities(i); //right
								if(entitygone) return true;
							} else {
								//approach from the left
								entities[i].xp = 320;
								entities[i].oldxp = 320;
								entities[i].state = 3;
								bool entitygone = updateentities(i); //left
								if(entitygone) return true;
							}

						} else if(entities[i].state == 1) {
							if(entities[i].outside()) entities[i].state = entities[i].onwall;
						} else if(entities[i].state == 2) {
							entities[i].vx = int(entities[i].para);
							entities[i].onwall = 3;
							entities[i].state = 1;
						} else if(entities[i].state == 3) {
							entities[i].vx = int(-entities[i].para);
							entities[i].onwall = 2;
							entities[i].state = 1;
						}
						break;
					case 17:                       //Special for ASCII Snake (left)
						if(entities[i].state == 0) //Init
						{
							entities[i].statedelay = 6;
							entities[i].xp -= int(entities[i].para);
							entities[i].oldxp -= int(entities[i].para);
						}
						break;
					case 18:                       //Special for ASCII Snake (right)
						if(entities[i].state == 0) //Init
						{
							entities[i].statedelay = 6;
							entities[i].xp += int(entities[i].para);
							entities[i].oldxp += int(entities[i].para);
						}
						break;
				}
				break;
			case 2: //Disappearing platforms
				//wait for collision
				if(entities[i].state == 1) {
					entities[i].life = 12;
					entities[i].state = 2;
					entities[i].onentity = 0;

					music.playef(7);
				} else if(entities[i].state == 2) {
					entities[i].life--;
					if(entities[i].life % 3 == 0) entities[i].tile++;
					if(entities[i].life <= 0) {
						removeblockat(entities[i].xp, entities[i].yp);
						entities[i].state = 3; // = false;
						entities[i].invis = true;
					}
				} else if(entities[i].state == 3) {
					//wait until recharged!
				} else if(entities[i].state == 4) {
					//restart!
					createblock(0, entities[i].xp, entities[i].yp, 32, 8);
					entities[i].state = 4;
					entities[i].invis = false;
					entities[i].tile--;
					entities[i].state++;
					entities[i].onentity = 1;
				} else if(entities[i].state == 5) {
					entities[i].life += 3;
					if(entities[i].life % 3 == 0) entities[i].tile--;
					if(entities[i].life >= 12) {
						entities[i].life = 12;
						entities[i].state = 0;
						entities[i].tile++;
					}
				}
				break;
			case 3: //Breakable blocks
				//Only counts if vy of player entity is non zero
				if(entities[i].state == 1) {
					entities[i].life = 4;
					entities[i].state = 2;
					entities[i].onentity = 0;
					music.playef(6);
				} else if(entities[i].state == 2) {
					entities[i].life--;
					entities[i].tile++;
					if(entities[i].life <= 0) {
						removeblockat(entities[i].xp, entities[i].yp);
						if(IS_VCE_LEVEL) {
							entities[i].state = 3;
							entities[i].invis = true;
						} else {
							return removeentity(i);
						}
					}
				} else if(entities[i].state == 3) {
					// Wait until recharged!
				} else if(entities[i].state == 4) {
					// Restart!
					createblock(BLOCK, entities[i].xp, entities[i].yp, 8, 8);
					entities[i].state = 4;
					entities[i].invis = false;
					entities[i].tile--;
					entities[i].state = 5;
					entities[i].onentity = 1;
				} else if(entities[i].state == 5) {
					entities[i].life++;
					entities[i].tile--;
					if(entities[i].life >= 4) {
						entities[i].life = 4;
						entities[i].state = 0;
						entities[i].tile++;
					}
				}
				break;
			case 4: //Gravity token
				//wait for collision
				if(entities[i].state == 1) {
					game.gravitycontrol = (game.gravitycontrol + 1) % 2;
					if(IS_VCE_LEVEL) {
						music.playef(8);
						// Remove it, but respawn it upon death
						entities[i].invis = true;
						entities[i].state = 2;
						// Removes collision
						entities[i].onentity = 0;
					} else {
						return removeentity(i);
					}
				} else if(entities[i].state == 2) {
					// Wait until recharged!
				} else if(entities[i].state == 3) {
					// Respawn
					entities[i].state = 0;
					entities[i].invis = false;
					entities[i].onentity = 1;
					// Spin really fast for half a second!
					entities[i].life = 4;
				}
				break;
			case 5: //Particle sprays
				if(entities[i].state == 0) {
					entities[i].life--;
					if(entities[i].life < 0) {
						return removeentity(i);
					}
				}
				break;
			case 6: //Small pickup
				//wait for collision
				if(entities[i].state == 1) {
					if(entities[i].behave == 0) game.coins++;
					if(entities[i].behave == 1) game.coins += 10;
					if(entities[i].behave == 2) game.coins += 20;
					if(entities[i].behave == 3) game.coins += 50;
					if(entities[i].behave == 4) game.coins += 100;
					music.playef(4);
					coincollect[(int) entities[i].para] = true;

					return removeentity(i);
				}
				break;
			case 7: //Found a trinket
				//wait for collision
				if(entities[i].state == 1) {
					if(INBOUNDS_ARR(entities[i].para, collect)) {
						collect[(int) entities[i].para] = true;
					}

					if(game.intimetrial) {
						music.playef(25);
					} else {
						game.state = 1000;
						music.silencedasmusik();
						music.playef(3);
						if(game.trinkets() > game.stat_trinkets && !map.custommode) {
							game.stat_trinkets = game.trinkets();
						}
					}

					return removeentity(i);
				}
				break;
			case 8: //Savepoints
				//wait for collision
				if(entities[i].state == 1) {
					//First, deactivate all other savepoints
					for(size_t j = 0; j < entities.size(); j++) {
						if(entities[j].type == 8) {
							entities[j].colour = 4;
							entities[j].onentity = 1;
						}
					}
					entities[i].colour = 5;
					entities[i].onentity = 0;
					game.savepoint = entities[i].para;
					music.playef(5);

					game.savex = entities[i].xp - 4;

					if(entities[i].tile == 20) {
						game.savey = entities[i].yp - 1;
						game.savegc = 1;
					} else if(entities[i].tile == 21) {
						game.savey = entities[i].yp - 8;
						game.savegc = 0;
					} else if(entities[i].tile == 190) {
						game.savey = entities[i].yp - 4;
						game.savegc = 0;
					} else if(entities[i].tile == 191) {
						game.savey = entities[i].yp - 4;
						game.savegc = 1;
					}

					game.saverx = game.roomx;
					game.savery = game.roomy;
					int player = getplayer();
					if(player > -1) {
						game.savedir = entities[player].dir;
					}
					entities[i].state = 0;
				}
				break;
			case 9: //Gravity Lines
				if(entities[i].state == 1) {
					entities[i].life--;
					entities[i].onentity = 0;

					if(entities[i].life <= 0) {
						entities[i].state = 0;
						entities[i].onentity = 1;
					}
				}
				break;
			case 10: //Vertical gravity Lines
				if(entities[i].state == 1) {
					entities[i].onentity = 3;
					entities[i].state = 2;

					music.playef(8);
					game.gravitycontrol = (game.gravitycontrol + 1) % 2;
					game.totalflips++;
					temp = getplayer();
					if(game.gravitycontrol == 0) {
						if(temp > -1 && entities[temp].vy < 3) entities[temp].vy = 3;
					} else {
						if(temp > -1 && entities[temp].vy > -3) entities[temp].vy = -3;
					}
				} else if(entities[i].state == 2) {
					entities[i].life--;
					if(entities[i].life <= 0) {
						entities[i].state = 0;
						entities[i].onentity = 1;
					}
				} else if(entities[i].state == 3) {
					entities[i].state = 2;
					entities[i].life = 4;
					entities[i].onentity = 3;
				} else if(entities[i].state == 4) {
					//Special case for room initilisations: As state one, except without the reversal
					entities[i].onentity = 3;
					entities[i].state = 2;
				}
				break;
			case 11: //Warp point
				//wait for collision
				if(entities[i].state == 1) {
					//Depending on the room the warp point is in, teleport to a new location!
					entities[i].onentity = 0;
					//play a sound or somefink
					music.playef(10);
					game.teleport = true;

					game.edteleportent = i;
					//for the multiple room:
					if(int(entities[i].xp) == 12 * 8) game.teleportxpos = 1;
					if(int(entities[i].xp) == 5 * 8) game.teleportxpos = 2;
					if(int(entities[i].xp) == 28 * 8) game.teleportxpos = 3;
					if(int(entities[i].xp) == 21 * 8) game.teleportxpos = 4;
				}
				break;
			case 12: //Crew member
				//Somewhat complex AI: exactly what they do depends on room, location, state etc
				//At state 0, do nothing at all.
				if(entities[i].state == 1) {
					//happy!
					if(entities[k].rule == 6) entities[k].tile = 0;
					if(entities[k].rule == 7) entities[k].tile = 6;
					//Stay close to the hero!
					int j = getplayer();
					if(j > -1 && entities[j].xp > entities[i].xp + 5) {
						entities[i].dir = 1;
					} else if(j > -1 && entities[j].xp < entities[i].xp - 5) {
						entities[i].dir = 0;
					}

					if(j > -1 && entities[j].xp > entities[i].xp + 45) {
						entities[i].ax = 3;
					} else if(j > -1 && entities[j].xp < entities[i].xp - 45) {
						entities[i].ax = -3;
					}

					//Special rules:
					if(game.roomx == 110 && game.roomy == 105) {
						if(entities[i].xp < 155) {
							if(entities[i].ax < 0) entities[i].ax = 0;
						}
					}
				} else if(entities[i].state == 2) {
					//Basic rules, don't change expression
					int j = getplayer();
					if(j > -1 && entities[j].xp > entities[i].xp + 5) {
						entities[i].dir = 1;
					} else if(j > -1 && entities[j].xp < entities[i].xp - 5) {
						entities[i].dir = 0;
					}

					if(j > -1 && entities[j].xp > entities[i].xp + 45) {
						entities[i].ax = 3;
					} else if(j > -1 && entities[j].xp < entities[i].xp - 45) {
						entities[i].ax = -3;
					}
				} else if(entities[i].state == 10) {
					//Everything from 10 on is for cutscenes
					//Basic rules, don't change expression
					int j = getplayer();
					if(j > -1 && entities[j].xp > entities[i].xp + 5) {
						entities[i].dir = 1;
					} else if(j > -1 && entities[j].xp < entities[i].xp - 5) {
						entities[i].dir = 0;
					}

					if(j > -1 && entities[j].xp > entities[i].xp + 45) {
						entities[i].ax = 3;
					} else if(j > -1 && entities[j].xp < entities[i].xp - 45) {
						entities[i].ax = -3;
					}
				} else if(entities[i].state == 11) {
					//11-15 means to follow a specific character, in crew order (cyan, purple, yellow, red, green, blue)
					int j = getcrewman(1); //purple
					if(entities[j].xp > entities[i].xp + 5) {
						entities[i].dir = 1;
					} else if(entities[j].xp < entities[i].xp - 5) {
						entities[i].dir = 0;
					}

					if(entities[j].xp > entities[i].xp + 45) {
						entities[i].ax = 3;
					} else if(entities[j].xp < entities[i].xp - 45) {
						entities[i].ax = -3;
					}
				} else if(entities[i].state == 12) {
					//11-15 means to follow a specific character, in crew order (cyan, purple, yellow, red, green, blue)
					int j = getcrewman(2); //yellow
					if(entities[j].xp > entities[i].xp + 5) {
						entities[i].dir = 1;
					} else if(entities[j].xp < entities[i].xp - 5) {
						entities[i].dir = 0;
					}

					if(entities[j].xp > entities[i].xp + 45) {
						entities[i].ax = 3;
					} else if(entities[j].xp < entities[i].xp - 45) {
						entities[i].ax = -3;
					}
				} else if(entities[i].state == 13) {
					//11-15 means to follow a specific character, in crew order (cyan, purple, yellow, red, green, blue)
					int j = getcrewman(3); //red
					if(entities[j].xp > entities[i].xp + 5) {
						entities[i].dir = 1;
					} else if(entities[j].xp < entities[i].xp - 5) {
						entities[i].dir = 0;
					}

					if(entities[j].xp > entities[i].xp + 45) {
						entities[i].ax = 3;
					} else if(entities[j].xp < entities[i].xp - 45) {
						entities[i].ax = -3;
					}
				} else if(entities[i].state == 14) {
					//11-15 means to follow a specific character, in crew order (cyan, purple, yellow, red, green, blue)
					int j = getcrewman(4); //green
					if(entities[j].xp > entities[i].xp + 5) {
						entities[i].dir = 1;
					} else if(entities[j].xp < entities[i].xp - 5) {
						entities[i].dir = 0;
					}

					if(entities[j].xp > entities[i].xp + 45) {
						entities[i].ax = 3;
					} else if(entities[j].xp < entities[i].xp - 45) {
						entities[i].ax = -3;
					}
				} else if(entities[i].state == 15) {
					//11-15 means to follow a specific character, in crew order (cyan, purple, yellow, red, green, blue)
					int j = getcrewman(5); //blue
					if(entities[j].xp > entities[i].xp + 5) {
						entities[i].dir = 1;
					} else if(entities[j].xp < entities[i].xp - 5) {
						entities[i].dir = 0;
					}

					if(entities[j].xp > entities[i].xp + 45) {
						entities[i].ax = 3;
					} else if(entities[j].xp < entities[i].xp - 45) {
						entities[i].ax = -3;
					}
				} else if(entities[i].state == 16) {
					//Follow a position: given an x coordinate, seek it out.
					if(entities[i].para > entities[i].xp + 5) {
						entities[i].dir = 1;
					} else if(entities[i].para < entities[i].xp - 5) {
						entities[i].dir = 0;
					}

					if(entities[i].para > entities[i].xp + 45) {
						entities[i].ax = 3;
					} else if(entities[i].para < entities[i].xp - 45) {
						entities[i].ax = -3;
					}
				} else if(entities[i].state == 17) {
					//stand still
				} else if(entities[i].state == 18) {
					//Stand still and face the player
					int j = getplayer();
					if(j > -1 && entities[j].xp > entities[i].xp + 5) {
						entities[i].dir = 1;
					} else if(j > -1 && entities[j].xp < entities[i].xp - 5) {
						entities[i].dir = 0;
					}
				} else if(entities[i].state == 19) {
					//Walk right off the screen after time t
					if(entities[i].para <= 0) {
						entities[i].dir = 1;
						entities[i].ax = 3;
					} else {
						entities[i].para--;
					}
				} else if(entities[i].state == 20) {
					//Panic! For briefing script
					if(entities[i].life == 0) {
						//walk left for a bit
						entities[i].ax = 0;
						if(40 > entities[i].xp + 5) {
							entities[i].dir = 1;
						} else if(40 < entities[i].xp - 5) {
							entities[i].dir = 0;
						}

						if(40 > entities[i].xp + 45) {
							entities[i].ax = 3;
						} else if(40 < entities[i].xp - 45) {
							entities[i].ax = -3;
						}
						if((entities[i].ax) == 0) {
							entities[i].life = 1;
							entities[i].para = 30;
						}
					} else if(entities[i].life == 1) {
						//Stand around for a bit
						entities[i].para--;
						if(entities[i].para <= 0) {
							entities[i].life++;
						}
					} else if(entities[i].life == 2) {
						//walk right for a bit
						entities[i].ax = 0;
						if(280 > entities[i].xp + 5) {
							entities[i].dir = 1;
						} else if(280 < entities[i].xp - 5) {
							entities[i].dir = 0;
						}

						if(280 > entities[i].xp + 45) {
							entities[i].ax = 3;
						} else if(280 < entities[i].xp - 45) {
							entities[i].ax = -3;
						}
						if((entities[i].ax) == 0) {
							entities[i].life = 3;
							entities[i].para = 30;
						}
					} else if(entities[i].life == 3) {
						//Stand around for a bit
						entities[i].para--;
						if(entities[i].para <= 0) {
							entities[i].life = 0;
						}
					}
				}
				break;
			case 13: //Terminals (very similar to savepoints)
				//wait for collision
				if(entities[i].state == 1) {
					entities[i].colour = 5;
					entities[i].onentity = 0;
					music.playef(17);

					entities[i].state = 0;
				}
				break;
			case 14: //Super Crew member
				//Actually needs less complex AI than the scripting crewmember
				if(entities[i].state == 0) {
					//follow player, but only if he's on the floor!
					int j = getplayer();
					if(j > -1 && entities[j].onground > 0) {
						if(entities[j].xp > entities[i].xp + 5) {
							entities[i].dir = 1;
						} else if(entities[j].xp > 15 && entities[j].xp < entities[i].xp - 5) {
							entities[i].dir = 0;
						}

						if(entities[j].xp > entities[i].xp + 45) {
							entities[i].ax = 3;
						} else if(entities[j].xp < entities[i].xp - 45) {
							entities[i].ax = -3;
						}
						if(entities[i].ax < 0 && entities[i].xp < 60) {
							entities[i].ax = 0;
						}
					} else {
						if(j > -1 && entities[j].xp > entities[i].xp + 5) {
							entities[i].dir = 1;
						} else if(j > -1 && entities[j].xp < entities[i].xp - 5) {
							entities[i].dir = 0;
						}

						entities[i].ax = 0;
					}

					if(entities[i].xp > 240) {
						entities[i].ax = 3;
						entities[i].dir = 1;
					}
					if(entities[i].xp >= 310) {
						game.scmprogress++;
						return removeentity(i);
					}
				}
				break;
			case 15: //Trophy
				//wait for collision
				if(entities[i].state == 1) {
					trophytext += 2;
					if(trophytext > 30) trophytext = 30;
					trophytype = entities[i].para;

					entities[i].state = 0;
				}
				break;
			case 23:
				//swn game!
				switch(entities[i].behave) {
					case 0:
						if(entities[i].state == 0) //Init
						{
							entities[i].vx = 7;
							if(entities[i].xp > 320) {
								return removeentity(i);
							}
						}
						break;
					case 1:
						if(entities[i].state == 0) //Init
						{
							entities[i].vx = -7;
							if(entities[i].xp < -20) {
								return removeentity(i);
							}
						}
						break;
				}
				break;

			case 51: //Vertical warp line
				if(entities[i].state == 2) {
					int j = getplayer();
					if(j > -1 && entities[j].xp <= 307) {
						customwarpmodevon = false;
						entities[i].state = 0;
					}
				} else if(entities[i].state == 1) {
					entities[i].state = 2;
					entities[i].statedelay = 2;
					entities[i].onentity = 1;
					customwarpmodevon = true;
				}
				break;
			case 52: //Vertical warp line
				if(entities[i].state == 2) {
					int j = getplayer();
					if(j > -1 && entities[j].xp <= 307) {
						customwarpmodevon = false;
						entities[i].state = 0;
					}
				} else if(entities[i].state == 1) {
					entities[i].state = 2;
					entities[i].statedelay = 2;
					entities[i].onentity = 1;
					customwarpmodevon = true;
				}
				break;
			case 53: //Warp lines Horizonal
				if(entities[i].state == 2) {
					customwarpmodehon = false;
					entities[i].state = 0;
				} else if(entities[i].state == 1) {
					entities[i].state = 2;
					entities[i].statedelay = 2;
					entities[i].onentity = 1;
					customwarpmodehon = true;
				}
				break;
			case 54: //Warp lines Horizonal
				if(entities[i].state == 2) {
					customwarpmodehon = false;
					entities[i].state = 0;
				} else if(entities[i].state == 1) {
					entities[i].state = 2;
					entities[i].statedelay = 2;
					entities[i].onentity = 1;
					customwarpmodehon = true;
				}
				break;
			case 55: //Collectable crewmate
				//wait for collision
				if(entities[i].state == 0) {
					//Basic rules, don't change expression
					int j = getplayer();
					if(j > -1 && entities[j].xp > entities[i].xp + 5) {
						entities[i].dir = 1;
					} else if(j > -1 && entities[j].xp < entities[i].xp - 5) {
						entities[i].dir = 0;
					}
				} else if(entities[i].state == 1) {
					if(INBOUNDS_ARR(entities[i].para, customcollect)) {
						customcollect[(int) entities[i].para] = true;
					}

					if(game.intimetrial) {
						music.playef(27);
					} else {
						game.state = 1010;
						//music.haltdasmusik();
						if(music.currentsong != -1) music.silencedasmusik();
						music.playef(27);
					}

					return removeentity(i);
				}
				break;
			case 100: //The teleporter
				if(entities[i].state == 1) {
					//if inactive, activate!
					if(entities[i].tile == 1) {
						music.playef(18);
						entities[i].onentity = 0;
						entities[i].tile = 2;
						entities[i].colour = 101;
						if(!game.intimetrial && !game.nodeathmode) {
							game.state = 2000;
							game.statedelay = 0;
						}

						game.activetele = true;
						game.teleblock.x = entities[i].xp - 32;
						game.teleblock.y = entities[i].yp - 32;
						game.teleblock.w = 160;
						game.teleblock.h = 160;

						//Alright, let's set this as our savepoint too
						//First, deactivate all other savepoints
						for(size_t j = 0; j < entities.size(); j++) {
							if(entities[j].type == 8) {
								entities[j].colour = 4;
								entities[j].onentity = 1;
							}
						}
						game.savepoint = static_cast<int>(entities[i].para);
						game.savex = entities[i].xp + 44;
						game.savey = entities[i].yp + 44;
						game.savegc = 0;

						game.saverx = game.roomx;
						game.savery = game.roomy;
						int player = getplayer();
						if(player > -1) {
							game.savedir = entities[player].dir;
						}
						entities[i].state = 0;
					}

					entities[i].state = 0;
				} else if(entities[i].state == 2) {
					//Initilise the teleporter without changing the game state or playing sound
					entities[i].onentity = 0;
					entities[i].tile = 6;
					entities[i].colour = 102;

					game.activetele = true;
					game.teleblock.x = entities[i].xp - 32;
					game.teleblock.y = entities[i].yp - 32;
					game.teleblock.w = 160;
					game.teleblock.h = 160;

					entities[i].state = 0;
				}
				break;
		}
	} else {
		entities[i].statedelay--;
		if(entities[i].statedelay < 0) {
			entities[i].statedelay = 0;
		}
	}

	return false;
}

void entityclass::animateentities(int _i)
{
	if(_i < 0 || _i >= (int) entities.size()) {
		puts("animateentities() out-of-bounds!");
		return;
	}

	if(entities[_i].statedelay < 1) {
		switch(entities[_i].type) {
			case 0:
				entities[_i].framedelay--;
				if(entities[_i].dir == 1) {
					entities[_i].drawframe = entities[_i].tile;
				} else {
					entities[_i].drawframe = entities[_i].tile + 3;
				}

				if(entities[_i].onground > 0 || entities[_i].onroof > 0) {
					if(entities[_i].vx > 0.00f || entities[_i].vx < -0.00f) {
						//Walking
						if(entities[_i].framedelay <= 1) {
							entities[_i].framedelay = 4;
							entities[_i].walkingframe++;
						}
						if(entities[_i].walkingframe >= 2) entities[_i].walkingframe = 0;
						entities[_i].drawframe += entities[_i].walkingframe + 1;
					}

					if(entities[_i].onroof > 0) entities[_i].drawframe += 6;
				} else {
					entities[_i].drawframe++;
					if(game.gravitycontrol == 1) {
						entities[_i].drawframe += 6;
					}
				}

				if(game.deathseq > -1) {
					entities[_i].drawframe = 13;
					if(entities[_i].dir == 1) entities[_i].drawframe = 12;
					if(game.gravitycontrol == 1) entities[_i].drawframe += 2;
				}
				break;
			case 1:
			case 23:
				//Variable animation
				switch(entities[_i].animate) {
					case 0:
						//Simple oscilation
						entities[_i].framedelay--;
						if(entities[_i].framedelay <= 0) {
							entities[_i].framedelay = 8;
							if(entities[_i].actionframe == 0) {
								entities[_i].walkingframe++;
								if(entities[_i].walkingframe == 4) {
									entities[_i].walkingframe = 2;
									entities[_i].actionframe = 1;
								}
							} else {
								entities[_i].walkingframe--;
								if(entities[_i].walkingframe == -1) {
									entities[_i].walkingframe = 1;
									entities[_i].actionframe = 0;
								}
							}
						}

						entities[_i].drawframe = entities[_i].tile;
						entities[_i].drawframe += entities[_i].walkingframe;
						break;
					case 1:
						//Simple Loop
						entities[_i].framedelay--;
						if(entities[_i].framedelay <= 0) {
							entities[_i].framedelay = 8;
							entities[_i].walkingframe++;
							if(entities[_i].walkingframe == 4) {
								entities[_i].walkingframe = 0;
							}
						}

						entities[_i].drawframe = entities[_i].tile;
						entities[_i].drawframe += entities[_i].walkingframe;
						break;
					case 2:
						//Simpler Loop (just two frames)
						entities[_i].framedelay--;
						if(entities[_i].framedelay <= 0) {
							entities[_i].framedelay = 2;
							entities[_i].walkingframe++;
							if(entities[_i].walkingframe == 2) {
								entities[_i].walkingframe = 0;
							}
						}

						entities[_i].drawframe = entities[_i].tile;
						entities[_i].drawframe += entities[_i].walkingframe;
						break;
					case 3:
						//Simpler Loop (just two frames, but double sized)
						entities[_i].framedelay--;
						if(entities[_i].framedelay <= 0) {
							entities[_i].framedelay = 2;
							entities[_i].walkingframe++;
							if(entities[_i].walkingframe == 2) {
								entities[_i].walkingframe = 0;
							}
						}

						entities[_i].drawframe = entities[_i].tile;
						entities[_i].drawframe += (entities[_i].walkingframe * 2);
						break;
					case 4:
						//Simpler Loop (just two frames, but double sized) (as above but slower)
						entities[_i].framedelay--;
						if(entities[_i].framedelay <= 0) {
							entities[_i].framedelay = 6;
							entities[_i].walkingframe++;
							if(entities[_i].walkingframe == 2) {
								entities[_i].walkingframe = 0;
							}
						}

						entities[_i].drawframe = entities[_i].tile;
						entities[_i].drawframe += (entities[_i].walkingframe * 2);
						break;
					case 5:
						//Simpler Loop (just two frames) (slower)
						entities[_i].framedelay--;
						if(entities[_i].framedelay <= 0) {
							entities[_i].framedelay = 6;
							entities[_i].walkingframe++;
							if(entities[_i].walkingframe == 2) {
								entities[_i].walkingframe = 0;
							}
						}

						entities[_i].drawframe = entities[_i].tile;
						entities[_i].drawframe += entities[_i].walkingframe;
						break;
					case 6:
						//Normal Loop (four frames, double sized)
						entities[_i].framedelay--;
						if(entities[_i].framedelay <= 0) {
							entities[_i].framedelay = 4;
							entities[_i].walkingframe++;
							if(entities[_i].walkingframe == 4) {
								entities[_i].walkingframe = 0;
							}
						}

						entities[_i].drawframe = entities[_i].tile;
						entities[_i].drawframe += (entities[_i].walkingframe * 2);
						break;
					case 7:
						//Simpler Loop (just two frames) (slower) (with directions!)
						entities[_i].framedelay--;
						if(entities[_i].framedelay <= 0) {
							entities[_i].framedelay = 6;
							entities[_i].walkingframe++;
							if(entities[_i].walkingframe == 2) {
								entities[_i].walkingframe = 0;
							}
						}

						entities[_i].drawframe = entities[_i].tile;
						entities[_i].drawframe += entities[_i].walkingframe;

						if(entities[_i].vx > 0.000f) entities[_i].drawframe += 2;
						break;
					case 8:
						//Simple Loop (with directions)
						entities[_i].framedelay--;
						if(entities[_i].framedelay <= 0) {
							entities[_i].framedelay = 8;
							entities[_i].walkingframe++;
							if(entities[_i].walkingframe == 4) {
								entities[_i].walkingframe = 0;
							}
						}

						entities[_i].drawframe = entities[_i].tile;
						entities[_i].drawframe += entities[_i].walkingframe;

						if(entities[_i].vx > 0.000f) entities[_i].drawframe -= 4;
						break;
					case 9:
						// No animation (with directions)
						entities[_i].drawframe = entities[_i].tile;
						if(entities[_i].vx < 0.000f) entities[_i].drawframe += 12;
						break;
					case 10:
						//Threadmill left
						entities[_i].framedelay--;
						if(entities[_i].framedelay <= 0) {
							entities[_i].framedelay = 3; //(6-entities[_i].para);
							entities[_i].walkingframe--;
							if(entities[_i].walkingframe == -1) {
								entities[_i].walkingframe = 3;
							}
						}

						entities[_i].drawframe = entities[_i].tile;
						entities[_i].drawframe += entities[_i].walkingframe;
						break;
					case 11:
						//Threadmill right
						entities[_i].framedelay--;
						if(entities[_i].framedelay <= 0) {
							entities[_i].framedelay = 3; //(6-entities[_i].para);
							entities[_i].walkingframe++;
							if(entities[_i].walkingframe == 4) {
								entities[_i].walkingframe = 0;
							}
						}

						entities[_i].drawframe = entities[_i].tile;
						entities[_i].drawframe += entities[_i].walkingframe;
						break;
					case 100:
						//Simple case for no animation (platforms, etc)
						entities[_i].drawframe = entities[_i].tile;
						break;
					case 12:
						//Simpler Loop (just two frames) (slower) (directionally flipped)
						entities[_i].framedelay--;
						if(entities[_i].framedelay <= 0) {
							entities[_i].framedelay = 6;
							entities[_i].walkingframe++;
							if(entities[_i].walkingframe == 2) {
								entities[_i].walkingframe = 0;
							}
						}

						entities[_i].drawframe = entities[_i].tile;
						entities[_i].drawframe += entities[_i].walkingframe;
						entities[_i].flipped = entities[_i].vx < 0.000f ? entities[_i].flippedsize : 0;
						break;
					default:
						entities[_i].drawframe = entities[_i].tile;
						break;
				}
				break;

			case 4: // Gravity token anim
				entities[_i].framedelay--;
				if(entities[_i].framedelay <= 0) {
					if(entities[_i].life > 0) {
						entities[_i].framedelay = 4;
						entities[_i].life--;
					} else {
						entities[_i].framedelay = 8;
					}
					entities[_i].walkingframe++;
					if(entities[_i].walkingframe == 4) {
						entities[_i].walkingframe = 0;
					}
				} else if(entities[_i].life > 0 && entities[_i].framedelay > 4) {
					// Clamp it down to 4 frames when respawning
					entities[_i].framedelay = 4;
				}

				entities[_i].drawframe = entities[_i].tile;
				if(IS_VCE_LEVEL) {
					entities[_i].drawframe += (entities[_i].walkingframe);
				}
				break;

			case 11:
				entities[_i].drawframe = entities[_i].tile;
				if(entities[_i].animate == 2) {
					//Simpler Loop (just two frames)
					entities[_i].framedelay--;
					if(entities[_i].framedelay <= 0) {
						entities[_i].framedelay = 10;
						entities[_i].walkingframe++;
						if(entities[_i].walkingframe == 2) {
							entities[_i].walkingframe = 0;
						}
					}

					entities[_i].drawframe = entities[_i].tile;
					entities[_i].drawframe += entities[_i].walkingframe;
				}
				break;
			case 12:
			case 55:
			case 14: //Crew member! Very similar to hero
				entities[_i].framedelay--;
				if(entities[_i].dir == 1) {
					entities[_i].drawframe = entities[_i].tile;
				} else {
					entities[_i].drawframe = entities[_i].tile + 3;
				}

				if(entities[_i].onground > 0 || entities[_i].onroof > 0) {
					if(entities[_i].vx > 0.0000f || entities[_i].vx < -0.000f) {
						//Walking
						if(entities[_i].framedelay <= 0) {
							entities[_i].framedelay = 4;
							entities[_i].walkingframe++;
						}
						if(entities[_i].walkingframe >= 2) entities[_i].walkingframe = 0;
						entities[_i].drawframe += entities[_i].walkingframe + 1;
					}

					//if (entities[_i].onroof > 0) entities[_i].drawframe += 6;
				} else {
					entities[_i].drawframe++;
					//if (game.gravitycontrol == 1) {
					//	entities[_i].drawframe += 6;
					//}
				}

				if(game.deathseq > -1) {
					entities[_i].drawframe = 13;
					if(entities[_i].dir == 1) entities[_i].drawframe = 12;
					if(entities[_i].rule == 7) entities[_i].drawframe += 2;
					//if (game.gravitycontrol == 1) entities[_i].drawframe += 2;
				}
				break;
			case 100: //the teleporter!
				if(entities[_i].tile == 1) {
					//it's inactive
					entities[_i].drawframe = entities[_i].tile;
				} else if(entities[_i].tile == 2) {
					entities[_i].drawframe = entities[_i].tile;

					entities[_i].framedelay--;
					if(entities[_i].framedelay <= 0) {
						entities[_i].framedelay = 1;
						entities[_i].walkingframe = int(fRandom() * 6);
						if(entities[_i].walkingframe >= 4) {
							entities[_i].walkingframe = -1;
							entities[_i].framedelay = 4;
						}
					}

					entities[_i].drawframe = entities[_i].tile;
					entities[_i].drawframe += entities[_i].walkingframe;
				} else if(entities[_i].tile == 6) {
					//faster!
					entities[_i].drawframe = entities[_i].tile;

					entities[_i].framedelay--;
					if(entities[_i].framedelay <= 0) {
						entities[_i].framedelay = 2;
						entities[_i].walkingframe = int(fRandom() * 6);
						if(entities[_i].walkingframe >= 4) {
							entities[_i].walkingframe = -5;
							entities[_i].framedelay = 4;
						}
					}

					entities[_i].drawframe = entities[_i].tile;
					entities[_i].drawframe += entities[_i].walkingframe;
				}
				break;
			default:
				entities[_i].drawframe = entities[_i].tile;
				break;
		}
	} else {
		//entities[_i].statedelay--;
		if(entities[_i].statedelay < 0) entities[_i].statedelay = 0;
	}
}

int entityclass::getcompanion()
{
	//Returns the index of the companion with rule t
	for(size_t i = 0; i < entities.size(); i++) {
		if(entities[i].rule == 6 || entities[i].rule == 7) {
			return i;
		}
	}

	return -1;
}

int entityclass::getplayer()
{
	//Returns the index of the first player entity
	for(size_t i = 0; i < entities.size(); i++) {
		if(entities[i].type == 0) {
			return i;
		}
	}

	return -1;
}

int entityclass::getscm()
{
	//Returns the supercrewmate
	for(size_t i = 0; i < entities.size(); i++) {
		if(entities[i].type == 14) {
			return i;
		}
	}

	return 0;
}

int entityclass::getlineat(int t)
{
	//Get the entity which is a horizontal line at height t (for SWN game)
	for(size_t i = 0; i < entities.size(); i++) {
		if(entities[i].size == 5) {
			if(entities[i].yp == t) {
				return i;
			}
		}
	}

	return 0;
}

int entityclass::getcrewman(int t)
{
	//Returns the index of the crewman with colour index given by t
	if(t == 0) t = 0;
	if(t == 1) t = 20;
	if(t == 2) t = 14;
	if(t == 3) t = 15;
	if(t == 4) t = 13;
	if(t == 5) t = 16;

	for(size_t i = 0; i < entities.size(); i++) {
		if((entities[i].type == 12 || entities[i].type == 14) && (entities[i].rule == 6 || entities[i].rule == 7)) {
			if(entities[i].colour == t) {
				return i;
			}
		}
	}

	return 0;
}

int entityclass::getcrewman(std::string t)
{
	if(t == "player") {
		return getplayer();
	} else if(t == "cyan") {
		return getcrewman(0);
	} else if(t == "customcyan") {
		return getcustomcrewman(0);
	} else if(t == "red") {
		return getcrewman(3);
	} else if(t == "green") {
		return getcrewman(4);
	} else if(t == "yellow") {
		return getcrewman(2);
	} else if(t == "blue") {
		return getcrewman(5);
	} else if(t == "purple") {
		return getcrewman(1);
	} else if(t == "pink") {
		return getcrewman(1);
	} else {
		auto iter = named_crewmen.find(t);
		if(iter != named_crewmen.end())
			return iter->second;
		else
			return 0;
	}
}

int entityclass::getcustomcrewman(int t)
{
	//Returns the index of the crewman with colour index given by t
	if(t == 0) t = 0;
	if(t == 1) t = 20;
	if(t == 2) t = 14;
	if(t == 3) t = 15;
	if(t == 4) t = 13;
	if(t == 5) t = 16;

	for(size_t i = 0; i < entities.size(); i++) {
		if(entities[i].type == 55) {
			if(entities[i].colour == t) {
				return i;
			}
		}
	}

	return 0;
}

int entityclass::getteleporter()
{
	for(size_t i = 0; i < entities.size(); i++) {
		if(entities[i].type == 100) {
			return i;
		}
	}

	return -1;
}

void entityclass::rectset(int xi, int yi, int wi, int hi)
{
	temprect.x = xi;
	temprect.y = yi;
	temprect.w = wi;
	temprect.h = hi;
}

void entityclass::rect2set(int xi, int yi, int wi, int hi)
{
	temprect2.x = xi;
	temprect2.y = yi;
	temprect2.w = wi;
	temprect2.h = hi;
}

bool entityclass::entitycollide(int a, int b)
{
	if(a < 0 || a > (int) entities.size() || b < 0 || b > (int) entities.size()) {
		puts("entitycollide() out-of-bounds!");
		return false;
	}

	//Do entities a and b collide?
	tempx = entities[a].xp + entities[a].cx;
	tempy = entities[a].yp + entities[a].cy;
	tempw = entities[a].w;
	temph = entities[a].h;
	rectset(tempx, tempy, tempw, temph);

	tempx = entities[b].xp + entities[b].cx;
	tempy = entities[b].yp + entities[b].cy;
	tempw = entities[b].w;
	temph = entities[b].h;
	rect2set(tempx, tempy, tempw, temph);

	if(help.intersects(temprect, temprect2)) return true;
	return false;
}

bool entityclass::checkdamage()
{
	//Returns true if player entity (rule 0) collides with a damagepoint
	for(size_t i = 0; i < entities.size(); i++) {
		if(entities[i].rule == 0) {
			tempx = entities[i].xp + entities[i].cx;
			tempy = entities[i].yp + entities[i].cy;
			tempw = entities[i].w;
			temph = entities[i].h;
			rectset(tempx, tempy, tempw, temph);

			for(size_t j = 0; j < blocks.size(); j++) {
				if(blocks[j].type == DAMAGE && help.intersects(blocks[j].rect, temprect)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool entityclass::scmcheckdamage()
{
	//Returns true if supercrewmate collides with a damagepoint
	for(size_t i = 0; i < entities.size(); i++) {
		if(entities[i].type == 14) {
			tempx = entities[i].xp + entities[i].cx;
			tempy = entities[i].yp + entities[i].cy;
			tempw = entities[i].w;
			temph = entities[i].h;
			rectset(tempx, tempy, tempw, temph);

			for(size_t j = 0; j < blocks.size(); j++) {
				if(blocks[j].type == DAMAGE && help.intersects(blocks[j].rect, temprect)) {
					return true;
				}
			}
		}
	}
	return false;
}

void entityclass::settemprect(int t)
{
	if(t < 0 || t >= (int) entities.size()) {
		return;
	}

	//setup entity t in temprect
	tempx = entities[t].xp + entities[t].cx;
	tempy = entities[t].yp + entities[t].cy;
	tempw = entities[t].w;
	temph = entities[t].h;
	rectset(tempx, tempy, tempw, temph);
}

int entityclass::checktrigger()
{
	//Returns an int player entity (rule 0) collides with a trigger
	for(size_t i = 0; i < entities.size(); i++) {
		if(entities[i].rule == 0) {
			tempx = entities[i].xp + entities[i].cx;
			tempy = entities[i].yp + entities[i].cy;
			tempw = entities[i].w;
			temph = entities[i].h;
			rectset(tempx, tempy, tempw, temph);

			for(size_t j = 0; j < blocks.size(); j++) {
				if(blocks[j].type == TRIGGER && help.intersects(blocks[j].rect, temprect)) {
					activetrigger = blocks[j].trigger;
					return blocks[j].trigger;
				}
			}
		}
	}
	return -1;
}

int entityclass::checkactivity()
{
	//Returns an int player entity (rule 0) collides with an activity
	for(size_t i = 0; i < entities.size(); i++) {
		if(entities[i].rule == 0) {
			tempx = entities[i].xp + entities[i].cx;
			tempy = entities[i].yp + entities[i].cy;
			tempw = entities[i].w;
			temph = entities[i].h;
			rectset(tempx, tempy, tempw, temph);

			for(size_t j = 0; j < blocks.size(); j++) {
				if(blocks[j].type == ACTIVITY && help.intersects(blocks[j].rect, temprect)) {
					return j;
				}
			}
		}
	}
	return -1;
}

int entityclass::getgridpoint(int t)
{
	t = (t - (t % 8)) / 8;
	return t;
}

bool entityclass::checkplatform()
{
	//Return true if rectset intersects a moving platform, setups px & py to the platform x & y
	for(size_t i = 0; i < blocks.size(); i++) {
		if(blocks[i].type == BLOCK && help.intersects(blocks[i].rect, temprect)) {
			px = blocks[i].xp;
			py = blocks[i].yp;
			return true;
		}
	}
	return false;
}

bool entityclass::checkblocks()
{
	for(size_t i = 0; i < blocks.size(); i++) {
		if(!skipdirblocks && blocks[i].type == DIRECTIONAL) {
			if(dy > 0 && blocks[i].trigger == 0)
				if(help.intersects(blocks[i].rect, temprect)) return true;
			if(dy <= 0 && blocks[i].trigger == 1)
				if(help.intersects(blocks[i].rect, temprect)) return true;
			if(dx > 0 && blocks[i].trigger == 2)
				if(help.intersects(blocks[i].rect, temprect)) return true;
			if(dx <= 0 && blocks[i].trigger == 3)
				if(help.intersects(blocks[i].rect, temprect)) return true;
		}
		if(blocks[i].type == BLOCK && help.intersects(blocks[i].rect, temprect)) {
			return true;
		}
		if(blocks[i].type == SAFE && (dr) == 1 && help.intersects(blocks[i].rect, temprect)) {
			return true;
		}
	}
	return false;
}

bool entityclass::checkwall()
{
	//Returns true if entity setup in temprect collides with a wall
	//used for proper collision functions; you can't just, like, call it
	//whenever you feel like it and expect a response
	//
	//that won't work at all
	if(skipblocks) {
		if(checkblocks()) return true;
	}

	tempx = getgridpoint(temprect.x);
	tempy = getgridpoint(temprect.y);
	tempw = getgridpoint(temprect.x + temprect.w - 1);
	temph = getgridpoint(temprect.y + temprect.h - 1);
	if(map.collide(tempx, tempy)) return true;
	if(map.collide(tempw, tempy)) return true;
	if(map.collide(tempx, temph)) return true;
	if(map.collide(tempw, temph)) return true;
	if(temprect.h >= 12) {
		tpy1 = getgridpoint(temprect.y + 6);
		if(map.collide(tempx, tpy1)) return true;
		if(map.collide(tempw, tpy1)) return true;
		if(temprect.h >= 18) {
			tpy1 = getgridpoint(temprect.y + 12);
			if(map.collide(tempx, tpy1)) return true;
			if(map.collide(tempw, tpy1)) return true;
			if(temprect.h >= 24) {
				tpy1 = getgridpoint(temprect.y + 18);
				if(map.collide(tempx, tpy1)) return true;
				if(map.collide(tempw, tpy1)) return true;
			}
		}
	}
	if(temprect.w >= 12) {
		tpx1 = getgridpoint(temprect.x + 6);
		if(map.collide(tpx1, tempy)) return true;
		if(map.collide(tpx1, temph)) return true;
	}
	return false;
}

float entityclass::hplatformat()
{
	//Returns first entity of horizontal platform at (px, py), -1000 otherwise.
	for(size_t i = 0; i < entities.size(); i++) {
		if(entities[i].rule == 2 && entities[i].behave >= 2 && entities[i].xp == px && entities[i].yp == py) {
			if(entities[i].behave == 8) //threadmill!
			{
				return entities[i].para;
			} else if(entities[i].behave == 9) //threadmill!
			{
				return -entities[i].para;
			} else {
				return entities[i].vx;
			}
		}
	}
	return -1000;
}

int entityclass::yline(int a, int b)
{
	if(a < b) return -1;
	return 1;
}

bool entityclass::entityhlinecollide(int t, int l)
{
	if(t < 0 || t >= (int) entities.size() || l < 0 || l >= (int) entities.size()) {
		puts("entityhlinecollide() out-of-bounds!");
		return false;
	}

	//Returns true is entity t collided with the horizontal line l.
	if(entities[t].xp + entities[t].cx + entities[t].w >= entities[l].xp) {
		if(entities[t].xp + entities[t].cx <= entities[l].xp + entities[l].w) {
			linetemp = 0;

			linetemp += yline(entities[t].yp, entities[l].yp);
			linetemp += yline(entities[t].yp + entities[t].h, entities[l].yp);
			linetemp += yline(entities[t].oldyp, entities[l].yp);
			linetemp += yline(entities[t].oldyp + entities[t].h, entities[l].yp);

			if(linetemp > -4 && linetemp < 4) return true;
			return false;
		}
	}
	return false;
}

bool entityclass::entityvlinecollide(int t, int l)
{
	if(t < 0 || t >= (int) entities.size() || l < 0 || l >= (int) entities.size()) {
		puts("entityvlinecollide() out-of-bounds!");
		return false;
	}

	//Returns true is entity t collided with the vertical line l.
	if(entities[t].yp + entities[t].cy + entities[t].h >= entities[l].yp && entities[t].yp + entities[t].cy <= entities[l].yp + entities[l].h) {
		linetemp = 0;

		linetemp += yline(entities[t].xp + entities[t].cx + 1, entities[l].xp);
		linetemp += yline(entities[t].xp + entities[t].cx + 1 + entities[t].w, entities[l].xp);
		linetemp += yline(entities[t].oldxp + entities[t].cx + 1, entities[l].xp);
		linetemp += yline(entities[t].oldxp + entities[t].cx + 1 + entities[t].w, entities[l].xp);

		if(linetemp > -4 && linetemp < 4) return true;
		return false;
	}
	return false;
}

bool entityclass::entitywarphlinecollide(int t, int l)
{
	if(t < 0 || t >= (int) entities.size() || l < 0 || l >= (int) entities.size()) {
		puts("entitywarphlinecollide() out-of-bounds!");
		return false;
	}

	//Returns true is entity t collided with the horizontal line l.
	if(entities[t].xp + entities[t].cx + entities[t].w >= entities[l].xp && entities[t].xp + entities[t].cx <= entities[l].xp + entities[l].w) {
		linetemp = 0;
		if(entities[l].yp < 120) {
			//Top line
			if(entities[t].vy < 0) {
				if(entities[t].yp < entities[l].yp + 10) linetemp++;
				if(entities[t].yp + entities[t].h < entities[l].yp + 10) linetemp++;
				if(entities[t].oldyp < entities[l].yp + 10) linetemp++;
				if(entities[t].oldyp + entities[t].h < entities[l].yp + 10) linetemp++;
			}

			if(linetemp > 0) return true;
			return false;
		} else {
			//Bottom line
			if(entities[t].vy > 0) {
				if(entities[t].yp > entities[l].yp - 10) linetemp++;
				if(entities[t].yp + entities[t].h > entities[l].yp - 10) linetemp++;
				if(entities[t].oldyp > entities[l].yp - 10) linetemp++;
				if(entities[t].oldyp + entities[t].h > entities[l].yp - 10) linetemp++;
			}

			if(linetemp > 0) return true;
			return false;
		}
	}
	return false;
}

bool entityclass::entitywarpvlinecollide(int t, int l)
{
	if(t < 0 || t >= (int) entities.size() || l < 0 || l >= (int) entities.size()) {
		puts("entitywarpvlinecollide() out-of-bounds!");
		return false;
	}

	//Returns true is entity t collided with the vertical warp line l.
	if(entities[t].yp + entities[t].cy + entities[t].h >= entities[l].yp && entities[t].yp + entities[t].cy <= entities[l].yp + entities[l].h) {
		linetemp = 0;
		if(entities[l].xp < 160) {
			//Left hand line
			if(entities[t].xp + entities[t].cx + 1 < entities[l].xp + 10) linetemp++;
			if(entities[t].xp + entities[t].cx + 1 + entities[t].w < entities[l].xp + 10) linetemp++;
			if(entities[t].oldxp + entities[t].cx + 1 < entities[l].xp + 10) linetemp++;
			if(entities[t].oldxp + entities[t].cx + 1 + entities[t].w < entities[l].xp + 10) linetemp++;

			if(linetemp > 0) return true;
			return false;
		} else {
			//Right hand line
			if(entities[t].xp + entities[t].cx + 1 > entities[l].xp - 10) linetemp++;
			if(entities[t].xp + entities[t].cx + 1 + entities[t].w > entities[l].xp - 10) linetemp++;
			if(entities[t].oldxp + entities[t].cx + 1 > entities[l].xp - 10) linetemp++;
			if(entities[t].oldxp + entities[t].cx + 1 + entities[t].w > entities[l].xp - 10) linetemp++;

			if(linetemp > 0) return true;
			return false;
		}
	}
	return false;
}

float entityclass::entitycollideplatformroof(int t)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("entitycollideplatformroof() out-of-bounds!");
		return -1000;
	}

	tempx = entities[t].xp + entities[t].cx;
	tempy = entities[t].yp + entities[t].cy - 1;
	tempw = entities[t].w;
	temph = entities[t].h;
	rectset(tempx, tempy, tempw, temph);

	if(checkplatform()) {
		//px and py now contain an x y coordinate for a platform, find it
		return hplatformat();
	}
	return -1000;
}

float entityclass::entitycollideplatformfloor(int t)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("entitycollideplatformfloor() out-of-bounds!");
		return -1000;
	}

	tempx = entities[t].xp + entities[t].cx;
	tempy = entities[t].yp + entities[t].cy + 1;
	tempw = entities[t].w;
	temph = entities[t].h;
	rectset(tempx, tempy, tempw, temph);

	if(checkplatform()) {
		//px and py now contain an x y coordinate for a platform, find it
		return hplatformat();
	}
	return -1000;
}

bool entityclass::entitycollidefloor(int t)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("entitycollidefloor() out-of-bounds!");
		return false;
	}

	//see? like here, for example!
	tempx = entities[t].xp + entities[t].cx;
	tempy = entities[t].yp + entities[t].cy + 1;
	tempw = entities[t].w;
	temph = entities[t].h;
	rectset(tempx, tempy, tempw, temph);

	map.collidefloor = IS_VCE_LEVEL;
	bool check = checkwall();
	map.collidefloor = false;
	return check;
}

bool entityclass::entitycollideroof(int t)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("entitycollideroof() out-of-bounds!");
		return false;
	}

	//and here!
	tempx = entities[t].xp + entities[t].cx;
	tempy = entities[t].yp + entities[t].cy - 1;
	tempw = entities[t].w;
	temph = entities[t].h;
	rectset(tempx, tempy, tempw, temph);

	map.collideroof = IS_VCE_LEVEL;
	bool check = checkwall();
	map.collideroof = false;
	return check;
}

bool entityclass::testwallsx(int t, int tx, int ty)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("testwallsx() out-of-bounds!");
		return false;
	}

	tempx = tx + entities[t].cx;
	tempy = ty + entities[t].cy;
	tempw = entities[t].w;
	temph = entities[t].h;
	rectset(tempx, tempy, tempw, temph);

	if(entities[t].rule < 2) {
		skipblocks = true;
	} else {
		skipblocks = false;
	}
	if(entities[t].type == 14) skipblocks = true;
	dx = 0;
	dy = 0;
	if(entities[t].rule == 0) dx = entities[t].vx;
	dr = entities[t].rule;

	//Ok, now we check walls
	if(checkwall()) {
		if(entities[t].vx > 1.0f) {
			entities[t].vx--;
			entities[t].newxp = entities[t].xp + entities[t].vx;
			return testwallsx(t, entities[t].newxp, entities[t].yp);
		} else if(entities[t].vx < -1.0f) {
			entities[t].vx++;
			entities[t].newxp = entities[t].xp + entities[t].vx;
			return testwallsx(t, entities[t].newxp, entities[t].yp);
		} else {
			entities[t].vx = 0;
			return false;
		}
	}
	return true;
}

bool entityclass::testwallsy(int t, float tx, float ty)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("testwallsy() out-of-bounds!");
		return false;
	}

	tempx = static_cast<int>(tx) + entities[t].cx;
	tempy = static_cast<int>(ty) + entities[t].cy;
	tempw = entities[t].w;
	temph = entities[t].h;
	rectset(tempx, tempy, tempw, temph);

	if(entities[t].rule < 2) {
		skipblocks = true;
	} else {
		skipblocks = false;
	}
	if(entities[t].type == 14) skipblocks = true;

	dx = 0;
	dy = 0;
	if(entities[t].rule == 0) dy = entities[t].vy;
	dr = entities[t].rule;

	//Ok, now we check walls
	if(checkwall()) {
		if(entities[t].vy > 1) {
			entities[t].vy--;
			entities[t].newyp = int(entities[t].yp + entities[t].vy);
			return testwallsy(t, entities[t].xp, entities[t].newyp);
		} else if(entities[t].vy < -1) {
			entities[t].vy++;
			entities[t].newyp = int(entities[t].yp + entities[t].vy);
			return testwallsy(t, entities[t].xp, entities[t].newyp);
		} else {
			entities[t].vy = 0;
			return false;
		}
	}
	return true;
}

void entityclass::fixfriction(int t, float xfix, float xrate, float yrate)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("fixfriction() out-of-bounds!");
		return;
	}

	if(entities[t].vx > xfix) entities[t].vx -= xrate;
	if(entities[t].vx < xfix) entities[t].vx += xrate;
	if(entities[t].vy > 0) entities[t].vy -= yrate;
	if(entities[t].vy < 0) entities[t].vy += yrate;
	if(entities[t].vy > 10) entities[t].vy = 10;
	if(entities[t].vy < -10) entities[t].vy = -10;
	if(entities[t].vx > 6) entities[t].vx = 6.0f;
	if(entities[t].vx < -6) entities[t].vx = -6.0f;

	if(std::abs(entities[t].vx - xfix) <= xrate) entities[t].vx = xfix;
	if(std::abs(entities[t].vy) < yrate) entities[t].vy = 0;
}

void entityclass::applyfriction(int t, float xrate, float yrate, int speed)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("applyfriction() out-of-bounds!");
		return;
	}

	if(entities[t].vx > 0.00f) entities[t].vx -= xrate;
	if(entities[t].vx < 0.00f) entities[t].vx += xrate;
	if(entities[t].vy > 0.00f) entities[t].vy -= yrate;
	if(entities[t].vy < 0.00f) entities[t].vy += yrate;
	if(entities[t].vy > 10.00f) entities[t].vy = 10.0f;
	if(entities[t].vy < -10.00f) entities[t].vy = -10.0f;
	if(entities[t].vx > (float) (speed * 2)) entities[t].vx = speed * 2;
	if(entities[t].vx < (float) (speed * -2)) entities[t].vx = speed * -2;

	if(std::abs(entities[t].vx) < xrate) entities[t].vx = 0.0f;
	if(std::abs(entities[t].vy) < yrate) entities[t].vy = 0.0f;
}

void entityclass::cleanupresurrectblocks()
{
}

void entityclass::updateentitylogic(int t)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("updateentitylogic() out-of-bounds!");
		return;
	}

	entities[t].vx = entities[t].vx + entities[t].ax;
	entities[t].vy = entities[t].vy + entities[t].ay;
	entities[t].ax = 0;

	if(entities[t].gravity) {
		if(entities[t].rule == 0) {
			if(game.gravitycontrol == 0) {
				entities[t].ay = 3;
			} else {
				entities[t].ay = -3;
			}
		} else if(entities[t].rule == 7) {
			entities[t].ay = -3;
		} else {
			entities[t].ay = 3;
		}
		if(!game.nofriction) applyfriction(t, game.inertia, 0.25f, game.playerspeed);
	}

	entities[t].newxp = entities[t].xp + entities[t].vx;
	entities[t].newyp = entities[t].yp + entities[t].vy;
}

void entityclass::entitymapcollision(int t)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("entitymapcollision() out-of-bounds!");
		return;
	}

	if(testwallsx(t, entities[t].newxp, entities[t].yp)) {
		entities[t].xp = entities[t].newxp;
	} else {
		if(entities[t].onwall > 0) entities[t].state = entities[t].onwall;
		if(entities[t].onxwall > 0) entities[t].state = entities[t].onxwall;
	}
	if(testwallsy(t, entities[t].xp, entities[t].newyp)) {
		entities[t].yp = entities[t].newyp;
	} else {
		if(entities[t].onwall > 0) entities[t].state = entities[t].onwall;
		if(entities[t].onywall > 0) entities[t].state = entities[t].onywall;
		entities[t].jumpframe = 0;
	}
}

void entityclass::movingplatformfix(int t)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("movingplatformfix() out-of-bounds!");
		return;
	}

	//If this intersects the player, then we move the player along it
	int j = getplayer();
	if(j > -1 && entitycollide(t, j)) {
		//ok, bollox, let's make sure
		entities[j].yp = entities[j].yp + int(entities[j].vy);
		if(entitycollide(t, j)) {
			entities[j].yp = entities[j].yp - int(entities[j].vy);
			entities[j].vy = entities[t].vy;
			entities[j].newyp = entities[j].yp + int(entities[j].vy);
			if(testwallsy(j, entities[j].xp, entities[j].newyp)) {
				if(entities[t].vy > 0) {
					entities[j].yp = entities[t].yp + entities[t].h;
					entities[j].vy = 0;
					entities[j].onroof = true;
				} else {
					entities[j].yp = entities[t].yp - entities[j].h - entities[j].cy;
					entities[j].vy = 0;
					entities[j].onground = true;
				}
			} else {
				entities[t].state = entities[t].onwall;
			}
		}
	}
}

void entityclass::scmmovingplatformfix(int t)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("scmmovingplatformfix() out-of-bounds!");
		return;
	}

	//If this intersects the SuperCrewMate, then we move them along it
	int j = getscm();
	if(entitycollide(t, j)) {
		//ok, bollox, let's make sure
		entities[j].yp = entities[j].yp + (entities[j].vy);
		if(entitycollide(t, j)) {
			entities[j].yp = entities[j].yp - (entities[j].vy);
			entities[j].vy = entities[t].vy;
			entities[j].newyp = static_cast<float>(entities[j].yp) + entities[j].vy;
			if(testwallsy(j, entities[j].xp, entities[j].newyp)) {
				if(entities[t].vy > 0) {
					entities[j].yp = entities[t].yp + entities[t].h;
					entities[j].vy = 0;
				} else {
					entities[j].yp = entities[t].yp - entities[j].h - entities[j].cy;
					entities[j].vy = 0;
				}
			} else {
				entities[t].state = entities[t].onwall;
			}
		}
	}
}

void entityclass::hormovingplatformfix(int t)
{
	if(t < 0 || t >= (int) entities.size()) {
		puts("hormovingplatformfix() out-of-bounds!");
		return;
	}

	//If this intersects the player, then we move the player along it
	//for horizontal platforms, this is simplier
	createblock(0, entities[t].xp, entities[t].yp, entities[t].w, entities[t].h);
}

void entityclass::customwarplinecheck(int i)
{
	if(i < 0 || i >= (int) entities.size()) {
		puts("customwarplinecheck() out-of-bounds!");
		return;
	}

	//Turns on obj.customwarpmodevon and obj.customwarpmodehon if player collides
	//with warp lines

	//We test entity to entity
	for(int j = 0; j < (int) entities.size(); j++) {
		if(i != j) {
			if(entities[i].rule == 0 && entities[j].rule == 5 //Player vs vertical line!
			   && (entities[j].type == 51 || entities[j].type == 52) && entitywarpvlinecollide(i, j)) {
				customwarpmodevon = true;
			}

			if(entities[i].rule == 0 && entities[j].rule == 7 //Player vs horizontal WARP line
			   && (entities[j].type == 53 || entities[j].type == 54) && entitywarphlinecollide(i, j)) {
				customwarpmodehon = true;
			}
		}
	}
}

void entityclass::entitycollisioncheck()
{
	std::vector<SDL_Surface*>& spritesvec = graphics.flipmode ? graphics.flipsprites : graphics.sprites;

	for(size_t i = 0; i < entities.size(); i++) {
		//We test entity to entity
		for(size_t j = 0; j < entities.size(); j++) {
			if(i != j) {
				if(entities[i].rule == 0 && entities[j].rule == 1 && entities[j].harmful) {
					//player i hits enemy or enemy bullet j
					if(entitycollide(i, j) && !map.invincibility) {
						if(entities[i].size == 0 && (entities[j].size == 0 || entities[j].size == 12)) {
							//They're both sprites, so do a per pixel collision
							colpoint1.x = entities[i].xp;
							colpoint1.y = entities[i].yp;
							colpoint2.x = entities[j].xp;
							colpoint2.y = entities[j].yp;
							int drawframe1 = entities[i].drawframe;
							int drawframe2 = entities[j].drawframe;
							if(INBOUNDS(drawframe1, spritesvec) && INBOUNDS(drawframe2, spritesvec) && graphics.Hitest(spritesvec[drawframe1], colpoint1, spritesvec[drawframe2], colpoint2)) {
								//Do the collision stuff
								game.deathseq = 30;
							}
						} else {
							//Ok, then we just assume a normal bounding box collision
							game.deathseq = 30;
						}
					}
				}
				if(entities[i].rule == 0 && entities[j].rule == 2) //Moving platforms
				{
					if(entitycollide(i, j)) removeblockat(entities[j].xp, entities[j].yp);
				}
				if(entities[i].rule == 0 && entities[j].rule == 3) //Entity to entity
				{
					if(entities[j].onentity > 0) {
						if(entitycollide(i, j)) entities[j].state = entities[j].onentity;
					}
				}
				if(entities[i].rule == 0 && entities[j].rule == 4) //Player vs horizontal line!
				{
					if(game.deathseq == -1) {
						//Here we compare the player's old position versus his new one versus the line.
						//All points either be above or below it. Otherwise, there was a collision this frame.
						if(entities[j].onentity > 0) {
							if(entityhlinecollide(i, j)) {
								music.playef(8);
								game.gravitycontrol = (game.gravitycontrol + 1) % 2;
								game.totalflips++;
								if(game.gravitycontrol == 0) {
									if(entities[i].vy < 1) entities[i].vy = 1;
								} else {
									if(entities[i].vy > -1) entities[i].vy = -1;
								}

								entities[j].state = entities[j].onentity;
								entities[j].life = 6;
							}
						}
					}
				}
				if(entities[i].rule == 0 && entities[j].rule == 5) //Player vs vertical line!
				{
					if(game.deathseq == -1) {
						if(entities[j].onentity > 0) {
							if(entityvlinecollide(i, j)) {
								entities[j].state = entities[j].onentity;
								entities[j].life = 4;
							}
						}
					}
				}
				if(entities[i].rule == 0 && entities[j].rule == 6) //Player versus crumbly blocks! Special case
				{
					if(entities[j].onentity > 0) {
						//ok; only check the actual collision if they're in a close proximity
						temp = entities[i].yp - entities[j].yp;
						if(temp > -30 && temp < 30) {
							temp = entities[i].xp - entities[j].xp;
							if(temp > -30 && temp < 30) {
								if(entitycollide(i, j)) entities[j].state = entities[j].onentity;
							}
						}
					}
				}
				if(game.supercrewmate) {
					//some extra collisions
					if(entities[i].type == 14) //i is the supercrewmate
					{
						if(entities[j].rule == 1 && entities[j].harmful) //j is a harmful enemy
						{
							//player i hits enemy or enemy bullet j
							if(entitycollide(i, j) && !map.invincibility) {
								if(entities[i].size == 0 && entities[j].size == 0) {
									//They're both sprites, so do a per pixel collision
									colpoint1.x = entities[i].xp;
									colpoint1.y = entities[i].yp;
									colpoint2.x = entities[j].xp;
									colpoint2.y = entities[j].yp;
									int drawframe1 = entities[i].drawframe;
									int drawframe2 = entities[j].drawframe;
									if(INBOUNDS(drawframe1, spritesvec) && INBOUNDS(drawframe2, spritesvec) && graphics.Hitest(spritesvec[drawframe1], colpoint1, spritesvec[drawframe2], colpoint2)) {
										//Do the collision stuff
										game.deathseq = 30;
										game.scmhurt = true;
									}
								} else {
									//Ok, then we just assume a normal bounding box collision
									game.deathseq = 30;
									game.scmhurt = true;
								}
							}
						}
						if(entities[j].rule == 2) //Moving platforms
						{
							if(entitycollide(i, j)) removeblockat(entities[j].xp, entities[j].yp);
						}
						if(entities[j].type == 8 && entities[j].rule == 3) //Entity to entity (well, checkpoints anyway!)
						{
							if(entities[j].onentity > 0) {
								if(entitycollide(i, j)) entities[j].state = entities[j].onentity;
							}
						}
					}
				}
			}
		}
	}

	//can't have the player being stuck...
	int j = getplayer();
	skipdirblocks = true;
	if(j > -1 && !testwallsx(j, entities[j].xp, entities[j].yp)) {
		//Let's try to get out...
		if(entities[j].rule == 0) {
			if(game.gravitycontrol == 0) {
				entities[j].yp -= 3;
			} else {
				entities[j].yp += 3;
			}
		}
	}
	skipdirblocks = false;

	//Can't have the supercrewmate getting stuck either!
	if(game.supercrewmate) {
		j = getscm();
		skipdirblocks = true;
		if(!testwallsx(j, entities[j].xp, entities[j].yp)) {
			//Let's try to get out...
			if(game.gravitycontrol == 0) {
				entities[j].yp -= 3;
			} else {
				entities[j].yp += 3;
			}
		}
		skipdirblocks = false;
	}

	//Is the player colliding with any damageblocks?
	if(checkdamage() && !map.invincibility) {
		//usual player dead stuff
		game.deathseq = 30;
	}

	//how about the supercrewmate?
	if(game.supercrewmate) {
		if(scmcheckdamage() && !map.invincibility) {
			//usual player dead stuff
			game.scmhurt = true;
			game.deathseq = 30;
		}
	}

	// WARNING: If updating this code, don't forget to update Map.cpp mapclass::twoframedelayfix()
	activetrigger = -1;
	if(checktrigger() > -1 && !script.nointerrupt) {
		game.state = activetrigger;
		game.statedelay = 0;
	}
}

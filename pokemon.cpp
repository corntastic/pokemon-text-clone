using namespace std;
/*

Arrays:
    pokedex: # of pokemon
    pokedexname: name of pokemon, index = #
    trainers: trainer job colum 1, name col 2
    movelist: name of moves
    party: list ot party pokemon. # level hp exp move1 move2 move3 move 4
    box: list of box pokes ^^
    pokestat: pokemon [base stats of HP ATK DEF SA SD SPD,
                                     evolve level, type1, type2]
    moveset: [pokemon][level]
    movestats [move#] [type, power, acc]
    map [x] [y]
    typechart [attacking type] [def type]
        norm fire water ele grass ice fight psn grnd fly psy bug rck ghost drg
        default
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
/* run this program using the console pauser or add your own getch,
 * system("pause") or input loop */
const string direct = "Go up/left/down/right? (w/a/s/d): ";

void pokeget(int, int, int, int);
void loadfile();
void savefile();
int trainerBattle(string job, string name, int templevel, int levelMod,
                  int SLMod, int pokemon0, int pokemon1 = 0, int pokemon2 = 0,
                  int pokemon3 = 0, int pokemon4 = 0, int pokemon5 = 0);
void gamestart();
int stat(string stat, int pokemon, int level);
int moves(int pokemon, int& level);
int moveselect(int enemypokemon, int enemylvl, int pPokemon);
double effective(int move, int pokemon);
void turnResult(int& eHP, int& pHP, int epoke, int ppoke, int elvl, int plvl,
                int emove, int pmove, int& estatus, int& pstatus, 
				double estats[], double pstats[]);
void xp(double trainer, int epoke, int elvl, int ppoke, int& pexp, int& plvl,
        int partypos);
char travel(int& x, int& y);
int bounds(int x, int y, char d);
int city(int x, int y);
int trainerpokemon(int level);
int wildBattle(int level, int levelMod, int pokemon);
int capture(int pokemon, int hp, int maxhp, int status);
void pokebox();
int wildpoke(int x, int y);
void gym(int x, int y);
void event(int& x, int& y, int flag[10][11]);
void partyorg();
void pokeheal();
string status(int intstatus);

int x, y;  // x and y coordinates
string pokedexname[152], trainers[30][2],
    movelist[167];  // pokemon names, trainer job/name, move names
int party[6][9], pokestat[152][9], moveset[152][93],
    movestats[167][18];  // party pokemon+stats, pokemon stats, pokemon movesets,
                        // move stats
double typechart[16][16];             // type chart
int box[30][9];                       // box pokemon
string map[10][11];                   // map names
char gender;                          // player gender
string pName;                         // planer name
int newgame = 0, badge[9] = {0};      // newgame flag, badges, 1 =  total
int garystarter;                      // garys starter pokemon
int mapbounds[10][11];                // 0/1 bounds
int maplvl[10][11];                   // pokemon/trainer level map
int tier1[23], tier2[43], tier3[65];  // tier list per trainer
int lastx = 2, lasty = 7;
int evFlags[10][11] = {0};  // event flags

int main() {
  srand(time(NULL));  // random seed

  loadfile();  // load arrays

  int i, j;

  if (newgame != 1)  // sets coordinates to pallet, deletes flags
  {
    x = 2;
    y = 7;

    for (i = 0; i < 10; i++)
      for (j = 0; j < 11; j++) evFlags[i][j] = 0;
  }

  char menu = 0;

  while (
      menu !=
      'q')  // q is quit command, checks events per area, then presents the menu
  {
    cout << "\t\t\t\t\n" << map[x][y] << "\n\n";
    event(x, y, evFlags);
    menu = travel(x, y);
  }

  savefile();  // saves file
  return 0;
}

char travel(int& x, int& y)  // map menu
{
  char action;

  //		cin.clear();
  //	cin.ignore(100, '\n');

  while (1) {
    int battleChance = rand() % 100;  // battle chance per area move
    cout << "What would you like to do? (press h for help): ";

    // cin.get(action);
    cin >> action;

    switch (action) {
      case 'w':                      // move up
        if (bounds(x, y, 'w') == 1)  // checks if up is within bounds
        {
          if (y == 0)  // double checking so you cant go around the world
          {
            cout << "You can't go that way!" << endl;
            continue;
          }

          else if (city(x, y) == 0)  // checks if its not a city
            if (battleChance < 7)    // checks for trainer battle
              if (trainerBattle(trainers[rand() % 30][0],
                                trainers[rand() % 30][1], maplvl[x][y], 3, 0,
                                trainerpokemon(maplvl[x][y]),
                                trainerpokemon(maplvl[x][y]),
                                trainerpokemon(maplvl[x][y]))) {
                y--;
                return 'w';
              } else  // else sends you to last town healed at
              {
                pokeheal();
                return 'w';
              }
            else if (battleChance < 55)  // checks if its a random battle
              if (wildBattle(maplvl[x][y], 3, wildpoke(x, y)) ==
                  1)  // checks if you win the wild battle
              {
                y--;
                return 'w';
              } else  // else sends you to last town healed at
              {
                pokeheal();
                return 'w';
              }
            else  // if not a random battle
            {
              y--;
              return 'w';
            }
          else  // if in a city
          {
            y--;
            return 'w';
          }
        } else  // if not within bounds
        {
          cout << "You can't go that way!" << endl;
          continue;
        }
      case 's':                      // move down
        if (bounds(x, y, 's') == 1)  // if down is within bounds
        {
          if ((x == 2 && y == 7) ||
              (x == 6 &&
               y == 8))  // if you need to surf from pallet or below fuschia
          {
            if (badge[0] > 5)  // need 6 badges
            {
              y++;
              return 's';
            } else  // if not 6 badges
            {
              cout << "You need 6 badges to surf!" << endl;
              continue;
            }
          } else if (y == 10)  // check to not go around the world
          {
            cout << "You can't go that way!" << endl;
            continue;
          }

          else  // if its not a surfable area
          {
            if (city(x, y) == 0)     // checks if its not a city
              if (battleChance < 7)  // checks for trainer battle
                if (trainerBattle(trainers[rand() % 30][0],
                                  trainers[rand() % 30][1], maplvl[x][y], 3, 0,
                                  trainerpokemon(maplvl[x][y]),
                                  trainerpokemon(maplvl[x][y]),
                                  trainerpokemon(maplvl[x][y]))) {
                  y++;
                  return 's';
                } else  // else sends you to last town healed at
                {
               		pokeheal();
                  return 's';
                }
              else if (battleChance < 55)  // checks if its a random battle
                if (wildBattle(maplvl[x][y], 3, wildpoke(x, y)) ==
                    1)  // if battle won, move down
                {
                  y++;
                  return 's';
                } else  // else moves to last pokecenter
                {
                	pokeheal();
                  return 's';
                }

              else  // if no battle
              {
                y++;
                return 's';
              }
            else  // if in a city
            {
              y++;
              return 's';
            }
          }
        } else  // if out of bounds
        {
          cout << "You can't go that way!" << endl;
          continue;
        }
      case 'd':                      // move right
        if (bounds(x, y, 'd') == 1)  // checks if in bounds
        {
          if (x == 9)  // checks its not looping
          {
            cout << "You can't go that way!" << endl;
            continue;
          } else if (city(x, y) == 0)  // checks not a city
            if (battleChance < 7)      // checks for trainer battle
              if (trainerBattle(trainers[rand() % 30][0],
                                trainers[rand() % 30][1], maplvl[x][y], 3, 0,
                                trainerpokemon(maplvl[x][y]),
                                trainerpokemon(maplvl[x][y]),
                                trainerpokemon(maplvl[x][y]))) {
                x++;
                return 'd';
              } else  // else sends you to last town healed at
              {
                pokeheal();
                return 'd';
              }
            else if (battleChance < 55)  // checks if its a random battle
              if (wildBattle(maplvl[x][y], 3, wildpoke(x, y)) ==
                  1)  // if player wins random battle
              {
                x++;  // move right
                return 'd';
              } else  // if losing battle, return to last pokecenter
              {
                pokeheal();
                return 'd';
              }
            else  // if no battle
            {
              x++;
              return 'd';
            }
          else  // if a city
          {
            x++;
            return 'd';
          }
        } else  // if out of bounds
        {
          cout << "You can't go that way!" << endl;
          continue;
        }
      case 'a':                      // move left
        if (bounds(x, y, 'a') == 1)  // if its within bounds
        {
          if (x == 0)  // so it doesnt roll over
          {
            cout << "You can't go that way!" << endl;
            continue;
          }

          else if (x == 2 && y == 5)  // if its viridian city
          {
            if (badge[0] > 7)  // check if there are enough badges
            {
              x--;
              return 'a';
            } else {
              cout << "You need 8 badges to challenge the Elite 4!" << endl;
              continue;
            }
          } else  // not viridian city
          {
            if (city(x, y) == 0)     // checks if its not a city
              if (battleChance < 7)  // checks for trainer battle
                if (trainerBattle(trainers[rand() % 30][0],
                                  trainers[rand() % 30][1], maplvl[x][y], 3, 0,
                                  trainerpokemon(maplvl[x][y]),
                                  trainerpokemon(maplvl[x][y]),
                                  trainerpokemon(maplvl[x][y]))) {
                  x--;
                  return 'a';
                } else  // else sends you to last town healed at
                {
               		pokeheal();
                  return 'a';
                }
              else if (battleChance < 55)  // checks if its a random battle
                if (wildBattle(maplvl[x][y], 3, wildpoke(x, y)) ==
                    1)  // if they win a wild battle
                {
                  x--;  // move left
                  return 'a';
                } else  // if lost battle, move back to last pokecenter
                {
                	pokeheal();
                  return 'a';
                }
              else  // if no battle
              {
                x--;
                return 'a';
              }
            else  // if city
            {
              x++;
              return 'a';
            }
          }
        } else  // if out of bounds
        {
          cout << "You can't go that way!" << endl;
          continue;
        }
      case 'h':  // help
        cout << "wasd\t- move cardinally \nt\t- fight trainers (outside of "
                "cities) \nc\t- fight wild pokemon (outside of cities) \ng\t- "
                "challenge the Gym (only in certain cities) \np\t- heal at the "
                "pokemon center (only in cities) \nb\t- access the box (only "
                "in cities) \nP\t- organize the party (capitals) \ng\t- "
                "challenge the gym (only in cities) \nS\t- save "
                "(capitals)\nq\t- save and quit"
             << endl;
        continue;
      case 'P':  // organize party
        partyorg();
        continue;
      case 'g':          // challenge gym
        if (city(x, y))  // if its in a city
          gym(x, y);     // run gym based on location
        else
          cout << "Gyms are only located in cities!" << endl;
        break;
      case 'S':  // save file
        savefile();
        continue;
      case 'b':  // access pokebox
        if (city(x, y)) {
          pokebox();
          break;
        } else
          cout << "There's no pokemon center out in the bush " << endl;
        continue;
      case 'q':  // quit
        return 'q';
      case 'p':  // heal pokemon if in a city
        if (city(x, y)) {
          lastx = x;
          lasty = y;
          
          pokeheal();
          continue;
        } else {
          cout << "Nurse Joy doesn't have an ambulance service! " << endl;
          continue;
        }

      case 't':  // trainer battle, back to last pokecenter if loss
        if (!city(x, y)) {
          int win;

          win = trainerBattle(
              trainers[rand() % 30][0], trainers[rand() % 30][1], maplvl[x][y],
              3, 0, trainerpokemon(maplvl[x][y]), trainerpokemon(maplvl[x][y]),
              trainerpokemon(maplvl[x][y]));

          if (win == 0) {
            pokeheal();
          }
          break;
        } else {
          cout << "All the trainers are in the gym! " << endl;
          continue;
        }
      case 'c':  // wild battle, back to pokecenter if loss
        if (!city(x, y)) {
          int win;

          win = wildBattle(maplvl[x][y], 3, wildpoke(x, y));

          if (win == 0) {
            pokeheal();
          }
          break;
        } else {
          cout << "There are no wild pokemon in the city! " << endl;
          continue;
        }

      default:  // if bad input, run again
        continue;
    }
    break;
  }
}

void event(int& x, int& y, int flag[10][11])  // scripted events
{
  int L = 0, win;
  if (flag[x][y] < 1) {
    switch (x) {
      case 0:  // indigo plateau, vic road, 23, 24 contd
      {
        switch (y) {
          case 2:  // indigo plateau
          {
            cout << "Guard: Welcome to the Pokemon League. \nGuard: Trainers "
                    "who have collected 8 badges have the opportunity to "
                    "challenge the Elite Four! \nGuard: You possess 8 badges, "
                    "please proceed!"
                 << endl;
            cout << "Advisor: Remember, you can call Nurse Joy to heal your "
                    "pokemon only twice during the Elite 4 Gauntlet!"
                 << endl;
            break;
          }        // end y case 2
          case 3:  // vic road
          {
            break;
          }        // end y case 3
          case 4:  // route 23
          {
            break;
          }        // end y case 4
          case 5:  // route 24 contd
          {
            break;
          }  // end y case 5
        }    // end switch y
        break;
      }        // end x case 0
      case 1:  // 24
      {
        switch (y) {
          case 5:  // route 24
          {
            L = 1;
            cout << "Gary: What? " << endl
                 << "Gary: " << pName << "! What a surprise to see you here!"
                 << endl
                 << "Gary: So, you're going to the Pokemon League? " << endl
                 << "Gary: You collected all the gym badges too? That's cool!"
                 << endl
                 << "Gary: Then I'll whip you, " << pName
                 << ", as a warmup for the Pokemon League!" << endl
                 << "Gary: Come on!" << endl;
            switch (garystarter) {
              case 1:
                win = trainerBattle("Gary", "Oak", 45, 0, 1, 18, 65, 111, 58,
                                    130, 3);
                break;
              case 4:
                win = trainerBattle("Champion", "Gary", 45, 0, 1, 18, 65, 111,
                                    130, 102, 6);
                break;
              case 7:
                win = trainerBattle("Champion", "Gary", 45, 0, 1, 18, 65, 111,
                                    102, 58, 9);
                break;
            }

            if (win == 0) {
              cout << "Gary: Hahaha!" << endl
                   << "Gary: " << pName << ", that's your best?" << endl
                   << "Gary: You're nowhere near as good as me, pal!" << endl
                   << "Gary: Go train some more, you loser!" << endl
                   << endl;
                pokeheal();
              flag[x][y] = -1;
              break;
            }

            cout << "Gary: That loosened me up! I'm ready for the Pokemon "
                    "League!"
                 << endl
                 << "Gary: " << pName << ", you need more practice." << endl
                 << "Gary: But hey, you know that! I'm out of here, smell ya!"
                 << endl;
            flag[x][y] = 1;
            break;
          }  // end y case 5
        }    // end switch y
        break;
      }        // end x case 1
      case 2:  // pewter, forest, 2, viridion, 1, pallet, 21 contd, 21, cinnibar
      {
        switch (y) {
          case 2:  // pewter city
          {
            break;
          }        // end y case 2
          case 3:  // viridian forest
          {
            break;
          }        // end y case 3
          case 4:  // route 2
          {
            break;
          }        // end y case 4
          case 5:  // viridian city
          {
            break;
          }        // end y case 5
          case 6:  // route 1
          {
            break;
          }        // end y case 6
          case 7:  // pallet town
          {
            gamestart();
            break;
          }        // end y case 7
          case 8:  // route 21 contd
          {
            break;
          }        // end y case 8
          case 9:  // route 21
          {
            break;
          }         // end y case 9
          case 10:  // cinnibar island
          {
            break;
          }  // end y case 10
        }    // end switch y
        break;
      }        // end x case 2
      case 3:  // 3, 20
      {
        switch (y) {
          case 2:  // route 3
          {
            break;
          }         // end y case 2
          case 10:  // route 20
          {
            break;
          }  // end y case 10
        }    // end switch y
        break;
      }        // end x case 3
      case 4:  // mt moon, 16, cycling road, seafoam islands
      {
        switch (y) {
          case 2:  // mt moon
          {
            break;
          }        // end y case 2
          case 4:  // route 16
          {
            break;
          }        // end y case 4
          case 5:  // cycling road 1
          {
            break;
          }        // end y case 5
          case 6:  // cycling road 2
          {
            break;
          }        // end y case 6
          case 7:  // cycling road 3
          {
            break;
          }        // end y case 7
          case 8:  // cycling road 4
          {
            break;
          }         // end y case 8
          case 10:  // seafoam islands
          {
            break;
          }  // end y case 10
        }    // end switch y
        break;
      }        // end x case 4
      case 5:  // 4, celadon, 18, 19 contd
      {
        switch (y) {
          case 2:  // route 4
          {
            break;
          }        // end y case 2
          case 4:  // celadon
          {
            break;
          }        // end y case 4
          case 8:  // route 18
          {
            break;
          }         // end y case 8
          case 10:  // route 19 contd
          {
            break;
          }  // end y case 10
        }    // end switch y
        break;
      }        // end x case 5
      case 6:  // 4 contd, 8, fuschia, 19, 19 contd
      {
        switch (y) {
          case 2:  // route 4 contd
          {
            break;
          }        // end y case 2
          case 4:  // route 8
          {
            break;
          }        // end y case 4
          case 8:  // fuschia city
          {
            break;
          }        // end y case 8
          case 9:  // route 19
          {
            break;
          }         // end y case 9
          case 10:  // route 19 contd
          {
            break;
          }  // end y case 10
        }    // end switch y
        break;
      }        // end x case 6
      case 7:  // 24 cont, 24, cer city, 5, saffron, 6, vermil, 15 cont
      {
        switch (y) {
          case 0:  // 24 cont
          {
            break;
          }        // end y case 0
          case 1:  // 24
          {
            break;
          }        // end y case 1
          case 2:  // cerulean city
          {
            break;
          }        // end y case 2
          case 3:  // route 5
          {
            break;
          }        // end y case 3
          case 4:  // saffron city
          {
            break;
          }        // end y case 4
          case 5:  // route 6
          {
            break;
          }        // end y case 5
          case 6:  // vermillion city
          {
            break;
          }        // end y case 6
          case 8:  // route 15 contd
          {
            break;
          }  // end y case 8
        }    // end switch y
        break;
      }        // end x case 7
      case 8:  // 25, 9, 7, 11, 15
      {
        switch (y) {
          case 0:  // route 25
          {
            break;
          }        // end y case 0
          case 2:  // route 9
          {
            break;
          }        // end y case 2
          case 4:  // route 7
          {
            break;
          }        // end y case 4
          case 6:  // route 11
          {
            break;
          }        // end y case 6
          case 8:  // route 15
          {
            break;
          }  // end y case 8
        }    // end switch y
        break;
      }        // end x case 8
      case 9:  // route 9 contd, rock tunnel, lav town, 12, 12 contd, 13, 14
      {
        switch (y) {
          case 2:  // route 9 contd
          {
            break;
          }        // end y case 2
          case 3:  // rock tunnel
          {
            break;
          }        // end y case 3
          case 4:  // lavender town
          {
            break;
          }        // end y case 4
          case 5:  // route 12
          {
            break;
          }        // end y case 5
          case 6:  // route 12 contd
          {
            break;
          }        // end y case 6
          case 7:  // route 13
          {
            break;
          }        // end y case 7
          case 8:  // route 14
          {
            break;
          }  // end y case 8
        }    // end switch y
        break;
      }  // end x case 9
    }
  }
  if (flag[x][y] == 0) flag[x][y] = 1;
  if (L == 1) cout << "\t\t\t\t\n" << map[x][y] << "\n\n";
}

int bounds(int x, int y,
           char d)  // checks whether movment request is out of bounds
{
  ifstream maploader;
  int i, j;

  maploader.open("map bounds.txt");
  for (i = 0; i < 11; i++)
    for (j = 0; j < 10; j++) maploader >> mapbounds[j][i];

  switch (d) {
    case 'w':
      return mapbounds[x][y - 1];
      break;
    case 'a':
      return mapbounds[x - 1][y];
      break;
    case 'd':
      return mapbounds[x + 1][y];
      break;
    case 's':
      return mapbounds[x][y + 1];
      break;
  }

  return 1;
}

void gym(int x, int y)  // gym script, if through the gauntlet, award badge and
                        // increment total
{
  int win = 1;
  int joy = 2;
  char choice;
  switch (y) {
    case 2:
      if (x == 0)  // e4
      {
        // elite 4

        if (win == 1) {
          // lorelei
          cout << "Lorelei: Welcome to the Pokemon League! \n"
               << "Lorelei: I am Lorelei of the Elite 4! \n"
               << "Lorelei: No one can best me when it comes to icy pokemon!\n"
               << "Lorelei: Freezing moves are powerful! \n"
               << "Lorelei: Your pokemon will be at my mercy when they're "
                  "frozen solid! \n"
               << "Lorelei: Hahaha! Are you ready? " << endl;
          if (trainerBattle("Elite 4", "Lorelei", 52, 0, 1, 87, 91, 80, 124,
                            131)) {
            cout << "Lorelei: How dare you! \n"
                 << "Lorelei: You're better than I thought! Go on ahead! \n"
                 << "Lorelei: You only got a taste of the Pokemon League's "
                    "power!"
                 << endl;
            if (joy > 0)  // call nurse joy
            {
              cout << "Call Nurse Joy (p) or proceed (w)? ";
              cin >> choice;
              if (choice == 'p') {
              	lastx = x;
              	lasty = y;
                pokeheal();
                joy--;
              }
            }
          } else
            win = 0;
        }

        if (win == 1) {
          // Bruno
          cout << "Bruno: I am Bruno of the Elite 4!\n"
               << "Bruno: Through rigorous training, people and pokemon can "
                  "become stronger! \n"
               << "Bruno: I've weight trained with my pokemon! \n"
               << "Bruno: " << pName << "!\n"
               << "Bruno: We will grind you down with our superior power! \n"
               << "Bruno: Hoo hah!" << endl;

          if (trainerBattle("Elite 4", "Bruno", 54, 0, 1, 95, 107, 106, 95,
                            68)) {
            cout << "Bruno: Why? How could I lose? \n"
                 << "Bruno: My job is done! Go face your next challenge!"
                 << endl;

            if (joy > 0)  // call nurse joy
            {
              cout << "Call Nurse Joy (p) or proceed (w)? ";
              cin >> choice;
              if (choice == 'p') {
              	lastx = x;
              	lasty = y;
                pokeheal();
                joy--;
              }
            }
          } else
            win = 0;
        }

        if (win == 1) {
          // agatha

          cout << "Agatha: I am Agatha of the Elite 4,\n"
               << "Agatha: Oak's taken a lot of interest in you, child! \n"
               << "Agatha: That old duff was once tough and handsome! That was "
                  "decades ago! \n"
               << "Agatha: Now he just wants to fiddle with his pokedex! He's "
                  "wrong!\n"
               << "Agatha: Pokemon are for fighting! \n"
               << "Agatha: " << pName
               << ", I'll show you how a real trainer fights!" << endl;

          if (trainerBattle("Elite 4", "Agatha", 55, 0, 1, 94, 42, 93, 24,
                            94)) {
            cout << "Agatha: Oh ho! You're something special, child! \n"
                 << "Agatha: You win! I see what the old duff sees in you now! "
                    "\n"
                 << "Agatha: I have nothing else to say. Run along now, child!"
                 << endl;

            if (joy > 0)  // call nurse joy
            {
              cout << "Call Nurse Joy (p) or proceed (w)? ";
              cin >> choice;
              if (choice == 'p') {
              	lastx = x;
              	lasty = y;
                pokeheal();
                joy--;
              }
            }

          } else
            win = 0;
        }

        if (win == 1) {
          // lance
          cout << "Lance: Ah! I heard about you, " << pName << "!\n"
               << "Lance: I lead the Elite 4. You can call me Lance, the "
                  "dragon trainer! \n"
               << "Lance: You know that dragons are mythical pokemon. \n"
               << "Lance: They're hard to catch and raise, but their powers "
                  "are superior! \n"
               << "Lance: They're virtually indestructable!\n"
               << "Lance: Well? Are you ready to lose?\n"
               << "Lance: Your league challenge ends with me, " << pName << "!"
               << endl;

          if (trainerBattle("Elite 4", "Lance", 58, 0, 1, 130, 148, 148, 142,
                            149)) {
            cout
                << "Lance: That's it!\n"
                << "Lance: I hate to admit it, but you are the pokemon "
                   "master!\n"
                << "Lance: I still can't believe my dragons lost to you, "
                << pName << "!\n"
                << "Lance: You are now the Pokemon League Champion!\n"
                << "Lance: . . . Or you would have been, but you have one more "
                   "challenge ahead. \n"
                << "Lance: You have to face another trainer! His name is. . .\n"
                << "Lance: Gary!\n"
                << "Lance: He beat the Elite 4 before you!\n"
                << "Lance: He is the real Pokemon League Champion!" << endl;

            if (joy > 0)  // call nurse joy
            {
              cout << "Call Nurse Joy (p) or proceed (w)? ";
              cin >> choice;
              if (choice == 'p') {
              	lastx = x;
              	lasty = y;
                pokeheal();
                joy--;
              }
            }
          } else
            win = 0;
        }

        if (win == 1) {
          // gary
          // cin = trainerBattle("Champion", "Gary", 52, 0, 1, 87, 91, 80, 124,
          // 131, int pokemon5)

          cout << "Gary: Hey! \n"
               << "Gary: I was looking forward to seeing you, " << pName
               << "!\n"
               << "Gary: My rival should be strong to keep me sharp! \n"
               << "Gary: While working on my pokedex, I looked all over for "
                  "powerful pokemon. \n"
               << "Gary: Not only that, I assembled teams that could beat any "
                  "pokemon type! \n"
               << "Gary: And now!\n"
               << "Gary: I am the Pokemon League Champion!\n"
               << "Gary: " << pName << "! Do you know what that means?! \n"
               << "Gary: I'll tell you! \n"
               << "Gary: I am the most powerful trainer in the world! " << endl;

          switch (garystarter) {
            case 1:
              win = trainerBattle("Champion", "Gary", 60, 0, 1, 18, 65, 112, 59,
                                  130, 3);
              break;
            case 4:
              win = trainerBattle("Champion", "Gary", 60, 0, 1, 18, 65, 112,
                                  130, 103, 6);
              break;
            case 7:
              win = trainerBattle("Champion", "Gary", 60, 0, 1, 18, 65, 112,
                                  103, 59, 9);
              break;
          }

          if (win == 0) {
            cout << "Gary: Hahaha! I won, I won! \n"
                 << "Gary: I'm too good for you, " << pName << "!\n"
                 << "Gary: You did well to even reach me, Gary, the pokemon "
                    "genius! \n"
                 << "Gary: Nice try, loser! Hahaha!" << endl;
          }
        }

        if (win == 0)  // loss to gary or anyone else
        {
          pokeheal();
          break;
        } else  // win against gary
        {
          cout << "Gary: NO!\n"
               << "Gary: That can't be! You beat me at my best!\n"
               << "Gary: After all that work to become the League champ?! \n"
               << "Gary: My reign is over already?! It's not fair! \n"
               << "Gary: Why?! Why did I lose?! \n"
               << "Gary: I never made any mistakes raising my pokemon. . . \n"
               << "Gary: Damn it! You're the new Pokemon League Champion! \n"
               << "Gary: Although I don't like to admit it. . ." << endl
               << endl;

          cout << "Oak: " << pName << "! \n"
               << "Oak: So, you won! Congratulations! You're the new Pokemon "
                  "League Champion!\n"
               << "Oak: You've grown up so much since you first left with "
               << ((garystarter == 1) ? pokedexname[7]
                                      : pokedexname[garystarter - 3])
               << "!\n"
               << "Oak: " << pName << ", you have come of age! \n"
               << "Oak: Gary, I'm disappointed! \n"
               << "Oak: I came when I heard you beat the Elite 4! \n"
               << "Oak: But when I got here, you had already lost! \n"
               << "Oak: Gary, do you understand why you lost? \n"
               << "Oak: You have forgotten to treat your pokemon with trust "
                  "and love! \n"
               << "Oak: Without them, you will never become the Champion "
                  "again! \n"
               << "Oak: " << pName << "!\n"
               << "Oak: You understand that your victory was not just your own "
                  "doing! \n"
               << "Oak: The bond you share with your pokemon is marvelous! \n"
               << "Oak: " << pName << ", come with me! \n"
               << "\nProfessor Oak leads you beyond the door, into a dark "
                  "room. \n\n"
               << "Oak: Er-hem! Congratulations, " << pName << "!\n"
               << "Oak: This floor is the Pokemon Hall of Fame! \n"
               << "Oak: Pokemon League Champions are honoured for their "
                  "exploits here. \n"
               << "Oak: Their pokemon are also recorded in the Hall of Fame. \n"
               << "Oak: " << pName << ", You have endeavoured hard to become "
                                      "the new League Champion! \n"
               << "Oak: Congratulations, " << pName
               << ", you and your pokemon are Hall of Famers!\n" << endl;

          time_t now = time(0);
          char* dt = ctime(&now);
          ofstream hall;
          hall.open("hall of fame.txt", ios::app);
          hall << pName << '\t' << dt << endl;
          for (int i = 0; i < 6; i++)
            hall << '\t' << pokedexname[party[i][0]] << '\t' << party[i][1]
                 << endl;
          hall << endl;

          hall.close();

          ::x = 2;
          ::y = 7;
        }

      } else if (x == 2 && badge[1] != 1)  // pewter
      {
        // pewter gym
        if (win == 1) {
          cout << "Disciple: You're lightyears away from facing Brock!" << endl;
          win = trainerBattle("Gym", "Disciple", 8, 0, 1, 41, 27);
          if (win == 1)
            cout << "Disciple: Wait, lightyears measures distance, not time!"
                 << endl
                 << endl;
        }

        if (win == 1) {
          cout << "Brock: I'm Brock! I'm Pewter's Gym leaeder! \nBrock: I "
                  "believe in rock-hard defense and determination!\nBrock: "
                  "That's why all my pokemon are rock-type!\nBrock: Do you "
                  "still want to challenge me? Fine then, show me your best!\n"
               << endl;
          if (trainerBattle("Leader", "Brock", 12, 0, 2, 74, 95)) {
            cout << "Brock: I took you for granted.\nBrock: As proof of your "
                    "victory, here's the Boulder Badge!"
                 << endl
                 << pName << " received the Boulder Badge!" << endl
                 << "Brock: That's an official Pokemon League badge. It's "
                    "bearer becomes more powerful.\n"
                 << "Brock: There are all kinds of trainers in the "
                    "world.\nBrock: You appear to be very gifted as a pokemon "
                    "trianer. \nBrock: Go to the gym in Cerulean City and test "
                    "your abilities!\n"
                 << endl;
            badge[1] = 1;
            badge[0] += 1;
            break;
          }
        }

      	lastx = x;
      	lasty = y;
        pokeheal();
        break;
      } else if (x == 7 && badge[2] != 1)  // cerulean
      {
        // cerulean gym

        if (win == 1) {
          cout << "Disciple: Misty can wait!" << endl;
          win = trainerBattle("Gym", "Disciple", 15, 0, 1, 118, 60);
          if (win == 1)
            cout << "Disciple: Oh no, I couldn't stop you!" << endl << endl;
        }

        if (win == 1) {
          cout << "Disciple: Misty is training right now, don't disturb her!"
               << endl;
          win = trainerBattle("Gym", "Disciple", 16, 0, 1, 116);
          if (win == 1) cout << "Disciple: Sorry Misty!" << endl << endl;
        }

        if (win == 1) {
          cout << "Misty: Hi, you're a new face!\nMisty: Trainers who want to "
                  "turn pro have to have a policy about pokemon.\nMisty: My "
                  "policy is an all-out offensive with water-type pokemon!\n"
               << endl;
          if (trainerBattle("Leader", "Misty", 18, 0, 3, 120, 121)) {
            cout << "Misty: Wow! You're too much!\nMisty: All right!\nMisty: "
                    "You can have the Cascade Badge to show you beat me!"
                 << endl;
            cout << pName << " received the Cascade Badge! " << endl << endl;
            badge[2] = 1;
            badge[0] += 1;
            break;
          }
        }

      	lastx = x;
      	lasty = y;
        pokeheal();
        break;

      } else
        ;
      break;
    case 4:
      if (x == 5 && badge[4] != 1)  // celadon
      {
        if (win == 1) {
          cout << "Disciple: Hey! No boys allowed!" << endl;
          win = trainerBattle("Gym", "Disciple", 20, 0, 1, 43);
          if (win == 1)
            cout << "Disciple: Don't move, I'm going to call the cops!" << endl
                 << endl;
        }

        if (win == 1) {
          cout << "Disciple: How did you get past the femguard?" << endl;
          win = trainerBattle("Gym", "Disciple", 21, 0, 1, 46, 102);
          if (win == 1)
            cout << "Disciple: I'll have to fix that. . ." << endl << endl;
        }
        // celadon gym
        if (win == 1) {
          cout << "Erika: Hello. Lovely weather, isn't it? It's so "
                  "pleasant.\nErika: . . . Oh dear. . . I must have dozed off. "
                  "Welcome\nErika: My name is Erika. I am the leader of the "
                  "Celadon Gym.\nErika: I teach the art of flower arranging. "
                  "My pokemon are of the grass type.\nErika: Oh, I'm sorry. I "
                  "had no idea that you wished to challenge me.\nErika: Very "
                  "well, but I shall not lose!"
               << endl;

          if (trainerBattle("Leader", "Erika", 24, 0, 2, 114, 71, 45)) {
            cout << "Erika: Oh! I concede defeat\nErika: You are remarkably "
                    "strong.\nErika: I must confer to you the Rainbow Badge."
                 << endl;
            cout << pName << " recieved the Rainbow Badge! " << endl;
            cout << "Erika: You are cataloging pokemon? I must say I'm "
                    "impressed.\n Erika: I would never collect pokemon if they "
                    "were unattractive. \n"
                 << endl;
            badge[4] = 1;
            badge[0] += 1;
            break;
          }
        }

        
      	lastx = x;
      	lasty = y;
        pokeheal();
        break;

      } else if (x == 7 && badge[6] != 1)  // saffron
      {
        // saffron gym
        if (win == 1) {
          cout << "Disciple: Sabrina is the coolest!" << endl;
          win = trainerBattle("Gym", "Disciple", 30, 0, 1, 96);
          if (win == 1)
            cout << "Disciple: Cooler than you anyway!" << endl << endl;
        }

        if (win == 1) {
          cout << "Disciple: She can read minds, man!" << endl;
          win = trainerBattle("Gym", "Disciple", 32, 0, 1, 124);
          if (win == 1)
            cout << "Disciple: You can't escape! Once she catches you. . .!"
                 << endl;
        }

        if (win == 1) {
          cout << "Disciple: HELP ME!" << endl;
          win = trainerBattle("Gym", "Disciple", 33, 0, 1, 80);
          if (win == 1) cout << "Disciple: . . ." << endl;
        }

        if (win == 1) {
          cout << "Sabrina: I had a vision of your arrival!\nSabrina: I have "
                  "had psychic powers ever since I was a child. \nSabrina: I "
                  "first learned to bend spoons with my mind. \nSabrina: I "
                  "dislike fighting, but if you wish, I will show you my "
                  "powers!\n"
               << endl;
          if (trainerBattle("Leader", "Sabrina", 38, 0, 2, 64, 122, 49, 65)) {
            cout << "Sabrina: I'm shocked! But, a loss is a loss. \nSabrina: I "
                    "admit, I didn't work hard enough to win! \nSabrina: You "
                    "earned the Marsh Badge!"
                 << endl;
            cout << pName << " received the Marsh Badge!" << endl;
            cout << "Sabrina: Everyone has psychic powers. Some people just "
                    "don't realize it yet! \n"
                 << endl;
            badge[6] = 1;
            badge[0] += 1;
            break;
          }
        }
        
      	lastx = x;
      	lasty = y;
        pokeheal();
        break;

      } else
        ;
      break;
    case 5:
      if (x == 2 && badge[0] == 7 && badge[8] != 1)  // virid
      {
        // viridion gym
        if (win == 1) {
          cout << "Disciple: I bet you're dying to know who our boss is!"
               << endl;
          win = trainerBattle("Gym", "Disciple", 34, 0, 1, 104, 104);
          if (win == 1) cout << "Disciple: I'm sorry G--hnggg!" << endl;
        }

        if (win == 1) {
          cout << "Disciple: Our leader is finally back!" << endl;
          win = trainerBattle("Gym", "Disciple", 35, 0, 1, 27, 50);
          if (win == 1) cout << "Disciple: I'm sorry G--hnggg!" << endl;
        }

        if (win == 1) {
          cout << "Disciple: You can't fight our leader till you get past me!"
               << endl;
          win = trainerBattle("Gym", "Disciple", 36, 0, 1, 111, 128);
          if (win == 1)
            cout << "Disciple: Good luck, Gi--erhem, leader." << endl;
        }

        if (win == 1) {
          cout << "Giovanni: Fwahahaha! This is my hideout! \nGiovanni: I "
                  "planned to resurrect Team Rocket here! \nGiovanni: But you "
                  "have caught me agian! So be it. \nGiovanni: This time, I'm "
                  "not holding back!\nGiovanni: One more, you shall face "
                  "Giovanni, the greatest trainer!\n"
               << endl;

          if (trainerBattle("Leader", "Giovanni", 42, 0, 2, 53, 111, 31, 34,
                            112)) {
            cout << "Giovanni: Ha! That was truly an intense fight!\nGiovanni: "
                    "You have won. As proof, here is the Earth Badge!"
                 << endl;
            cout << pName << " has received the Earth Badge! " << endl;
            cout << "Giovanni: The Earth Badge makes pokemon of any level "
                    "obey.\nGiovanni: It is evidence of your mastery as a "
                    "pokemon trainer!\nGiovanni: With it, you can enter the "
                    "Pokemon League!\nGiovanni: It is my gift for your Pokemon "
                    "League challenge."
                 << endl;
            cout << "Giovanni: Having lost, I cannot face my underlings! "
                    "\nGiovanni: Team Rocket is finished forever! \nGiovanni: "
                    "I will dedicate my life to the study of "
                    "Pokemon!\nGiovanni: Let us meet again someday. Farewell!\n"
                 << endl;
            badge[8] = 1;
            badge[0] += 1;
            break;
          }
        }

       
      	lastx = x;
      	lasty = y;
        pokeheal();
        break;

      } else {
        cout << "It appears to be locked. . ." << endl;
      }
      break;
    case 6:
      if (x == 7 && badge[3] != 1)  // vermil
      {
        // vermillion gym
        if (win == 1) {
          cout << "Disciple: Surge saved my life during the war!" << endl;
          win = trainerBattle("Gym", "Disciple", 15, 0, 1, 100, 100);
          if (win == 1) cout << "Disciple: Semper fi!" << endl;
        }

        if (win == 1) {
          cout << "Lt. Surge: Hey kid! What do you think you're doing in "
                  "here?\nLt. Surge: You won't live long in combat, that's for "
                  "sure!\nLt. Surge: I tell you kid, electric pokemon saved me "
                  "during the war!\nLt. Surge: They zapped my enemies into "
                  "paralysis!\nLt. Surge: The same I'll do to you!\n"
               << endl;
          if (trainerBattle("Leader", "Lt. Surge", 18, 0, 3, 25, 100, 26)) {
            cout << "Lt. Surge: Whoa! You're the real deal, kid!\nLt. Surge: "
                    "Fine then, take the Thunder Badge!"
                 << endl;
            cout << pName << " received the Thunder Badge!" << endl;
            cout << "Lt. Surge: A little word of advice, kid! \nLt. Surge: "
                    "Electricity sure is powerful! \nLt. Surge: But it's "
                    "useless against ground pokemon!\n"
                 << endl;
            badge[1] = 1;
            badge[0] += 1;
            break;
          }
        }

        
      	lastx = x;
      	lasty = y;
        pokeheal();
        break;
      }
      break;
    case 8:
      if (x == 6 && badge[5] != 1)  // fuschia
      {
        // fuschia gym
        if (win == 1) {
          cout << "Disciple: Haha, did you see my trap?" << endl;
          win = trainerBattle("Gym", "Disciple", 30, 0, 1, 23, 48);
          if (win == 1)
            cout << "Disciple: Oops, that was just my trap card!" << endl;
        }

        if (win == 1) {
          cout << "Disciple: We're all going to become the Hokage!" << endl;
          win = trainerBattle("Gym", "Disciple", 30, 0, 1, 70, 42, 88);
          if (win == 1)
            cout << "Disciple: Maybe piracy is a better career path. . ."
                 << endl;
        }

        if (win == 1) {
          cout << "Koga: Fwahahaha! \nKoga: A mere child like you dares to "
                  "challenge me? \nKoga: Very well, I shall show you the true "
                  "terror as a ninja master! \nKoga: You shall feel the "
                  "despair of poison and sleep techniques! \n"
               << endl;
          if (trainerBattle("Leader", "Koga", 37, 0, 2, 109, 89, 109, 110)) {
            cout << "Koga: Hmph! You have proven your worth! \nKoga: Here! "
                    "Take the Soul Badge!"
                 << endl;
            cout << pName << " received the Soul Badge!\n " << endl;
            badge[1] = 1;
            badge[0] += 1;
            break;
          }
        }

        
      	lastx = x;
      	lasty = y;
        pokeheal();
        break;
      }
      break;
    case 10:
      if (x == 2 && badge[7] != 1)  // cinbar
      {
        // cinnibar gym
        if (win == 1) {
          cout << "Disciple: They say Blaine survived because of fire pokemon!"
               << endl;
          win = trainerBattle("Gym", "Disciple", 36, 0, 1, 5, 37);
          if (win == 1)
            cout << "Disciple: He'll crush you, just not me!" << endl;
        }

        if (win == 1) {
          cout << "Disciple: I love playing with fire and wetting the bed!"
               << endl;
          win = trainerBattle("Gym", "Disciple", 36, 0, 1, 58, 77, 136);
          if (win == 1)
            cout << "Disciple: Ouch! I've got blisters on my fingers!" << endl;
        }

        if (win == 1) {
          cout << "Blaine: Hah! \nBlaine: I am Blaine! I am the leader of "
                  "Cinnibar Gym!\nBlaine: My fiery pokemon will incinerate all "
                  "challengers! \nBlaine: Hah! You better have burn heal!\n"
               << endl;
          if (trainerBattle("Leader", "Blaine", 42, 0, 2, 58, 77, 78, 43)) {
            cout << "Blaine: I have burnt out!\nBlaine: You have earned the "
                    "Volcano Badge!"
                 << endl;
            cout << pName << " receieved the Volcano Badge! \n" << endl;
            badge[1] = 1;
            badge[0] += 1;
            break;
          }
        }
        
      	lastx = x;
      	lasty = y;
        pokeheal();
        break;
      }
      break;
    default:
      cout << "There is no gym in this city" << endl;
  }

  return;
}

string status(int intstatus)
{  		
	string estatus;
	
	switch(intstatus){
		case 0:
			estatus = "";
			break;
		case 1:
			estatus = "Psn";
			break;
		case 2:
			estatus = "Burn";
			break;
		case 3:
			estatus = "Prlz";
			break;
		case 4:
			estatus = "Frz";
			break;
		case 5:
			estatus = "Slp";
			break;
	}
	
	return estatus;
}
	
void pokebox()  // box functions
{
  int choice, i, quit = 0;
  cout << "Booted up the PC" << endl;

  do {
    cout << "(1): Deposit a pokemon\n(2): Withdraw a pokemon\n(3): Release a "
            "pokemon\n(4): Quit\n";
    cin >> choice;

    while (choice != 1 && choice != 2 && choice != 3 && choice != 4) {
      cin.clear();
      cin.ignore(100, '\n');
      cin >> choice;
    }

    switch (choice) {
      case 1: {
        if (box[29][0] != 0) {
          cout << "The box is full!" << endl;
          break;
        }

        cout << "Deposit which pokemon?" << endl;

        for (i = 0; party[i][0] != 0; i++)
          cout << "(" << i + 1 << "): " << pokedexname[party[i][0]]
               << " - lvl. " << party[i][1] << endl;
        cout << "(" << i + 1 << "): Go back" << endl;

        cin >> choice;

        while (cin.fail()) {
          cin.clear();
          cin.ignore();
          cin >> choice;
        }

        if (choice == i + 1) break;

        cout << "Transferred " << pokedexname[party[--choice][0]]
             << " to the box." << endl;
        int j = 0;
        while (box[j][0] != 0) j++;
        for (int k = 0; k < 8; k++) box[j][k] = party[choice][k];
        for (i = choice; i < 5; i++)
          for (j = 0; j < 9; j++) party[i][j] = party[i + 1][j];
        for (j = 0; j < 9; j++) party[5][j] = 0;
        break;
      }
      case 2: {
        if (party[5][0] != 0) {
          cout << "The party is full!" << endl;
          break;
        }
        cout << "Withdraw which pokemon?" << endl;

        for (i = 0; box[i][0] != 0; i++)
          cout << "(" << i + 1 << "): " << pokedexname[box[i][0]] << " - lvl. "
               << box[i][1] << endl;
        cout << "(" << i + 1 << "): Go back" << endl;

        //	i--;
        cin >> choice;

        while (cin.fail()) {
          cin.clear();
          cin.ignore();
          cin >> choice;
        }

        if (choice == i + 1) break;

        cout << "Transferred " << pokedexname[box[--choice][0]]
             << " to the party." << endl;
        int j = choice;
        while (party[j][0] != 0) j++;
        for (int k = 0; k < 8; k++) party[j][k] = box[choice][k];
        for (i = choice; i < 29; i++)
          for (j = 0; j < 8; j++) box[i][j] = box[i + 1][j];
        for (j = 0; j < 8; j++) box[29][j] = 0;
        break;
      }
      case 3: {
        cout << "Release which pokemon?" << endl;
        for (i = 0; box[i][0] != 0; i++)
          cout << "(" << i + 1 << "): " << pokedexname[box[i][0]] << " - lvl. "
               << box[i][1] << endl;
        cout << "(" << i + 1 << "): Go back" << endl;

        cin >> choice;
        while (cin.fail()) {
          cin.clear();
          cin.ignore();
          cin >> choice;
        }

        if (choice == i + 1) break;

        cout << "Released " << pokedexname[box[--choice][0]]
             << " back to the wild." << endl;
        cout << "Goodbye " << pokedexname[box[choice][0]] << "!" << endl;
        for (i = choice; i < 29; i++)
          for (int j = 0; j < 8; j++) box[i][j] = box[i + 1][j];
        for (int j = 0; j < 8; j++) box[29][j] = 0;
        break;
      }
      case 4: {
        cout << pName << " powered off the PC" << endl;
        quit = 1;
      }
    }

  } while (quit != 1);
}

void partyorg() 
{
  int i, pokemon1, pokemon2;
  char choice = 't';
  for (i = 0; i < 6; i++)
    if (party[i][0] != 0)
      cout << "(" << i + 1 << ") " << setw(10) << pokedexname[party[i][0]]
           << '\t' << "Lvl " << party[i][1] << '\t' << party[i][2] << "/"
           << stat("HP", party[i][0], party[i][1]) << setw(5) << status(party[i][8]) << endl;

  cout << "Choose a pokemon: ";
  cin >> pokemon1;
  while (cin.fail())  // bad inputcheck
  {
    cin.clear();
    cin.ignore();
    cin >> pokemon2;
  }

  while ((pokemon1 <= 1 && pokemon1 >= 6) || party[pokemon1 - 1][0] == 0) {
    cin >> pokemon1;
  }
  pokemon1--;

  while (choice != 'q') {
    cout << "What would you like to do with that pokemon (press h for help)? ";
    cin >> choice;
    switch (choice) {
      case 'h':
        cout << "p\t- swap pokemon positions with another pokemon\ns\t- view "
                "the stats of the pokemon\nq\t- quit"
             << endl;
        continue;
      case 'p':
        int temppoke[8];

        for (i = 0; i < 6; i++)
          if (party[i][0] != 0 && party[i][0] != party[pokemon1][0])
            cout << "(" << i + 1 << ") " << setw(10) << pokedexname[party[i][0]]
                 << '\t' << "Lvl " << party[i][1] << '\t' << party[i][2] << "/"
                 << stat("HP", party[i][0], party[i][1]) << setw(5) << status(party[i][8]) << endl;

        cout << "Choose another pokemon to swap with: ";
        cin >> pokemon2;
        while (cin.fail())  // bad inputcheck
        {
          cin.clear();
          cin.ignore();
          cin >> pokemon2;
        }
        while ((pokemon2 <= 1 && pokemon2 >= 6) ||
               party[pokemon2 - 1][0] == 0) {
          cin.clear();
          cin.ignore(100, '\n');
          cin >> pokemon2;
        }

        pokemon2--;
        for (i = 0; i < 9; i++) {
          temppoke[i] = party[pokemon2][i];
          party[pokemon2][i] = party[pokemon1][i];
          party[pokemon1][i] = temppoke[i];
        }
        break;
      case 's':
        cout << '\n' << pokedexname[party[pokemon1][0]] << '\n'
             << "Level: " << party[pokemon1][1] << '\n' << "EXP to next level: "
             << pow(party[pokemon1][1] + 1, 3) - party[pokemon1][3] << '\n'
             << "HP: " << party[pokemon1][2] << "/"
             << stat("HP", party[pokemon1][0], party[pokemon1][1]) << '\n'
             << "Status: " << ((party[pokemon1][8] == 0) ? "Healthy" : status(party[pokemon1][8])) << '\n'
             << "Attack: " << stat("A", party[pokemon1][0], party[pokemon1][1])
             << '\n'
             << "Defense: " << stat("D", party[pokemon1][0], party[pokemon1][1])
             << '\n' << "Special Attack: "
             << stat("SA", party[pokemon1][0], party[pokemon1][1]) << '\n'
             << "Special Defense: "
             << stat("SD", party[pokemon1][0], party[pokemon1][1]) << '\n'
             << "Speed: " << stat("S", party[pokemon1][0], party[pokemon1][1])
             << '\n' << "Knows: " << movelist[party[pokemon1][4]] << '\t'
             << movelist[party[pokemon1][5]] << '\t'
             << movelist[party[pokemon1][6]] << '\t'
             << movelist[party[pokemon1][7]] << endl;

        break;
      default:
        continue;
    }
    break;
  }
}

int trainerpokemon(int level)  // determines trainers pokemon tier
{
  if (level < 17)
    return tier1[rand() % 23];
  else if (level < 35)
    return tier2[rand() % 43];
  else
    return tier3[rand() % 65];
}

int city(int x, int y)  // checks if you're in a city
{
  int cities[11][2] = {{0, 2}, {2, 2}, {2, 5}, {2, 7}, {2, 10}, {5, 4},
                       {6, 8}, {7, 2}, {7, 4}, {7, 6}, {9, 4}};

  for (int i = 0; i < 11; i++)
    if (x == cities[i][0] && (y == cities[i][1])) return 1;

  return 0;
}

int wildBattle(int level, int levelMod,
               int pokemon)  // battle functino for wild pokemon
{
  if (levelMod > 0) level += rand() % levelMod;
  int i = 0, tCurHP = stat("HP", pokemon, level);
  int choice, winner = 0, intestatus = 0;
  double pstatmods[7] = {1,1,1,1,1,1,1}, estatmods[7] = {1,1,1,1,1,1,1};
  
  string pstatus, estatus;

  cout << "\nA wild " << pokedexname[pokemon] << " appeared!" << endl;

  while (party[i][2] == 0) i++;
  cout << pName << " sent out " << pokedexname[party[i][0]] << "!" << endl;

  while (party[i][2] != 0)
    while (party[i][2] != 0) {
  		estatus = status(intestatus);
		      	
      cout << "\n\t\t\t" << tCurHP << "/" << stat("HP", pokemon, level)
           << setw(5) << estatus << '\t' << "lv. " << level << " " << pokedexname[pokemon] << endl
           << endl;
      // enemy poke data
      
  		pstatus = status(party[i][8]);

      cout << "lv. " << party[i][1] << " " << pokedexname[party[i][0]] << '\t'
           << party[i][2] << "/" << stat("HP", party[i][0], party[i][1])
           << setw(5) << pstatus << endl;
      // your poke data

      cout << "Use " << movelist[party[i][4]] << " (1), "
           << movelist[party[i][5]] << " (2), " << movelist[party[i][6]]
           << " (3), " << movelist[party[i][7]]
           << " (4), Throw ball (5), Run (6), Change pokemon (7): ";
      cin >> choice;  // move select

      while (cin.fail())  // bad inputcheck
      {
        cin.clear();
        cin.ignore();
        cin >> choice;
      }

      if (choice == 5)  // ball throw
      {
        int caught = capture(pokemon, tCurHP, stat("HP", pokemon, level), intestatus);
        if (caught == 1) {
          cout << "You caught a " << pokedexname[pokemon] << "!" << endl;
          pokeget(pokemon, level, tCurHP, intestatus);
          return 1;
        } else {
          cout << "Oh no! It broke free!" << endl;
        }

        choice = 0;

      } else if (choice == 6)  // run check
      {
        if (stat("S", party[i][0], party[i][1]) > stat("S", pokemon, level)) {
          cout << "Got away safely. . . " << endl;
          return 1;
        } else if (stat("S", party[i][0], party[i][1]) /
                       stat("S", pokemon, level) >
                   (rand() % 100) / 100) {
          cout << "Got away safely. . . " << endl;
          return 1;
        } else {
          cout << "Couldn't get away!" << endl;
        }

        choice = 0;
      }

      else if (choice == 7) { // swap
        int temppoke[8], pokemon2;

        for (int j = 0; j < 6; j++)
          if (party[j][0] != 0 && party[j][0] != party[i][0])
            cout << "(" << j + 1 << ") " << setw(10) << pokedexname[party[j][0]]
                 << '\t' << "Lvl " << party[j][1] << '\t' << party[j][2] << "/"
                 << stat("HP", party[j][0], party[j][1]) << setw(5) << status(party[j][8]) << endl;

        cout << "Choose another pokemon to swap with: ";
        cin >> pokemon2;
        while (cin.fail())  // bad inputcheck
        {
          cin.clear();
          cin.ignore();
          cin >> pokemon2;
        }
        while ((pokemon2 <= 1 && pokemon2 >= 6) ||
               party[pokemon2 - 1][0] == 0) {
          cin.clear();
          cin.ignore(100, '\n');
          cin >> pokemon2;
        }

        pokemon2--;
        cout << pokedexname[party[i][0]] << ", return!" << endl;
        cout << "Go, " << pokedexname[party[pokemon2][0]] << "!" << endl;
        for (int j = 0; j < 9; j++) {
          temppoke[j] = party[pokemon2][j];
          party[pokemon2][j] = party[i][j];
          party[i][j] = temppoke[j];
        }
        for (int j = 0; j < 7; j++) {
        	pstatmods[j] = 1;
        }

        choice = 0;
      } else {
        if (choice > 4 || choice < 1)  // out of bounds check
          choice = 1;
        choice = party[i][choice + 3];
      }

      turnResult(tCurHP, party[i][2], pokemon, party[i][0], level, party[i][1],
                 moveselect(pokemon, level, party[i][0]), choice, intestatus, party[i][8], 
				 estatmods, pstatmods);

      if (tCurHP == 0) {
        cout << "\nThe wild " << pokedexname[pokemon] << " fainted!" << endl;
        xp(1, pokemon, level, party[i][0], party[i][3], party[i][1], i);
        return 1;
      }

      int loss = 1;
      if (party[i][2] == 0)  // if the battle pokemon faints
      {
        cout << "Oh no! " << pName << "'s " << pokedexname[party[i][0]]
             << " fainted!" << endl;
        for (int k = 0; k < 6; k++)  // check there are remaining pokemon
        {
          if (party[k][2] > 0) loss = 0;
        }
        if (loss == 0)  // if there are remaining pokemon, print out pokemon
        {
          cout << "Send out: " << endl;
          for (int k = 0; k < 6; k++) {
            if (party[k][2] > 0)
              cout << "(" << k + 1 << ") " << setw(10)
                   << pokedexname[party[k][0]] << '\t' << "Lvl " << party[k][1]
                   << '\t' << party[k][2] << "/"
                   << stat("HP", party[k][0], party[k][1]) << endl;
          }
        }

        if (loss == 1)  // if no remaining pokemon, end battle
        {
          cout << pName << " has been defeated!\n" << pName
               << " blacked out. . ." << endl
               << endl;
          return 0;
        }
        i = 1;
        while ((i <= 1 && i >= 6) || party[i - 1][0] == 0 ||
               party[i - 1][2] == 0) {
          cin >> i;
          while (cin.fail()) {
            cin.clear();
            cin.ignore();
            cin >> i;
          }
        }
        cout << pName << " sent out " << pokedexname[party[--i][0]] << "!"
             << endl
             << endl;
        // send out choice
      }
    }

  return 0;
}

int wildpoke(int x, int y)  // randoming which wild pokemon per area
{
  int i, j, k, pokemon[10][11][4], temppoke[32][4], tempmap[10][11];

  ifstream pokeloader;

  pokeloader.open("wild pokemon list.txt");
  for (i = 0; i < 32; i++) {
    for (j = 0; j < 4; j++) {
      pokeloader >> temppoke[i][j];
      // cout << temppoke[i][j];
    }
    // cout << '\n';
  }
  pokeloader.close();

  pokeloader.open("pokemap.txt");
  for (i = 0; i < 11; i++) {
    for (j = 0; j < 10; j++) {
      pokeloader >> tempmap[j][i];
      // cout << tempmap[i][j]<< " ";
    }
    // cout << "\n";
  }
  pokeloader.close();

  for (i = 0; i < 10; i++)
    for (j = 0; j < 11; j++)
      for (k = 0; k < 4; k++) pokemon[i][j][k] = temppoke[tempmap[i][j]][k];

  int wildpokemon = pokemon[x][y][rand() % 4];

  return wildpokemon;
}

int capture(int pokemon, int hp, int maxhp, int status)  // capture check
{
  double statmult;
  
  switch(status) {
  	case 0:
	  statmult = 1;
	case 1:
	case 2:
	case 3:
		statmult = 1.5;
	case 4:
	case 5:
		statmult = 2;
	}
  
  if (rand() % 255 < 80)
    if (statmult * (maxhp * 255 * 4) / hp >= rand() % 255) return 1;

  return 0;
}

int trainerBattle(string job, string name, int templevel, int levelMod,
                  int SLMod, int pokemon0, int pokemon1, int pokemon2,
                  int pokemon3, int pokemon4, int pokemon5) {
  int level = templevel, intestatus = 0;
  string pstatus, estatus;
  double pstatmods[7] = {1,1,1,1,1,1,1}, estatmods[7] = {1,1,1,1,1,1,1};
  
  
  if (levelMod > 0) level += rand() % levelMod;

  int i = 0, j = 0, tCurHP = stat("HP", pokemon0, level);
  int choice, winner = 0;
  int epoke[6]{pokemon0, pokemon1, pokemon2, pokemon3, pokemon4, pokemon5};

  cout << endl
       << job << " " << name << " has challenged you to a battle!" << endl;
  cout << job << " " << name << " sent out " << pokedexname[pokemon0] << "!"
       << endl;

  while (party[i][2] == 0) i++;
  cout << pName << " sent out " << pokedexname[party[i][0]] << "!" << endl;

  while (party[i][2] != 0)
    while (epoke[j] > 0 && party[i][2] != 0) {
    	
  		estatus = status(intestatus);
		      	
      cout << "\n\t\t\t" << tCurHP << "/" << stat("HP", epoke[j], level)
           << setw(5) << estatus << '\t' << "lv. " << level << " " << pokedexname[epoke[j]] << endl
           << endl;
      // enemy poke data
      
  		pstatus = status(party[i][8]);

      cout << "lv. " << party[i][1] << " " << pokedexname[party[i][0]] << '\t'
           << party[i][2] << "/" << stat("HP", party[i][0], party[i][1])
           << setw(5) << pstatus << endl;
      // your poke data

      cout << "Use " << movelist[party[i][4]] << " (1), "
           << movelist[party[i][5]] << " (2), " << movelist[party[i][6]]
           << " (3), " << movelist[party[i][7]] << " (4), Change pokemon (5): ";
      cin >> choice;

      while (cin.fail()) {
        cin.clear();
        cin.ignore();
        cin >> choice;
      }

      if (choice == 5) { // swap
        int temppoke[8], pokemon2;

        for (int j = 0; j < 6; j++)
          if (party[j][0] != 0 && party[j][0] != party[i][0])
            cout << "(" << j + 1 << ") " << setw(10) << pokedexname[party[j][0]]
                 << '\t' << "Lvl " << party[j][1] << '\t' << party[j][2] << "/"
                 << stat("HP", party[j][0], party[j][1]) << setw(5) << status(party[j][8]) << endl;

        cout << "Choose another pokemon to swap with: ";
        cin >> pokemon2;
        while (cin.fail())  // bad inputcheck
        {
          cin.clear();
          cin.ignore();
          cin >> pokemon2;
        }
        while ((pokemon2 <= 1 && pokemon2 >= 6) ||
               party[pokemon2 - 1][0] == 0) {
          cin.clear();
          cin.ignore(100, '\n');
          cin >> pokemon2;
        }

        pokemon2--;
        cout << pokedexname[party[i][0]] << ", return!" << endl;
        cout << "Go, " << pokedexname[party[pokemon2][0]] << "!" << endl;
        for (int j = 0; j < 9; j++) {
          temppoke[j] = party[pokemon2][j];
          party[pokemon2][j] = party[i][j];
          party[i][j] = temppoke[j];
        }
        for (int j = 0; j < 7; j++) {
        	pstatmods[j] = 1;
        }

        choice = 0;
      } else {
        if (choice > 4 || choice < 1)  // out of bounds check
          choice = 1;
        choice = party[i][choice + 3];
      }

      turnResult(tCurHP, party[i][2], epoke[j], party[i][0], level, party[i][1],
                 moveselect(epoke[j], level, party[i][0]), choice, intestatus, party[i][8],
				 estatmods, pstatmods);

      if (tCurHP == 0) {
        cout << job << " " << name << "'s " << pokedexname[epoke[j]]
             << " fainted!" << endl;
        xp(1.5, epoke[j], level, party[i][0], party[i][3], party[i][1], i);

        if (epoke[++j] == 0) {
          cout << endl
               << job << " " << name << " has been defeated!" << endl
               << endl;
          return 1;
        } else {
          cout << job << " " << name << " sent out " << pokedexname[epoke[j]]
               << endl;
          if (levelMod > 0) level = templevel + rand() % levelMod;
          level += SLMod;
          tCurHP = stat("HP", epoke[j], level);
          for (int j = 0; j < 7; j++) estatmods[j] = 1;
          intestatus = 0;
        }
      }

      int loss = 1;
      if (party[i][2] == 0) {
        cout << "Oh no! " << pName << "'s " << pokedexname[party[i][0]]
             << " fainted!" << endl;
        for (int k = 0; k < 6; k++) {
          if (party[k][2] > 0) loss = 0;
        }
        if (loss == 0) {
          cout << "Send out: " << endl;
          for (int k = 0; k < 6; k++) {
            if (party[k][2] > 0)
              cout << "(" << k + 1 << ") " << setw(10)
                   << pokedexname[party[k][0]] << '\t' << "Lvl " << party[k][1]
                   << '\t' << party[k][2] << "/"
                   << stat("HP", party[k][0], party[k][1]) << endl;
          }
        }

        if (loss == 1) {
          cout << pName << " has been defeated!\n" << pName
               << " blacked out. . ." << endl
               << endl;
          return 0;
        }
        i = 1;
        while ((i <= 1 && i >= 6) || party[i - 1][0] == 0 ||
               party[i - 1][2] == 0) {
          cin >> i;
          while (cin.fail()) {
            cin.clear();
            cin.ignore();
            cin >> i;
          }
        }
        cout << pName << " sent out " << pokedexname[party[--i][0]] << "!"
             << endl
             << endl;
      }
    }

  return 0;
}

void xp(double trainer, int epoke, int elvl, int ppoke, int& pexp, int& plvl,
        int partypos)  // calcs xp gain, move learning, evolution
{
  int i, basesum = 0;

  for (i = 0; i < 6; i++) basesum += pokestat[epoke][i];
  int b = 0.47 * basesum - 55;
  int dexp = trainer * b * elvl / 7;

  pexp += dexp;
  int tempxp = pexp;

  cout << pokedexname[ppoke] << " gained " << dexp << " experience points."
       << endl;

  //	cout << static_cast<int>(pow(tempxp,(1/3))) << " " << pow(tempxp,(1/3))
  //<< " " << cbrt(tempxp) << endl;

  while (static_cast<int>(cbrt(tempxp)) > plvl) {	
    cout << pokedexname[ppoke] << " has leveled up!" << endl;
    plvl++;
    if (plvl >= pokestat[ppoke][6]) {
      cout << endl << pokedexname[ppoke] << " is evolving!!" << endl;
      cout << ". . . " << pokedexname[ppoke] << " evolved into "
           << pokedexname[++party[partypos][0]] << "!!!" << endl
           << endl;
      ppoke++;
    }
    if (moveset[ppoke][plvl] != 0) {
      for (i = 4; i < 8; i++)
        if (party[partypos][i] == 0) {
          party[partypos][i] = moveset[ppoke][plvl];
          cout << pokedexname[ppoke] << " learned "
               << movelist[party[partypos][i]] << "!" << endl;

          break;
        }
      if (i == 8) {
        int choice;
        cout << pokedexname[ppoke] << " would like to learn "
             << movelist[moveset[ppoke][plvl]]
             << ", but already knows 4 moves." << endl;
        cout << "Which move should be deleted?" << endl;
        cout << movelist[party[partypos][4]] << " (1), "
             << movelist[party[partypos][5]] << " (2), "
             << movelist[party[partypos][6]] << " (3), "
             << movelist[party[partypos][7]] << " (4), "
             << "Don't learn (5): ";
        cin >> choice;
        while (cin.fail()) {
          cin.clear();
          cin.ignore();
          cin >> i;
        }
        if (choice == 5) {
        	cout << pokedexname[ppoke] << " didn't learn " << movelist[moveset[ppoke][plvl]] << "." << endl;
        }
        else {
        cout << pokedexname[ppoke] << " forgot " << movelist[party[partypos][choice + 3]]
             << ", and learned " << movelist[moveset[ppoke][plvl]] << "!" << endl;
        party[partypos][choice + 3] = moveset[ppoke][plvl];
    	}
      }
    }
  }
}

void turnResult(int& eHP, int& pHP, int epoke, int ppoke, int elvl, int plvl,
                int emove, int pmove, int& estatus, int& status, double estats[], double pstats[])  // negotiates turns
{
  int damage=0, efrz=0, pfrz=0;
  
  if (estatus == 4 || estatus == 5) efrz = 1;
  if (status == 4 || status == 4) pfrz = 1;

  double modifier, attack, defense, stab = 1, crit = 1;

  if (stat("S", ppoke, plvl) * pstats[4] >= stat("S", epoke, elvl) * estats[4]) {
    if (pmove != 0) {
    	if (status != 3 || (status == 3 && rand()%100 <= 50)){
    		if (pfrz == 1) {
    			if (rand() % 100 <= 40) { 
    				if (status == 4) cout << pokedexname[ppoke] << " thawed itself!" << endl;
    				else cout << pokedexname[ppoke] << " woke up!" << endl;
    				pfrz = 0;
    				status = 0;
    			}
    		}
    		if (pfrz != 1) {
		      cout << pokedexname[ppoke] << " used " << movelist[pmove] << "! ";
		      if ((rand() % 100) <= movestats[pmove][2] * (pstats[5]/estats[6]) || movestats[pmove][2] == 0) { 
			      	if (movestats[pmove][1] != 0) { 
				        // player first
				        if (movestats[pmove][0] == 1 || movestats[pmove][0] == 2 ||
				            movestats[pmove][0] == 3 || movestats[pmove][0] == 4 ||
				            movestats[pmove][0] == 5 || movestats[pmove][0] == 10) {
				          attack = stat("SA", ppoke, plvl) * pstats[2];
				          defense = stat("SD", epoke, elvl) * estats[3];
				        } else {
				          attack = stat("A", ppoke, plvl) * pstats[0];
				          defense = stat("D", epoke, elvl) * estats[1]	;
				        }
				        
				        if (status == 2) attack /= 2;
				
				        if (rand() % 16 == 1) {
				          crit = 2;
				          cout << " Critical hit! ";
				        }
				
				        if (movestats[pmove][0] == pokestat[ppoke][8] ||
				            movestats[pmove][0] == pokestat[ppoke][9])
				          stab = 1.5;
				
				        if (effective(pmove, epoke) > 1)
				          cout << " Super effective! ";
				        else if (effective(pmove, epoke) < 1)
				          cout << " It's not very effective. . . ";
				        else if (effective(pmove, epoke) == 0)
				          cout << "It had no effect! ";
				        else
				          ;
				
				        switch (pmove) {
				          case 99:  // psywave
				            damage = plvl * ((rand() % 6) / 10.0 + 1);
				            break;
				          case 139:  // super fang
				            damage = eHP / 2;
				            break;
				          case 35:  // dragon rage
				            damage = 40;
				            break;
				          case 88:   // night shade
				          case 114:  // seismic toss
				            if (effective(pmove, epoke) == 0) {
				              damage = 0;
				            } else {
				              damage = plvl;
				            }
				            break;
				          case 128:  // sonic boom
				            if (effective(pmove, epoke) == 0) {
				              damage = 0;
				            } else {
				              damage = 20;
				            }
				            break;
				          default:
				            modifier = stab * effective(pmove, epoke) *
				                       (1 - rand() % 15 / 100.0) * crit;
				            damage = (((2.0 * plvl + 10) / 250) * (attack / defense) *
				                          (movestats[pmove][1]) +
				                      2) *
				                     modifier;
				            break;
				        }
			    }
		
			    if (movestats[pmove][3] != 0) { 
			    	if ((rand() % 100) <= movestats[pmove][3]) { 
				    	if (movestats[pmove][5] == 1) { // heal
				    		pHP += .5*damage;
				    		cout << pokedexname[ppoke] << " regained health!" << endl;
				    	}
				    	if (movestats[pmove][6] == 1) { // attack modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[0] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its attack lowered!" << endl;
				    		}
				    		else {
				    			pstats[0] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its attack!" << endl;
				    		}
				    	}
				    	if (movestats[pmove][7] == 1) { // def modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[1] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its defense lowered!" << endl;
				    		}
				    		else {
				    			pstats[1] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its defense!" << endl;
				    		}
				    	}
				    	if (movestats[pmove][8] == 1) { // sp attack modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[2] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its special attack lowered!" << endl;
				    		}
				    		else {
				    			pstats[2] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its special attack!" << endl;
				    		}
				    	}
				    	if (movestats[pmove][9] == 1) { // sp def modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[3] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its special defense lowered!" << endl;
				    		}
				    		else {
				    			pstats[3] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its special defense!" << endl;
				    		}
				    	}
				    	if (movestats[pmove][10] == 1) { // speed modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[4] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its speed lowered!" << endl;
				    		}
				    		else {
				    			pstats[4] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its speed!" << endl;
				    		}
				    	}
				    	if (movestats[pmove][11] == 1) { // acc modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[5] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its accuracy lowered!" << endl;
				    		}
				    		else {
				    			pstats[5] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its accuracy!" << endl;;
				    		}
				    	}
					    if (movestats[pmove][12] == 1) { // eva modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[6] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its evasion lowered!" << endl;
				    		}
				    		else {
				    			pstats[6] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its evasion!" << endl;
				    		}
				    	}
				    	
				    	if (movestats[pmove][13] == 1) { // poison
				    		estatus = 1;
				    		cout << "Enemy " << pokedexname[epoke] << " was poisoned!" << endl;
				    	}
				    	else if (movestats[pmove][14] == 1) { // burn
				    		estatus = 2;
				    		cout << "Enemy " << pokedexname[epoke] << " was burned!" << endl;
				    	}
				    	else if (movestats[pmove][15] == 1) { //paralyze
				    		estatus = 3;
				    		cout << "Enemy " << pokedexname[epoke] << " was paralyzed!" << endl;
				    	}
				  		else if (movestats[pmove][16] == 1) { // freeze
				  			estatus = 4;
				    		cout << "Enemy " << pokedexname[epoke] << " was frozen solid!" << endl;
				  		}
				  		else if (movestats[pmove][17] == 1) { //sleep
				  			estatus = 5;
				    		cout << "Enemy " << pokedexname[epoke] << " was put to sleep!" << endl;
				  		}
				  		else {
				  			;
				  		}
				    }
			  		else {
			  			if (movestats[pmove][1] == 0) cout << "But it failed!" << endl;
			  		}
				}   	    		
		
		        cout << endl;
		
		        stab = 1;
		        crit = 1;
		
		        if (eHP - damage <= 0) {
		          eHP = 0;
		          return;
		        } else
		          eHP -= damage;
		      } else
		        cout << " But it missed!" << endl;
	    	}
	    	else {
				if (status == 4) cout << pokedexname[ppoke] << " is frozen and can't move!" << endl;
				else cout << pokedexname[ppoke] << " is fast asleep!" << endl;
			}
	    		
	    }
	    else cout << pokedexname[ppoke] << " is paralyzed and can't move!" << endl;
	}

    // enemy turn
    damage = 0;
	if (estatus != 3 || (estatus == 3 && rand()%100 <= 50)){
		if (efrz == 1) {
			if (rand() % 100 <= 40) { 
				if (estatus == 4) cout << "Enemy " << pokedexname[epoke] << " thawed itself!" << endl;
				else cout << "Enemy " << pokedexname[epoke] << " woke up!" << endl;
    			efrz = 0;
    			estatus = 0;
			}
		}
		if (efrz != 1) {
		    cout << "Enemy " << pokedexname[epoke] << " used " << movelist[emove]
		         << "! ";
		    if ((rand() % 100) <= movestats[emove][2] * (estats[5]/pstats[6]) || movestats[emove][2] == 0) {
				if (movestats[emove][1] != 0) {
		    	
			      if (movestats[emove][0] == 1 || movestats[emove][0] == 2 ||
			          movestats[emove][0] == 3 || movestats[emove][0] == 4 ||
			          movestats[emove][0] == 5 || movestats[emove][0] == 10) {
			        attack = stat("SA", epoke, plvl) * estats[2];
			        defense = stat("SD", ppoke, elvl) * pstats[3];
			      } else {
			        attack = stat("A", epoke, plvl) * estats[0];
			        defense = stat("D", ppoke, elvl) * pstats[1];
			      }
					        
			      if (status == 2) attack /= 2;
			      if (movestats[emove][0] == pokestat[epoke][8] ||
			          movestats[emove][0] == pokestat[epoke][9])
			        stab = 1.5;
			
			      if (rand() % 16 == 1) {
			        crit = 2;
			        cout << " Critical hit! ";
			      }
			
			      if (effective(emove, ppoke) > 1)
			        cout << " Super effective! ";
			      else if (effective(emove, ppoke) < 1)
			        cout << " It's not very effective. . . ";
			      else if (effective(emove, ppoke) == 0)
			        cout << "It had no effect! ";
			      else
			        ;
			
			      switch (emove) {
			        case 99:  // psywave
			          damage = elvl * ((rand() % 6) / 10.0 + 1);
			          break;
			        case 139:  // super fang
			          damage = pHP / 2;
			          break;
			        case 35:  // dragon rage
			          damage = 40;
			          break;
			        case 88:   // night shade
			        case 114:  // seismic toss
			          if (effective(emove, ppoke) == 0) {
			            damage = 0;
			          } else {
			            damage = elvl;
			          }
			          break;
			        case 128:  // sonic boom
			          if (effective(emove, ppoke) == 0) {
			            damage = 0;
			          } else {
			            damage = 20;
			          }
			          break;
			        default:
			          modifier =
			              stab * effective(emove, ppoke) * (1 - rand() % 15 / 100.0) * crit;
			          damage = (((2.0 * elvl + 10) / 250) * (attack / defense) *
			                        (movestats[emove][1]) +
			                    2) *
			                   modifier;
			          break;
			      }
			  }
			    if (movestats[emove][3] != 0) {
			    	if ((rand() % 100) <= movestats[emove][3]) {
				    	if (movestats[emove][5] == 1) { // heal
				    		eHP += .5*damage;
				    		cout << "Enemy " << pokedexname[epoke] << " regained health!" << endl;
				    	}
				    	if (movestats[emove][6] == 1) { // attack modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[0] *= .67;
				    			cout << pokedexname[ppoke] << " had its attack lowered!" << endl;
				    		}
				    		else {
				    			estats[0] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its attack!" << endl;
				    		}
				    	}
				    	if (movestats[emove][7] == 1) { // def modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[1] *= .67;
				    			cout << pokedexname[ppoke] << " had its defense lowered!" << endl;
				    		}
				    		else {
				    			estats[1] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its defense!" << endl;
				    		}
				    	}
				    	if (movestats[emove][8] == 1) { // sp attack modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[2] *= .67;
				    			cout << pokedexname[ppoke] << " had its special attack lowered!" << endl;
				    		}
				    		else {
				    			estats[2] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its special attack!" << endl;
				    		}
				    	}
				    	if (movestats[emove][9] == 1) { // sp def modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[3] *= .67;
				    			cout << pokedexname[ppoke] << " had its special defense lowered!" << endl;
				    		}
				    		else {
				    			estats[3] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its special defense!" << endl;
				    		}
				    	}
				    	if (movestats[emove][10] == 1) { // speed modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[4] *= .67;
				    			cout << pokedexname[ppoke] << " had its speed lowered!" << endl;
				    		}
				    		else {
				    			estats[4] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its speed!" << endl;
				    		}
				    	}
				    	if (movestats[emove][11] == 1) { // acc modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[5] *= .67;
				    			cout << pokedexname[ppoke] << " had its accuracy lowered!" << endl;
				    		}
				    		else {
				    			estats[5] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its accuracy!" << endl;;
				    		}
				    	}
					    if (movestats[emove][12] == 1) { // eva modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[6] *= .67;
				    			cout << pokedexname[ppoke] << " had its evasion lowered!" << endl;
				    		}
				    		else {
				    			estats[6] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its evasion!" << endl;
				    		}
				    	}
				    	
				    	if (movestats[emove][13] == 1) { // poison
				    		status = 1;
				    		cout << pokedexname[ppoke] << " was poisoned!" << endl;
				    	}
				    	else if (movestats[emove][14] == 1) { // burn
				    		status = 2;
				    		cout << pokedexname[ppoke] << " was burned!" << endl;
				    	}
				    	else if (movestats[emove][15] == 1) { //paralyze
				    		status = 3;
				    		cout << pokedexname[ppoke] << " was paralyzed!" << endl;
				    	}
				  		else if (movestats[emove][16] == 1) { // freeze
				  			status = 4;
				    		cout << pokedexname[ppoke] << " was frozen solid!" << endl;
				  		}
				  		else if (movestats[emove][17] == 1) { //sleep
				  			status = 5;
				    		cout << pokedexname[ppoke] << " was put to sleep!" << endl;
				  		}
				  		else {
				  			;
				  		}
				    }
			  		else {
			  			if (movestats[emove][1] == 0) cout << "But it failed!" << endl;
			  		}
				}      
		
		
		      cout << endl;
		
		      stab = 1;
		      crit = 1;
		
		      if (pHP - damage <= 0) {
		        pHP = 0;
		        return;
		      } else
		        pHP -= damage;
		    } else
		      cout << " But it missed!" << endl;
		  }
    	else {
			if (estatus == 4) cout << "Enemy " << pokedexname[epoke] << " is frozen and can't move!" << endl;
			else cout << "Enemy " << pokedexname[epoke] << " is fast asleep!" << endl;
		}
	}
	else cout << "Enemy " << pokedexname[epoke] << " is paralyzed and can't move!" << endl;
  } else {
    // enemy first
    damage = 0;
	if (estatus != 3 || (estatus == 3 && rand()%100 <= 50)){
		if (efrz == 1) {
			if (rand() % 100 <= 40) { 
				if (estatus == 4) cout << "Enemy " << pokedexname[epoke] << " thawed itself!" << endl;
				else cout << "Enemy " << pokedexname[epoke] << " woke up!" << endl;
    			efrz = 0;
    			estatus = 0;
			}
		}
		if (efrz != 1) {
		    cout << "Enemy " << pokedexname[epoke] << " used " << movelist[emove]
		         << "! ";
		    if ((rand() % 100) <= movestats[emove][2] * (estats[5]/pstats[6]) || movestats[emove][2] == 0) {
				if (movestats[emove][1] != 0) {
		    	
			      if (movestats[emove][0] == 1 || movestats[emove][0] == 2 ||
			          movestats[emove][0] == 3 || movestats[emove][0] == 4 ||
			          movestats[emove][0] == 5 || movestats[emove][0] == 10) {
			        attack = stat("SA", epoke, plvl) * estats[2];
			        defense = stat("SD", ppoke, elvl) * pstats[3];
			      } else {
			        attack = stat("A", epoke, plvl) * estats[0];
			        defense = stat("D", ppoke, elvl) * pstats[1];
			      }
					        
			      if (status == 2) attack /= 2;
			      if (movestats[emove][0] == pokestat[epoke][8] ||
			          movestats[emove][0] == pokestat[epoke][9])
			        stab = 1.5;
			
			      if (rand() % 16 == 1) {
			        crit = 2;
			        cout << " Critical hit! ";
			      }
			
			      if (effective(emove, ppoke) > 1)
			        cout << " Super effective! ";
			      else if (effective(emove, ppoke) < 1)
			        cout << " It's not very effective. . . ";
			      else if (effective(emove, ppoke) == 0)
			        cout << "It had no effect! ";
			      else
			        ;
			
			      switch (emove) {
			        case 99:  // psywave
			          damage = elvl * ((rand() % 6) / 10.0 + 1);
			          break;
			        case 139:  // super fang
			          damage = pHP / 2;
			          break;
			        case 35:  // dragon rage
			          damage = 40;
			          break;
			        case 88:   // night shade
			        case 114:  // seismic toss
			          if (effective(emove, ppoke) == 0) {
			            damage = 0;
			          } else {
			            damage = elvl;
			          }
			          break;
			        case 128:  // sonic boom
			          if (effective(emove, ppoke) == 0) {
			            damage = 0;
			          } else {
			            damage = 20;
			          }
			          break;
			        default:
			          modifier =
			              stab * effective(emove, ppoke) * (1 - rand() % 15 / 100.0) * crit;
			          damage = (((2.0 * elvl + 10) / 250) * (attack / defense) *
			                        (movestats[emove][1]) +
			                    2) *
			                   modifier;
			          break;
			      }
			  }
			    if (movestats[emove][3] != 0) {
			    	if ((rand() % 100) <= movestats[emove][3]) {
				    	if (movestats[emove][5] == 1) { // heal
				    		eHP += .5*damage;
				    		cout << "Enemy " << pokedexname[epoke] << " regained health!" << endl;
				    	}
				    	if (movestats[emove][6] == 1) { // attack modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[0] *= .67;
				    			cout << pokedexname[ppoke] << " had its attack lowered!" << endl;
				    		}
				    		else {
				    			estats[0] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its attack!" << endl;
				    		}
				    	}
				    	if (movestats[emove][7] == 1) { // def modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[1] *= .67;
				    			cout << pokedexname[ppoke] << " had its defense lowered!" << endl;
				    		}
				    		else {
				    			estats[1] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its defense!" << endl;
				    		}
				    	}
				    	if (movestats[emove][8] == 1) { // sp attack modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[2] *= .67;
				    			cout << pokedexname[ppoke] << " had its special attack lowered!" << endl;
				    		}
				    		else {
				    			estats[2] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its special attack!" << endl;
				    		}
				    	}
				    	if (movestats[emove][9] == 1) { // sp def modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[3] *= .67;
				    			cout << pokedexname[ppoke] << " had its special defense lowered!" << endl;
				    		}
				    		else {
				    			estats[3] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its special defense!" << endl;
				    		}
				    	}
				    	if (movestats[emove][10] == 1) { // speed modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[4] *= .67;
				    			cout << pokedexname[ppoke] << " had its speed lowered!" << endl;
				    		}
				    		else {
				    			estats[4] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its speed!" << endl;
				    		}
				    	}
				    	if (movestats[emove][11] == 1) { // acc modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[5] *= .67;
				    			cout << pokedexname[ppoke] << " had its accuracy lowered!" << endl;
				    		}
				    		else {
				    			estats[5] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its accuracy!" << endl;;
				    		}
				    	}
					    if (movestats[emove][12] == 1) { // eva modifier
				    		if (movestats[emove][4] == 1) {
				    			pstats[6] *= .67;
				    			cout << pokedexname[ppoke] << " had its evasion lowered!" << endl;
				    		}
				    		else {
				    			estats[6] *= 1.67;
				    			cout << "Enemy " << pokedexname[epoke] << " raised its evasion!" << endl;
				    		}
				    	}
				    	
				    	if (movestats[emove][13] == 1) { // poison
				    		status = 1;
				    		cout << pokedexname[ppoke] << " was poisoned!" << endl;
				    	}
				    	else if (movestats[emove][14] == 1) { // burn
				    		status = 2;
				    		cout << pokedexname[ppoke] << " was burned!" << endl;
				    	}
				    	else if (movestats[emove][15] == 1) { //paralyze
				    		status = 3;
				    		cout << pokedexname[ppoke] << " was paralyzed!" << endl;
				    	}
				  		else if (movestats[emove][16] == 1) { // freeze
				  			status = 4;
				    		cout << pokedexname[ppoke] << " was frozen solid!" << endl;
				  		}
				  		else if (movestats[emove][17] == 1) { //sleep
				  			status = 5;
				    		cout << pokedexname[ppoke] << " was put to sleep!" << endl;
				  		}
				  		else {
				  			;
				  		}
				    }
			  		else {
			  			if (movestats[emove][1] == 0) cout << "But it failed!" << endl;
			  		}
				}      
		
		
		      cout << endl;
		
		      stab = 1;
		      crit = 1;
		
		      if (pHP - damage <= 0) {
		        pHP = 0;
		        return;
		      } else
		        pHP -= damage;
		    } else
		      cout << " But it missed!" << endl;
		  }
    	else {
			if (estatus == 4) cout << "Enemy " << pokedexname[epoke] << " is frozen and can't move!" << endl;
			else cout << "Enemy " << pokedexname[epoke] << " is fast asleep!" << endl;
		}
	}
	else cout << "Enemy " << pokedexname[epoke] << " is paralyzed and can't move!" << endl;
	
	
    // player turn
    damage = 0;
    if (pmove != 0) {
    	if (status != 3 || (status == 3 && rand()%100 <= 50)){
    		if (pfrz == 1) {
    			if (rand() % 100 <= 40) { 
    				if (status == 3) cout << pokedexname[ppoke] << " thawed itself!" << endl;
    				else cout << pokedexname[ppoke] << " woke up!" << endl;
    				pfrz = 0;
    				status = 0;
    			}
    		}
    		if (pfrz != 1) {
		      cout << pokedexname[ppoke] << " used " << movelist[pmove] << "! ";
		      if ((rand() % 100) <= movestats[pmove][2] * (pstats[5]/estats[6]) || movestats[pmove][2] == 0) { 
			      	if (movestats[pmove][1] != 0) { 
				        // player first
				        if (movestats[pmove][0] == 1 || movestats[pmove][0] == 2 ||
				            movestats[pmove][0] == 3 || movestats[pmove][0] == 4 ||
				            movestats[pmove][0] == 5 || movestats[pmove][0] == 10) {
				          attack = stat("SA", ppoke, plvl) * pstats[2];
				          defense = stat("SD", epoke, elvl) * estats[3];
				        } else {
				          attack = stat("A", ppoke, plvl) * pstats[0];
				          defense = stat("D", epoke, elvl) * estats[1]	;
				        }
				        
				        if (status == 2) attack /= 2;
				
				        if (rand() % 16 == 1) {
				          crit = 2;
				          cout << " Critical hit! ";
				        }
				
				        if (movestats[pmove][0] == pokestat[ppoke][8] ||
				            movestats[pmove][0] == pokestat[ppoke][9])
				          stab = 1.5;
				
				        if (effective(pmove, epoke) > 1)
				          cout << " Super effective! ";
				        else if (effective(pmove, epoke) < 1)
				          cout << " It's not very effective. . . ";
				        else if (effective(pmove, epoke) == 0)
				          cout << "It had no effect! ";
				        else
				          ;
				
				        switch (pmove) {
				          case 99:  // psywave
				            damage = plvl * ((rand() % 6) / 10.0 + 1);
				            break;
				          case 139:  // super fang
				            damage = eHP / 2;
				            break;
				          case 35:  // dragon rage
				            damage = 40;
				            break;
				          case 88:   // night shade
				          case 114:  // seismic toss
				            if (effective(pmove, epoke) == 0) {
				              damage = 0;
				            } else {
				              damage = plvl;
				            }
				            break;
				          case 128:  // sonic boom
				            if (effective(pmove, epoke) == 0) {
				              damage = 0;
				            } else {
				              damage = 20;
				            }
				            break;
				          default:
				            modifier = stab * effective(pmove, epoke) *
				                       (1 - rand() % 15 / 100.0) * crit;
				            damage = (((2.0 * plvl + 10) / 250) * (attack / defense) *
				                          (movestats[pmove][1]) +
				                      2) *
				                     modifier;
				            break;
				        }
			    }
		
			    if (movestats[pmove][3] != 0) { 
			    	if ((rand() % 100) <= movestats[pmove][3]) { 
				    	if (movestats[pmove][5] == 1) { // heal
				    		pHP += .5*damage;
				    		cout << pokedexname[ppoke] << " regained health!" << endl;
				    	}
				    	if (movestats[pmove][6] == 1) { // attack modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[0] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its attack lowered!" << endl;
				    		}
				    		else {
				    			pstats[0] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its attack!" << endl;
				    		}
				    	}
				    	if (movestats[pmove][7] == 1) { // def modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[1] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its defense lowered!" << endl;
				    		}
				    		else {
				    			pstats[1] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its defense!" << endl;
				    		}
				    	}
				    	if (movestats[pmove][8] == 1) { // sp attack modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[2] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its special attack lowered!" << endl;
				    		}
				    		else {
				    			pstats[2] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its special attack!" << endl;
				    		}
				    	}
				    	if (movestats[pmove][9] == 1) { // sp def modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[3] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its special defense lowered!" << endl;
				    		}
				    		else {
				    			pstats[3] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its special defense!" << endl;
				    		}
				    	}
				    	if (movestats[pmove][10] == 1) { // speed modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[4] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its speed lowered!" << endl;
				    		}
				    		else {
				    			pstats[4] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its speed!" << endl;
				    		}
				    	}
				    	if (movestats[pmove][11] == 1) { // acc modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[5] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its accuracy lowered!" << endl;
				    		}
				    		else {
				    			pstats[5] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its accuracy!" << endl;;
				    		}
				    	}
					    if (movestats[pmove][12] == 1) { // eva modifier
				    		if (movestats[pmove][4] == 1) {
				    			estats[6] *= .67;
				    			cout << "Enemy " << pokedexname[epoke] << " had its evasion lowered!" << endl;
				    		}
				    		else {
				    			pstats[6] *= 1.67;
				    			cout << pokedexname[ppoke] << " raised its evasion!" << endl;
				    		}
				    	}
				    	
				    	if (movestats[pmove][13] == 1) { // poison
				    		estatus = 1;
				    		cout << "Enemy " << pokedexname[epoke] << " was poisoned!" << endl;
				    	}
				    	else if (movestats[pmove][14] == 1) { // burn
				    		estatus = 2;
				    		cout << "Enemy " << pokedexname[epoke] << " was burned!" << endl;
				    	}
				    	else if (movestats[pmove][15] == 1) { //paralyze
				    		estatus = 3;
				    		cout << "Enemy " << pokedexname[epoke] << " was paralyzed!" << endl;
				    	}
				  		else if (movestats[pmove][16] == 1) { // freeze
				  			estatus = 4;
				    		cout << "Enemy " << pokedexname[epoke] << " was frozen solid!" << endl;
				  		}
				  		else if (movestats[pmove][17] == 1) { //sleep
				  			estatus = 5;
				    		cout << "Enemy " << pokedexname[epoke] << " was put to sleep!" << endl;
				  		}
				  		else {
				  			;
				  		}
				    }
			  		else {
			  			if (movestats[pmove][1] == 0) cout << "But it failed!" << endl;
			  		}
				}   	    		
		
		        cout << endl;
		
		        stab = 1;
		        crit = 1;
		
		        if (eHP - damage <= 0) {
		          eHP = 0;
		          return;
		        } else
		          eHP -= damage;
		      } else
		        cout << " But it missed!" << endl;
	    	}
	    	else {
				if (status == 4) cout << pokedexname[ppoke] << " is frozen and can't move!" << endl;
				else cout << pokedexname[ppoke] << " is fast asleep!" << endl;
			}
	    		
	    }
	    else cout << pokedexname[ppoke] << " is paralyzed and can't move!" << endl;
	}
  }
  
  if (eHP > 0 && (estatus == 2 || estatus == 1)) {
  	eHP -= (1.0/16)*stat("HP", epoke, elvl);
  	if (estatus == 1) {
  		cout << "Enemy " << pokedexname[epoke] << " was hurt by poison!" << endl;
  	}
  	else {
  		cout << "Enemy " << pokedexname[epoke] << " was hurt by its burn!" << endl;
  	}
  	if (eHP <= 0) eHP = 0;
  }
  	
 if (pHP > 0 && (status == 2 || status == 1)) {
  	pHP -= (1.0/16)*stat("HP", ppoke, plvl);
  	if (status == 1) {
  		cout << pokedexname[ppoke] << " was hurt by poison!" << endl;
  	}
  	else {
  		cout << pokedexname[ppoke] << " was hurt by its burn!" << endl;
  	}
  	if (pHP <= 0) pHP = 0;
  }
  
  
}

double effective(int move, int pokemon)  // evaluates move effectiveness
{
  return typechart[movestats[move][0]][pokestat[pokemon][7]] *
         typechart[movestats[move][0]][pokestat[pokemon][8]];
}

void pokeheal()
{
	for (int i = 0; i < 6; i++) {
		party[i][2] = stat("HP", party[i][0], party[i][1]);
		party[i][8] = 0;
		
	}
	cout << "Nurse Joy: All pokemon fully restored! We hope to see "
	    "you again!"
	 << endl;
	x = lastx;
	y = lasty;
	
}

int moveselect(int enemypokemon, int enemylvl, int pPokemon)  // enemy AI
{
  int templvl = enemylvl,
      move[4] = {moves(enemypokemon, templvl), moves(enemypokemon, templvl),
                 moves(enemypokemon, templvl), moves(enemypokemon, templvl)};

  int i, max = 0;

  //	for (i=0; i < 4; i++)
  //	{
  //		if (effective(move[i], pPokemon) > max+1)
  //			max = i;
  //	}

  int dmg = effective(move[0], pPokemon) * movestats[move[0]][1];
  int movesel = 0;
  for (i = 0; i < 4; i++) {
    if (effective(move[i], pPokemon) * movestats[move[i]][1] > dmg) {
      dmg = effective(move[i], pPokemon) * movestats[move[i]][1];
      movesel = i;
    }
  }

  return move[movesel];
}

int stat(string stat, int pokemon, int level)  // stat calculator
{
  if (pokemon == 0) return 0;
  if (stat == "HP") return pokestat[pokemon][0] * 2 * level / 100 + level + 10;
  if (stat == "A") return pokestat[pokemon][1] * 2 * level / 100 + 5;
  if (stat == "D") return pokestat[pokemon][2] * 2 * level / 100 + 5;
  if (stat == "SA") return pokestat[pokemon][3] * 2 * level / 100 + 5;
  if (stat == "SD") return pokestat[pokemon][4] * 2 * level / 100 + 5;
  if (stat == "S") return pokestat[pokemon][5] * 2 * level / 100 + 5;
}

int moves(int pokemon, int& level)  // loads moves on lvlup
{
  int moveID;

  while (moveset[pokemon][level] == 0 && level >= 0) {
    level--;
  }

  if (level >= 0)
    moveID = moveset[pokemon][level--];
  else
    moveID = 0;

  return moveID;
}

void pokeget(int pokemon, int level, int hp, int status)  // puts poke in box or bag
{
  int i = 0;

  while (party[i][0] != 0) i++;

  if (i > 5) {
    i = 0;
    while (box[i][0] != 0) i++;

    if (i > 29) {
      cout << "Oh no, the box is full! Please withraw or release a pokemon"
           << endl;
      return;
    }

    cout << "Transferred " << pokedexname[pokemon] << " to the box" << endl;

    box[i][0] = pokemon;
    box[i][1] = level;
    box[i][2] = hp;
    box[i][3] = pow(level, 3);
    box[i][4] = moves(pokemon, level);
    box[i][5] = moves(pokemon, level);
    box[i][6] = moves(pokemon, level);
    box[i][7] = moves(pokemon, level);
    box[i][8] = 0;

  }

  else {
    party[i][0] = pokemon;
    party[i][1] = level;
    party[i][2] = hp;
    party[i][3] = pow(level, 3);
    party[i][4] = moves(pokemon, level);
    party[i][5] = moves(pokemon, level);
    party[i][6] = moves(pokemon, level);
    party[i][7] = moves(pokemon, level);
    party[i][8] = status;
  }
}

void loadfile()  // loads global vars
{
  ifstream load;
  ifstream pokedexloader;
  ifstream boxF;
  ifstream partyF;
  ifstream maploader;
  ifstream statsloader;
  ifstream movesetloader;
  ifstream movelistloader;
  ifstream chartloader;
  ifstream tierloader;
  ifstream trainerloader;
  ifstream flagloader;

  int i, j;

  load.open("save.txt");
  load >> newgame >> gender;
  load.ignore(100, '\n');
  getline(load, pName);
  load >> x >> y >> garystarter;
  for (i = 0; i < 9; i++) load >> badge[i];
  load.close();

  flagloader.open("flags.txt");
  for (i = 0; i < 10; i++)
    for (j = 0; j < 11; j++) flagloader >> evFlags[i][j];
  flagloader.close();

  pokedexloader.open("pokedex.txt");
  for (i = 0; i < 152; ++i) {
    pokedexloader >> pokedexname[i];
  }
  pokedexloader.close();

  maploader.open("map.txt");
  for (i = 0; i < 11; i++) {
    for (j = 0; j < 10; j++) {
      getline(maploader, map[j][i]);
      //	cout << map[i][j] << "\t";
    }
    // cout << '\n';
  }
  maploader.close();
  maploader.open("map levels.txt");
  for (i = 0; i < 11; i++)
    for (j = 0; j < 10; j++) maploader >> maplvl[j][i];
  maploader.close();

  boxF.open("box.txt");
  for (i = 0; i < 9; i++)
    for (j = 0; j < 30; j++) boxF >> box[j][i];
  boxF.close();

  partyF.open("party.txt");
  for (i = 0; i < 9; i++)
    for (j = 0; j < 6; j++) partyF >> party[j][i];
  partyF.close();

  statsloader.open("stats.txt");
  for (i = 0; i < 152; i++)
    for (j = 0; j < 9; j++) statsloader >> pokestat[i][j];
  statsloader.close();

  int temp;
  movesetloader.open("moveset.txt");
  for (j = 0; j < 151; j++)
    for (i = 0; i < 93; i++) {
      movesetloader >> temp;
      moveset[j][i] = temp;
    }

  movesetloader.close();

  movelistloader.open("movelistnames.txt");
  for (i = 0; i < 166; i++) getline(movelistloader, movelist[i]);
  movelistloader.close();

  movelistloader.open("movestats.txt");
  for (i = 0; i < 166; i++)
    for (j = 0; j < 18; j++) movelistloader >> movestats[i + 1][j];
  movelistloader.close();

  chartloader.open("typechart.txt");
  for (i = 0; i < 16; i++)
    for (j = 0; j < 16; j++) chartloader >> typechart[i][j];
  chartloader.close();

  tierloader.open("tier 1.txt");
  for (i = 0; i < 23; i++) tierloader >> tier1[i];
  tierloader.close();

  tierloader.open("tier 2.txt");
  for (i = 0; i < 43; i++) tierloader >> tier2[i];
  tierloader.close();

  tierloader.open("tier 3.txt");
  for (i = 0; i < 65; i++) tierloader >> tier2[i];
  tierloader.close();

  trainerloader.open("jobs.txt");
  for (i = 0; i < 30; i++) trainerloader >> trainers[i][0];
  trainerloader.close();

  trainerloader.open("names.txt");
  for (i = 0; i < 30; i++) trainerloader >> trainers[i][1];
  trainerloader.close();
}

void savefile()  // saves save file, box, and party
{
  int i, j;

  ofstream save;
  ofstream boxS;
  ofstream partyS;
  ofstream flagS;

  boxS.open("box.txt");
  partyS.open("party.txt");

  flagS.open("flags.txt");
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 11; j++) {
      flagS << evFlags[i][j];
      flagS << " ";
    }
    flagS << '\n';
  }
  flagS.close();

  // saves box pokemon to box.txt
  for (j = 0; j < 9;
       j++)  // row dex#, level, exp, hp, move1, move2, move3, move4
  {
    for (i = 0; i < 30; i++)  // column pokemon
      boxS << box[i][j] << '\t';
    boxS << '\n';  // delineates rows
  }

  // saves party pokemon to praty.txt
  for (j = 0; j < 9;
       j++)  // row dex#, level, exp, hp, move1, move2, move3, move4
  {
    for (i = 0; i < 6; i++)  // column pokemon
      partyS << party[i][j] << '\t';
    partyS << '\n';  // delineates rows
  }

  save.open("save.txt");  // saves newgame flag, gender, name, coordinates,
                          // badges to save.txt
  save << newgame << '\n' << gender << '\n' << pName << '\n' << x << '\n' << y
       << '\n' << garystarter << endl;

  for (i = 0; i < 9; i++) save << '\n' << badge[i];

  cout << pName << " saved the game!" << endl;
}

void gamestart()  // pokeselect and first battle
{
  newgame = 1;
  memset(party, 0, sizeof(party[0][0]) * 6 * 8);
  memset(box, 0, sizeof(box[0][0]) * 30 * 8);
  cout << "Are you a boy or a girl?" << endl;
  gender = 1;
  while (gender != 'g' && gender != 'b') cin.get(gender);
  cin.ignore(100, '\n');
  cout << "What is your name?" << endl;
  getline(cin, pName);

  // cout << "\n\nPallet Town" << endl;
  cout << direct;

  char direction;

  while (1) {
    cin.get(direction);
    switch (direction) {
      case 'w':
        cout << "Oak: You can't go that way! It's dangerous without your own "
                "pokemon. Follow me"
             << endl;
        break;
      case 's':
        cout << "It's the sea. ";
        continue;
      case 'd':
      case 'a':
        cout << "Trees block your path. ";
      default:
        continue;
    }
    break;
  }

  cout << "Oak takes you to his lab and offers you 3 pokemon and a pokedex."
       << endl;
  cout << "Pick Bulbasaur (1), Charmander (2), or Squirtle (3)? ";

  int starter;

  while (1) {
    cin >> starter;
    switch (starter) {
      case 1:
        cout << "You got Bulbasaur!" << endl;
        pokeget(1, 5, stat("HP", 1, 5), 0);
        break;
      case 2:
        cout << "You got Charmander!" << endl;
        pokeget(4, 5, stat("HP", 4, 5), 0);
        break;
      case 3:
        cout << "You got Squirtle!" << endl;
        pokeget(7, 5, stat("HP", 7, 5), 0);
        break;
      default:
        continue;
    }
    break;
  }

  switch (party[0][0]) {
    case 1:
      garystarter = 4;
      break;
    case 4:
      garystarter = 7;
      break;
    case 7:
      garystarter = 1;
      break;
  }
  cout << "Oak's grandson barges in, snatches a remaining pokeball, and "
          "challenges you to a battle!"
       << endl;
  if (trainerBattle("Gary", "Oak", 5, 0, 0, garystarter) == 1)
    cout << "Gary: Wow, you really bested me. Smell ya later!" << endl;
  else
    cout << "Gary: Haha, loser. Smell ya later!" << endl;

  cout << "Oak: That was dramatic. Here is your pokedex. Go off now and "
          "explore the world of Pokemon!"
       << endl
       << endl;

  party[0][2] = stat("HP", party[0][0], party[0][1]);
}
